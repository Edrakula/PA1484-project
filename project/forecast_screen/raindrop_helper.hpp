#ifndef RAINDROP_HELPER
#define RAINDROP_HELPER

#include "lvgl.h"

// Helper: create one blue raindrop
static void create_raindrop(lv_obj_t *parent, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *drop = lv_obj_create(parent);

    lv_obj_set_size(drop, 10, 18);
    lv_obj_set_style_radius(drop, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(drop, lv_color_hex(0x008CFF), 0);
    lv_obj_set_style_border_width(drop, 0, 0);

    // Position relative to middle like sleet
    lv_obj_align(drop, LV_ALIGN_TOP_MID, -240 + x, y);

    // Send behind UI
    lv_obj_move_background(drop);
}

#endif