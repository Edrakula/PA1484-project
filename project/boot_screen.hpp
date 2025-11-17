#ifndef BOOT_SCREEN
#define BOOT_SCREEN
#include <Arduino.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <LilyGo_AMOLED.h>
#include <LV_Helper.h>
#include <lvgl.h>
static lv_obj_t* bootScreenLabel1;

void create_bootscreen(lv_obj_t* tile_view, lv_obj_t* tile, String version){

    String version_label = "Version: " + version + "\n Group 10";
    bootScreenLabel1 = lv_label_create(tile);
    lv_label_set_text(bootScreenLabel1, version_label.c_str());
    lv_obj_set_style_text_font(bootScreenLabel1, &lv_font_montserrat_40, LV_PART_MAIN);
    lv_obj_align(bootScreenLabel1, LV_ALIGN_CENTER, 0, 0);
}

#endif 