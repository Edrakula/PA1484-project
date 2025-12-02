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
#include "boot_screen.hpp"
#include "historic_data_screen/historic_data_screen.hpp"
#include "forecast_screen/Bg_sunny.hpp"
#include "settings_screen.hpp"

#include <Preferences.h>

static const String VERSION = "1.0";

// Wi-Fi credentials (Delete these before commiting to GitHub)
static const char* WIFI_SSID     = "SSID";
static const char* WIFI_PASSWORD = "PASSWORD";

LilyGo_Class amoled;

static lv_obj_t* tileview;
static lv_obj_t* t1;
static lv_obj_t* t2;

static lv_obj_t* bootScreen;

static lv_obj_t* settingsScreen;

static lv_obj_t* cityDropdown;

static lv_obj_t* forecastDataCityLabel;
static lv_obj_t* historicDataCityLabel;

static StationData currStation;
static HistoricDataParameters currParam;

static std::vector<ForecastTemp> forecastTemps;
static std::vector<HistoricData> historicData;


static void updateHistoricDataBasedOnNewParam(const HistoricDataParameters& param) {
  currParam = param;
  Serial.println("getting historic data");
  Error err;
  historicData = getHistoricDataFromId(currStation.id, param, err);
  Serial.println("got historic data");
  if (!err) {
    Serial.println(historicData[0].date.ymdhms().c_str());
    populateGraph(&historicData);
  } else {
    Serial.println(err.msg.c_str());
  }
}

static void updateAllTilesBasedOnStation(const StationData& stationData) {
  currStation = stationData;
  Error err;
  Serial.println("getting forcastData");
  forecastTemps = getForecastFromLongAndLat(stationData.longitude, stationData.latitude, err);
  Serial.println("got forecastData");
  if (!err) {
    update_temperatures(forecastTemps, t1);
  } else {
    Serial.println(err.msg.c_str());
  }
  err = Error();    
  
  Serial.println("getting historic data");
  historicData = getHistoricDataFromId(stationData.id, HISTORIC_TEMP, err);
  Serial.println("got historic data");
  if (!err) {
    Serial.println(historicData[0].date.ymdhms().c_str());
    populateGraph(&historicData);
  } else {
    Serial.println(err.msg.c_str());
  }

  lv_label_set_text(forecastDataCityLabel, stationData.name.c_str());
  lv_label_set_text(historicDataCityLabel, stationData.name.c_str());

}


// Function: Creates UI
static void create_ui() {

  lv_style_t style_global;
  lv_style_init(&style_global);
  lv_style_set_text_font(&style_global, &lv_font_montserrat_16);

  lv_obj_add_style(lv_scr_act(), &style_global, 0);  
  lv_obj_add_style(lv_layer_top(), &style_global, 0); 
  lv_obj_add_style(lv_layer_sys(), &style_global, 0);
  


  // Fullscreen Tileview
  tileview = lv_tileview_create(lv_scr_act());
  lv_obj_set_size(tileview, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
  lv_obj_set_scrollbar_mode(tileview, LV_SCROLLBAR_MODE_OFF);

  // Add two horizontal tiles
  
  t1 = lv_tileview_add_tile(tileview, 1, 0, LV_DIR_HOR);
  t2 = lv_tileview_add_tile(tileview, 2, 0, LV_DIR_HOR);
  bootScreen = lv_tileview_add_tile(tileview, 0,0, LV_DIR_HOR);
  settingsScreen = lv_tileview_add_tile(tileview, 3, 0, LV_DIR_HOR);

  // BOOT SCREEN
  {
    create_bootscreen(tileview, bootScreen, VERSION);
  }

  // Tile #1
  {
    forecastDataCityLabel = draw_sunny_ui(t1);
  }

  // Tile #2
  {
    historicDataCityLabel = CreateHistoricDataScreen(tileview, t2);
  }

  //Settings Screen.
  {
    cityDropdown = create_settings_screen(tileview, settingsScreen, updateAllTilesBasedOnStation, updateHistoricDataBasedOnNewParam);
  }
}

// Function: Connects to WIFI
static void connect_wifi() {
  Serial.printf("Connecting to WiFi SSID: %s\n", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  const uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - start) < 15000) {
    delay(250);
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi connected.");
  } else {
    Serial.println("WiFi could not connect (timeout).");
  }
}

// Must have function: Setup is run once on startup
void setup() {
  Serial.begin(115200);
  delay(200);

  if (!amoled.begin()) {
    Serial.println("Failed to init LilyGO AMOLED.");
    while (true) delay(1000);
  }

  beginLvglHelper(amoled);   // init LVGL for this board

  create_ui();
  connect_wifi();

  if (WiFi.status() == WL_CONNECTED) {

    currStation = STATIONS[0];
    currParam = HISTORIC_TEMP;

    Error err;
    Serial.println("getting forcastData");
    forecastTemps = getForecastFromLongAndLat(KARLSKRONA_LONGITUDE, KARLSKRONA_LATITUDE, err);
    Serial.println("got forecastData");
    if (!err) {
      update_temperatures(forecastTemps, t1);
    } else {
      Serial.println(err.msg.c_str());
    }

    err = Error();    
    
    Serial.println("getting historic data");
    historicData = getHistoricDataFromId(KARLSKRONA_STATION_ID, HISTORIC_TEMP, err);
    Serial.println("got historic data");
    if (!err) {
      Serial.println(historicData[0].date.ymdhms().c_str());
      populateGraph(&historicData);
    } else {
      Serial.println(err.msg.c_str());
    }
    
    err = Error(); 
    // std::vector<StationData> stations = getAllStations(err);

    if (!err){
      err = Error();
      // populate_settings_screen_with_cities(cityDropdown, &stations, "Karlskrona Söderstjerna", updateAllTilesBasedOnStation, err);
    }

  } else {
    Serial.println("No wifi");
  }
}

// Must have function: Loop runs continously on device after setup
void loop() {
  lv_timer_handler();
  delay(5);
}
