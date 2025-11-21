#ifndef CLOUD_ICON
#define CLOUD_ICON

#include "lvgl.h"
#include "backend_logic/apiconnections.hpp"
#include <sstream>
#include <iomanip>
#include <string>

lv_obj_t* get_cloud_icon(lv_obj_t* parent, int x) {
    lv_color_t c = lv_color_hex(0xCFE2F3); // light cloud color

    lv_obj_t *cloud = lv_obj_create(parent);
    lv_obj_set_size(cloud, 20, 12);
    lv_obj_set_style_bg_color(cloud, c, 0);
    lv_obj_align(cloud, LV_ALIGN_TOP_LEFT, x, 62);

    // Left bump
    lv_obj_t *b1 = lv_obj_create(cloud);
    lv_obj_set_size(b1, 8, 8);
    lv_obj_set_style_radius(b1, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(b1, c, 0);
    lv_obj_align(b1, LV_ALIGN_BOTTOM_LEFT, 0, 0);

    // Middle bump (largest)
    lv_obj_t *b2 = lv_obj_create(cloud);
    lv_obj_set_size(b2, 10, 10);
    lv_obj_set_style_radius(b2, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(b2, c, 0);
    lv_obj_align(b2, LV_ALIGN_BOTTOM_MID, 0, -1);

    // Right bump
    lv_obj_t *b3 = lv_obj_create(cloud);
    lv_obj_set_size(b3, 7, 7);
    lv_obj_set_style_radius(b3, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(b3, c, 0);
    lv_obj_align(b3, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

    // Bottom flat part
    lv_obj_t *base = lv_obj_create(cloud);
    lv_obj_set_size(base, 14, 6);
    lv_obj_set_style_radius(base, 3, 0);
    lv_obj_set_style_bg_color(base, c, 0);
    lv_obj_align(base, LV_ALIGN_BOTTOM_MID, 0, 0);

    return cloud;
}




#endif


