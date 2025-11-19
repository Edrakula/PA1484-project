
#ifndef HISTORIC_DATA_SCREEN
#define HISTORIC_DATA_SCREEN

#include <Arduino.h>
#include <LV_Helper.h>
#include <LilyGo_AMOLED.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <vector>

#include "apiconnections.hpp"
#include "historic_data_screen_slider.hpp"

static lv_obj_t* chart;
static lv_chart_series_t* chartData;

void CreateHistoricDataScreen(lv_obj_t *TILE_VIEW, lv_obj_t *TILE) {
    
    chart = lv_chart_create(TILE);
    lv_obj_set_size(chart, 200, 150);
    lv_obj_center(chart);

    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_set_point_count(chart, 10);


    chartData = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);

    for (int i = 0; i < 10; i++) {
        chartData->y_points[i] = i * 10;   // simple ascending line
    }
    lv_chart_refresh(chart);
    

    create_slider(TILE);

}

void populateGraph(std::vector<HistoricData> data) {
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, data.size());
    
}

#endif