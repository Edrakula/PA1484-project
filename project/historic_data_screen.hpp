
#ifndef HISTORIC_DATA_SCREEN
#define HISTORIC_DATA_SCREEN

#include <Arduino.h>
#include <LV_Helper.h>
#include <LilyGo_AMOLED.h>
#include <TFT_eSPI.h>
#include <lvgl.h>

#include "apiconnections.hpp"

static lv_obj_t* chart;

void CreateHistoricDataScreen(const lv_obj_t *TILE_VIEW, const lv_obj_t *TILE) {
    //chart = lv_chart_create();
    lv_obj_set_size(chart, 200, 150);
    lv_obj_center(chart);

}

#endif