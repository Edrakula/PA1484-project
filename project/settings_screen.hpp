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

#include "backend_logic/apiconnections.hpp"


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

    
    static city_info_t info;
    info.updateAllTiles = updateAllTiles;

    lv_obj_add_event_cb(cityDropdown, city_dropdown_event_cb, LV_EVENT_VALUE_CHANGED, &info);
    
    lv_obj_align(cityDropdown, LV_ALIGN_CENTER, -200, 0);

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

