#ifndef SETTINGS_SCREEN_HPP
#define SETTINGS_SCREEN_HPP

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <time.h>
#include <LilyGo_AMOLED.h>
#include <LV_Helper.h>
#include <lvgl.h>
#include <Preferences.h>

#include "backend_logic/apiconnections.hpp"

extern Preferences prefs;

extern StationData defaultStation;
extern HistoricDataParameters defaultParam;
extern StationData currStation;
extern HistoricDataParameters currParam;

static lv_obj_t* settingsDefaultCityLabel;

void saveDefaultStation(const StationData& station, const HistoricDataParameters& param){
    prefs.begin("myapp", false);
    prefs.putString("name", station.name.c_str());
    prefs.putString("id", station.id.c_str());
    prefs.putString("lon", station.longitude.c_str());
    prefs.putString("lat", station.latitude.c_str());
    prefs.putUChar("param", static_cast<uint8_t>(defaultParam));
    prefs.end();
}


bool loadDefaultStation(StationData &station, HistoricDataParameters &param) {
    prefs.begin("myapp", true);

    station.name = (std::string) prefs.getString("name", "").c_str();
    station.id = (std::string) prefs.getString("id", "").c_str();
    station.longitude = (std::string) prefs.getString("lon", "").c_str();
    station.latitude = (std::string) prefs.getString("lat", "").c_str();

    
    if (!prefs.isKey("param")) {
        prefs.end();
        return false;   // nothing saved yet
    }

    uint8_t value = prefs.getUChar("param");
    

    prefs.end();

    param = static_cast<HistoricDataParameters>(value);

    // param = HISTORIC_TEMP;

    return station.name.length() > 0; // or any other validity check
}



typedef struct {
    void (*updateHistoricParam)(const HistoricDataParameters&) = nullptr;
} param_info_t;

void weather_dropdown_event_cb(lv_event_t* event)
{
    lv_obj_t* dropDownObject = lv_event_get_target(event);
    uint16_t selected = lv_dropdown_get_selected(dropDownObject);
    
    param_info_t* info = (param_info_t *) lv_event_get_user_data(event);

    switch (selected)
    {
        case 0:
            //Visa temperature.
            if (info->updateHistoricParam != nullptr) info->updateHistoricParam(HISTORIC_TEMP);
            break;
        case 1:
            //Visa humidity.
            if (info->updateHistoricParam != nullptr) info->updateHistoricParam(HISTORIC_HUMIDITY);
            break;
        case 2:
            //Visa wind speed.
            if (info->updateHistoricParam != nullptr) info->updateHistoricParam(HISTORIC_WIND_SPEED);
            break;
        case 3:
            //Visa air pressure.
            if (info->updateHistoricParam != nullptr) info->updateHistoricParam(HISTORIC_AIR_PRESSURE);
            break;
    }
}

typedef struct {
    void (*updateAllTiles)(const StationData&) = nullptr;
} city_info_t;

void city_dropdown_event_cb(lv_event_t* event) {
    lv_obj_t* dropdownObject = lv_event_get_target(event);
    uint16_t selected = lv_dropdown_get_selected(dropdownObject);
    
    city_info_t* info = (city_info_t *) lv_event_get_user_data(event);

    if (info->updateAllTiles != nullptr) info->updateAllTiles(STATIONS[selected]);
}



void setDeafultCityBtnOnClick(lv_event_t* event) {
    Serial.println("setting default");
    defaultStation.id = currStation.id;
    defaultStation.name = currStation.name;
    defaultStation.longitude = currStation.longitude;
    defaultStation.latitude = currStation.latitude;

    defaultParam = currParam;

    saveDefaultStation(defaultStation, defaultParam);
    // saveDefaultParam(defaultParam);

    
    std::string idk = "Default city,param: " + defaultStation.name + "," + param_to_string(defaultParam);

    lv_label_set_text(settingsDefaultCityLabel, idk.c_str());
}



void loadDeafultCityBtnOnClick(lv_event_t* event) {
    Serial.println("loading default");
    currStation = defaultStation;
    currParam = defaultParam;

    city_info_t* info = (city_info_t *) lv_event_get_user_data(event);

    if (info->updateAllTiles != nullptr) info->updateAllTiles(defaultStation);
}


void dropdown_select_by_string(lv_obj_t * dd, const char * target) {
    const char * opts = lv_dropdown_get_options(dd);
    if(!opts) return;

    char buf[128];
    int index = 0;

    const char * p = opts;
    while (*p) {
        int i = 0;

        /* Extract one option (until newline or end) */
        while (*p && *p != '\n' && i < sizeof(buf)-1) {
            buf[i++] = *p++;
        }
        buf[i] = '\0';   // null-terminate the option

        /* Compare with target string */
        if (strcmp(buf, target) == 0) {
            lv_dropdown_set_selected(dd, index);
            return;      // found and selected
        }

        /* Skip newline */
        if (*p == '\n') p++;

        index++;
    }
}



lv_obj_t* create_settings_screen(lv_obj_t* tileview, lv_obj_t* tile, void (*updateAllTiles)(const StationData&), void (*updateHistoricParam)(const HistoricDataParameters&))
{
    std::string settings_text = "Settings";
    lv_obj_t* settings_label1 = lv_label_create(tile);
    lv_label_set_text(settings_label1, settings_text.c_str());
    lv_obj_set_style_text_font(settings_label1, &lv_font_montserrat_32, LV_PART_MAIN);
    lv_obj_align(settings_label1, LV_ALIGN_TOP_MID, 0, 0);

    //Weather dropdown.
    lv_obj_t* weatherDropdown = lv_dropdown_create(tile);
    lv_dropdown_set_options(
        weatherDropdown,
        "Temperature\n"
        "Humidity\n"
        "Wind speed\n"
        "Air pressure"
    );

    switch (defaultParam)
    {
    case HISTORIC_TEMP:
        dropdown_select_by_string(weatherDropdown, "Temperature");
        break;
    case HISTORIC_HUMIDITY:
        dropdown_select_by_string(weatherDropdown, "Humidity");
        break;
    case HISTORIC_WIND_SPEED:
        dropdown_select_by_string(weatherDropdown, "Wind speed");
        break;
    case HISTORIC_AIR_PRESSURE:
        dropdown_select_by_string(weatherDropdown, "Air pressure");
        break;
    }

    lv_obj_align(weatherDropdown, LV_ALIGN_CENTER, 200, 0);

    static param_info_t param_info;
    param_info.updateHistoricParam = updateHistoricParam;

    lv_obj_add_event_cb(weatherDropdown, weather_dropdown_event_cb, LV_EVENT_VALUE_CHANGED, &param_info);

    //City dropdown.
    lv_obj_t* cityDropdown = lv_dropdown_create(tile);

    std::string cities_options_str = "";
    for (size_t i = 0; i < 4; i++){
        cities_options_str += STATIONS[i].name + "\n";
    }
    cities_options_str += STATIONS[4].name;

    lv_dropdown_set_options(
        cityDropdown,
        cities_options_str.c_str()
    );

    dropdown_select_by_string(cityDropdown, defaultStation.name.c_str());

    
    static city_info_t info;
    info.updateAllTiles = updateAllTiles;

    lv_obj_add_event_cb(cityDropdown, city_dropdown_event_cb, LV_EVENT_VALUE_CHANGED, &info);
    
    lv_obj_align(cityDropdown, LV_ALIGN_CENTER, -200, 0);


    settingsDefaultCityLabel = lv_label_create(tile);

    std::string idk = "Default city,param: " + defaultStation.name + "," + param_to_string(defaultParam);

    lv_label_set_text(settingsDefaultCityLabel, idk.c_str());

    lv_obj_t* setDefaultBtn = lv_btn_create(tile);
    
    lv_obj_set_size(setDefaultBtn, 150, 50);
    // lv_obj_center(setDefaultBtn);

    

    lv_obj_add_event_cb(setDefaultBtn, setDeafultCityBtnOnClick, LV_EVENT_PRESSED, NULL);

    lv_obj_t *setDefaultBtnlabel = lv_label_create(setDefaultBtn);
    lv_label_set_text(setDefaultBtnlabel, "Set Default");
    lv_obj_center(setDefaultBtnlabel);

    lv_obj_align(settingsDefaultCityLabel, LV_ALIGN_CENTER, 0, 50);
    lv_obj_align(setDefaultBtn, LV_ALIGN_CENTER, -200, 100);



    lv_obj_t* loadDefaultBtn = lv_btn_create(tile);
    
    lv_obj_set_size(loadDefaultBtn, 150, 50);
    // lv_obj_center(setDefaultBtn);

    lv_obj_add_event_cb(loadDefaultBtn, loadDeafultCityBtnOnClick, LV_EVENT_PRESSED, &info);

    lv_obj_t *loadDefaultBtnlabel = lv_label_create(loadDefaultBtn);
    lv_label_set_text(loadDefaultBtnlabel, "load Default");
    lv_obj_center(loadDefaultBtnlabel);

    lv_obj_align(loadDefaultBtn, LV_ALIGN_CENTER, 200, 100);


    return cityDropdown;

}

void populate_settings_screen_with_cities(lv_obj_t* cityDropdown, std::vector<StationData> stations) {
    
    std::string cities_options_str = "";
    size_t stations_size = stations.size();
    for (size_t i = 0; i < stations_size-1; i++){
        cities_options_str += stations[i].name + "\n";
    }
    cities_options_str += stations[stations_size-1].name;

    lv_dropdown_set_options(
        cityDropdown,
        cities_options_str.c_str()
    );

}


#endif

