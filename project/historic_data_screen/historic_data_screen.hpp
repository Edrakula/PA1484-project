
#ifndef HISTORIC_DATA_SCREEN
#define HISTORIC_DATA_SCREEN

#include <Arduino.h>
#include <LV_Helper.h>
#include <LilyGo_AMOLED.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <vector>

#include "backend_logic/apiconnections.hpp"
#include "historic_data_screen/historic_data_screen_slider.hpp"

std::pair<int,int> get_xy_dist_from_center_of_chart_point(int x_index, int y_value, int y_max);

static const int CHART_WIDTH = 400;
static const int CHART_HEIGHT = 250;
static const int CHART_PADDING = 10;

lv_obj_t* city_label_history;
lv_obj_t* data_type_label;

static lv_obj_t* chart;
static lv_chart_series_t* chartData;

static lv_obj_t* historic_slider;
static lv_obj_t* slider_data_label;

static lv_obj_t* min_date_label;
static lv_obj_t* mid_date_label;
static lv_obj_t* max_date_label;

static lv_obj_t* red_chart_dot;

static void chart_event_cb(lv_event_t * e)
{
    lv_obj_draw_part_dsc_t * dsc =
        (lv_obj_draw_part_dsc_t *)lv_event_get_param(e);

    /* Only process tick labels */
    if(dsc->part == LV_PART_TICKS &&
       dsc->id == LV_CHART_AXIS_PRIMARY_Y) {

        /* Set the Y-axis tick text */
        lv_snprintf(dsc->text, dsc->text_length, "%d", dsc->value);
    }
}



void CreateHistoricDataScreen(lv_obj_t *TILE_VIEW, lv_obj_t *TILE) {
    
    // ----- City name -----
    city_label_history = lv_label_create(TILE);
    lv_label_set_text(city_label_history, "Karlskrona");
    lv_obj_set_style_text_color(city_label_history, lv_color_hex(0x0000000), 0);
    lv_obj_align(city_label_history, LV_ALIGN_TOP_MID, 0, 10);

    // latest months label
    lv_obj_t* latestMonthsLabel = lv_label_create(TILE);
    lv_label_set_text(latestMonthsLabel, "Latest months data");
    lv_obj_set_style_text_color(latestMonthsLabel, lv_color_hex(0x0000000), 0);
    lv_obj_align(latestMonthsLabel, LV_ALIGN_TOP_MID, 0, 50);

    // what data is displayed label
    data_type_label = lv_label_create(TILE);
    lv_label_set_text(data_type_label, "Temperature, celsius");
    lv_obj_set_style_text_color(data_type_label, lv_color_hex(0x0000000), 0);
    lv_obj_align(data_type_label, LV_ALIGN_TOP_MID, 0, 70);


    chart = lv_chart_create(TILE);
    lv_obj_set_size(chart, CHART_WIDTH, CHART_HEIGHT);
    lv_obj_center(chart);
    lv_obj_set_style_pad_all(chart, CHART_PADDING, 0);

    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, -30, 30);
    lv_chart_set_point_count(chart, 10);


    chartData = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);

    for (int i = 0; i < 10; i++) {
        chartData->y_points[i] = i * 10;   // simple ascending line
    }


    lv_obj_add_event_cb(chart, chart_event_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);

    lv_chart_set_axis_tick(chart,
        LV_CHART_AXIS_PRIMARY_Y,  /* y-axis */
        10,   /* major tick length */
        5,    /* minor tick length */
        13,    /* number of major ticks */
        5,    /* number of minor ticks between majors */
        true, /* enable label for major ticks */
        40    /* extra space for drawing labels */
    );
        
    lv_chart_set_div_line_count(chart, 7, 0);

    lv_chart_refresh(chart);

    slider_data_label = lv_label_create(TILE);
    lv_label_set_text(slider_data_label, "empty");
    lv_obj_set_style_text_color(slider_data_label, lv_color_hex(0x0000000), 0);
    lv_obj_align(slider_data_label, LV_ALIGN_BOTTOM_MID, 0, -15-20);


    // red dot for selected point
    red_chart_dot = lv_obj_create(chart);
    lv_obj_set_size(red_chart_dot, 11, 11);
    lv_obj_set_style_radius(red_chart_dot, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(red_chart_dot, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_set_style_border_width(red_chart_dot, 0, 0);

    // historic slider
    historic_slider = create_slider_that_updates_label_based_on_historic_data(TILE, 10, nullptr, slider_data_label, red_chart_dot, 10);

    // date labels
    min_date_label = lv_label_create(TILE);
    lv_label_set_text(min_date_label, "left");
    lv_obj_set_style_text_color(min_date_label, lv_color_hex(0x0000000), 0);
    lv_obj_center(min_date_label);
    lv_obj_align(min_date_label, LV_ALIGN_CENTER, - (CHART_WIDTH/2 + CHART_PADDING), CHART_HEIGHT/2 + CHART_PADDING + 10);

    mid_date_label = lv_label_create(TILE);
    lv_label_set_text(mid_date_label, "mid");
    lv_obj_set_style_text_color(mid_date_label, lv_color_hex(0x0000000), 0);
    lv_obj_center(mid_date_label);
    lv_obj_align(mid_date_label, LV_ALIGN_CENTER, 0, CHART_HEIGHT/2 + CHART_PADDING + 10);

    max_date_label = lv_label_create(TILE);
    lv_label_set_text(max_date_label, "right");
    lv_obj_set_style_text_color(max_date_label, lv_color_hex(0x0000000), 0);
    lv_obj_center(max_date_label);
    lv_obj_align(max_date_label, LV_ALIGN_CENTER, (CHART_WIDTH/2 + CHART_PADDING), CHART_HEIGHT/2 + CHART_PADDING + 10);

}

std::pair<int,int> get_xy_dist_from_center_of_chart_point(int x_index, int y_value, int y_max) {

    lv_coord_t left = -CHART_WIDTH/2;
    lv_coord_t top = -CHART_HEIGHT/2;

    int point_count = lv_chart_get_point_count(chart);
    int y_dist = abs(y_value - y_max);

    // Map x and y to pixels
    int px = left + x_index * ((float)(CHART_WIDTH-(CHART_PADDING*2)) / (float)(point_count-1));
    int py = top + y_dist * ((float)(CHART_HEIGHT-(CHART_PADDING*2)) / (float)(y_max*2));

    return {px + CHART_PADDING,py + CHART_PADDING};
}

void populateGraph(std::vector<HistoricData>* data) {
    size_t dataSize = data->size();
    if (dataSize == 0) return;

    // -------------- set current data label copied from slider func --------------
    HistoricData cur_data_point = data->at(0);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << cur_data_point.data;
    std::string s = oss.str();
    lv_label_set_text(slider_data_label, (cur_data_point.date.ymd() + " : " + s + " " + cur_data_point.unit).c_str());


    // -------------- adjust graph date under graph --------------
    lv_label_set_text(min_date_label, data->at(0).date.ymd().c_str());
    lv_label_set_text(mid_date_label, data->at(dataSize/2).date.ymd().c_str());
    lv_label_set_text(max_date_label, data->at(dataSize-1).date.ymd().c_str());

    // -------------- change graph --------------
    lv_chart_set_point_count(chart, dataSize);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0, dataSize);

    // get abs(max value of data) for height of graph
    float maxData = 0;

    for(size_t i = 0; i < dataSize; i++) {
        chartData->y_points[i] = data->at(i).data; // add the points
        float absData = abs(data->at(i).data);
        maxData = maxData < absData ? absData : maxData;
    }

    // round to nearest higher multiple of 5 for better formating of y-axel
    int maxDataCeil5 = 5 * std::ceil(maxData / 5.0);

    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, -maxDataCeil5, maxDataCeil5);

    lv_chart_set_axis_tick(chart,
        LV_CHART_AXIS_PRIMARY_Y,
        10, // big ticks 10px wide
        5, // small ticks 5 px wide
        (maxDataCeil5 * 2) / 5 + 1, // formula (2x/5 + 1) to always have steps of 5 and 1 step on 0
        5, // 5 small ticks between big ticks
        true, // use lavels
        40  // extra space for drawing labels
    );

    lv_chart_set_div_line_count(chart, maxDataCeil5 / 5 + 1, 0); // horizontal lines every 10 steps

    // -------------- highlight current point --------------
    std::pair<int,int> cords = get_xy_dist_from_center_of_chart_point(0, data->at(0).data, maxDataCeil5);

    lv_obj_center(red_chart_dot);
    lv_obj_align(red_chart_dot, LV_ALIGN_CENTER, cords.first, cords.second);

    lv_chart_refresh(chart);

    // -------------- update slider --------------
    slider_update_slider_range_and_info(historic_slider ,dataSize, slider_data_label, data, red_chart_dot, maxDataCeil5);
}

#endif