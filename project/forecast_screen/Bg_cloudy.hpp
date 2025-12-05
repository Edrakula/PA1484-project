#ifndef CLOUD_BG
#define CLOUD_BG

#include "lvgl.h"
#include "backend_logic/apiconnections.hpp"
#include <sstream>
#include <iomanip>
#include <string>


lv_obj_t* draw_cloudy_ui(lv_obj_t *tile, lv_obj_t *condLabel) {

    lv_obj_t *cloud = lv_obj_create(tile);
    lv_obj_set_size(cloud, 480, 40);
    lv_obj_set_style_bg_color(cloud, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_radius(cloud, 60, 0);
    lv_obj_align(cloud, LV_ALIGN_TOP_MID, 0, 60);
    lv_obj_set_style_border_color(cloud,lv_color_hex(0xFFFFFF),0);

    lv_obj_t *cloud2 = lv_obj_create(tile);
    lv_obj_set_size(cloud2, 300, 40);
    lv_obj_set_style_bg_color(cloud2, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_radius(cloud2, 40, 0);
    lv_obj_align(cloud2, LV_ALIGN_TOP_MID, 20, 40);
    lv_obj_set_style_border_color(cloud2,lv_color_hex(0xFFFFFF),0);

    lv_label_set_text(condLabel, "Cloudy");

    return cloud;
}

#endif