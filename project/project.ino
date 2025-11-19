#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <time.h>
#include <LilyGo_AMOLED.h>
#include <LV_Helper.h>
#include <lvgl.h>
#include "apiconnections.hpp"
#include "boot_screen.hpp"
#include "historic_data_screen.hpp"
#include "Img/Bg_sleet.hpp"

static const String VERSION = "1.0";

// Wi-Fi credentials (Delete these before commiting to GitHub)
static const char* WIFI_SSID     = "SSID";
static const char* WIFI_PASSWORD = "PASSWORD";

LilyGo_Class amoled;

static lv_obj_t* tileview;
static lv_obj_t* t1;
static lv_obj_t* t2;

static lv_obj_t* bootScreen;


static lv_obj_t* t1_label;
static lv_obj_t* t2_label;
static bool t2_dark = false;  // start tile #2 in light mode

// Function: Tile #2 Color change
static void apply_tile_colors(lv_obj_t* tile, lv_obj_t* label, bool dark) {
  // Background
  lv_obj_set_style_bg_opa(tile, LV_OPA_COVER, 0);
  lv_obj_set_style_bg_color(tile, dark ? lv_color_black() : lv_color_white(), 0);

  // Text
  lv_obj_set_style_text_color(label, dark ? lv_color_white() : lv_color_black(), 0);
}

static void on_tile2_clicked(lv_event_t* e) {
  LV_UNUSED(e);
  t2_dark = !t2_dark;
  apply_tile_colors(t2, t2_label, t2_dark);
}

// Function: Creates UI
static void create_ui() {
  // Fullscreen Tileview
  tileview = lv_tileview_create(lv_scr_act());
  lv_obj_set_size(tileview, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
  lv_obj_set_scrollbar_mode(tileview, LV_SCROLLBAR_MODE_OFF);

  // Add two horizontal tiles
  
  t1 = lv_tileview_add_tile(tileview, 1, 0, LV_DIR_HOR);
  t2 = lv_tileview_add_tile(tileview, 2, 0, LV_DIR_HOR);
  bootScreen = lv_tileview_add_tile(tileview, 0,0, LV_DIR_HOR);

  // BOOT SCREEN
  {
    create_bootscreen(tileview, bootScreen, VERSION);
  }

  // Tile #1
  {
    draw_sleet_ui(t1);
  }

  // Tile #2
  {
    CreateHistoricDataScreen(tileview, t2);
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

std::vector<ForecastTemp> forecastTemps;
std::vector<HistoricData> historicData;

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
    Error err;
    //forecastTemps = getForecastFromLongAndLat(LONGITUDE, LATITUDE, err);
    
    if (!err) {
      //update_temperatures(forecastTemps);
    } else {
      //lv_label_set_text(t1_label, err.msg.c_str());
      //lv_obj_set_size(t1_label,lv_disp_get_hor_res(NULL),lv_disp_get_ver_res(NULL));
    }

    err = Error();    
    /*
    historicData = getHistoricDataFromId(STATION_ID, HISTORIC_TEMP, err);
    if (!err) {
      lv_label_set_text(t2_label, (historicData[0].date.ymdhms() + " : " + historicData[0].data + " " + historicData[0].unit).c_str());
    } else {
      lv_label_set_text(t2_label, err.msg.c_str());
    }
    */
  } else {

    
  }
}

// Must have function: Loop runs continously on device after setup
void loop() {
  lv_timer_handler();
  delay(5);
}