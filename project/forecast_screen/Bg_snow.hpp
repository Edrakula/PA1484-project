#ifndef SNOW_BG
#define SNOW_BG

#include "lvgl.h"
#include "backend_logic/apiconnections.hpp"
#include <sstream>
#include <iomanip>
#include <string>



// --- Helper for snowflake ---
static void create_snowflake(lv_obj_t *parent, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *flake = lv_obj_create(parent);
    lv_obj_set_size(flake, 8, 8);
    lv_obj_set_style_radius(flake, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(flake, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_border_width(flake, 1, 0);
    lv_obj_set_style_border_color(flake, lv_color_hex(0x000000), 0);
    lv_obj_set_style_border_opa(flake, LV_OPA_COVER, 0);

    // Position relative to screen center
    lv_obj_align(flake, LV_ALIGN_TOP_MID, -240 + x, y);

    // Draw behind labels/boxes/cards
    lv_obj_move_background(flake);
}

lv_obj_t* draw_snow_ui(lv_obj_t *tile, lv_obj_t *condLabel) {

    
    lv_obj_t *cont = lv_obj_create(tile);
    lv_obj_remove_style_all(cont);          // No borders, no bg, no padding, no shadow

    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));  // Fill 100% of parent

    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);  // Optional: remove scrollbars
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 0);
    lv_obj_move_background(cont);
    // ----- Cloud (same style idea as your sleet) -----
    lv_obj_t *cloud = lv_obj_create(cont);
    lv_obj_set_size(cloud, 480, 40);
    lv_obj_set_style_bg_color(cloud, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_radius(cloud, 60, 0);
    lv_obj_align(cloud, LV_ALIGN_TOP_MID, 0, 60);
    lv_obj_set_style_border_color(cloud, lv_color_hex(0xFFFFFF), 0);

    lv_obj_t *cloud2 = lv_obj_create(cont);
    lv_obj_set_size(cloud2, 300, 40);
    lv_obj_set_style_bg_color(cloud2, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_radius(cloud2, 40, 0);
    lv_obj_align(cloud2, LV_ALIGN_TOP_MID, 20, 40);
    lv_obj_set_style_border_color(cloud2, lv_color_hex(0xFFFFFF), 0);

    // ----- Condition -----
    
    lv_label_set_text(condLabel, "Snow");


    // ----- Snowflakes -----
    int start_y = 130;
    int rows    = 6;
    int cols    = 7;
    int dx      = 70;
    int dy      = 45;
    int start_x = 30;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int sx = start_x + c * dx + ((r & 1) ? 30 : 0);
            int sy = start_y + r * dy;
            create_snowflake(cont, sx, sy);
        }
    }

    return cont;
}


#endif