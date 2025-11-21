#ifndef RAIN_ICON
#define RAIN_ICON

#include "lvgl.h"
#include "backend_logic/apiconnections.hpp"
#include <sstream>
#include <iomanip>
#include <string>


lv_obj_t* get_rain_icon(lv_obj_t* parent, int x) {
    lv_obj_t *flake = lv_obj_create(parent);
    lv_obj_set_size(flake, 8, 8);
    lv_obj_set_style_radius(flake, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(flake, lv_color_hex(0x0000FF), 0);
    lv_obj_set_style_border_width(flake, 2, 0);
    lv_obj_set_style_border_color(flake, lv_color_hex(0xC0E0FF), 0);
    lv_obj_set_style_border_opa(flake, LV_OPA_COVER, 0);

    lv_obj_align(flake, LV_ALIGN_TOP_LEFT, x, 62);


    lv_obj_t *flake2 = lv_obj_create(parent);
    lv_obj_set_size(flake2, 8, 8);
    lv_obj_set_style_radius(flake2, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(flake2, lv_color_hex(0x0000FF), 0);
    lv_obj_set_style_border_width(flake2, 2, 0);
    lv_obj_set_style_border_color(flake2, lv_color_hex(0xC0E0FF), 0);
    lv_obj_set_style_border_opa(flake2, LV_OPA_COVER, 0);

    lv_obj_align(flake2, LV_ALIGN_TOP_LEFT, x + 10, 62 + 10);

    return flake;
}


#endif


