#ifndef SUNNY_BG
#define SUNNY_BG

#include "lvgl.h"
#include "backend_logic/apiconnections.hpp"
#include <sstream>
#include <iomanip>
#include <string>
#include "small_icons/sunny_icon.hpp"
#include "small_icons/snow_icon.hpp"
#include "small_icons/cloud_icon.hpp"
#include "small_icons/rain_icon.hpp"
#include <algorithm>

lv_obj_t* draw_sunny_ui(lv_obj_t *tile, lv_obj_t *condLabel) {
    
    // ----- Sun -----
    lv_obj_t *sun = lv_obj_create(tile);
    lv_obj_set_size(sun, 130, 130);
    lv_obj_set_style_radius(sun, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(sun, lv_color_hex(0xFFD726), 0);
    lv_obj_align(sun, LV_ALIGN_TOP_RIGHT, -40, 40);

    lv_label_set_text(condLabel, "Sunny");

    return sun;
}

#endif