#ifndef SUNNY_ICON
#define SUNNY_ICON

#include "lvgl.h"
#include "backend_logic/apiconnections.hpp"
#include <sstream>
#include <iomanip>
#include <string>


lv_obj_t* get_sunny_icon(lv_obj_t* parent, int x) {
    lv_obj_t *sun = lv_obj_create(parent);
    lv_obj_set_size(sun, 20, 20);
    lv_obj_set_style_radius(sun, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(sun, lv_color_hex(0xFFD726), 0);

    lv_obj_align(sun, LV_ALIGN_TOP_LEFT, x, 62);

    return sun;
}


#endif