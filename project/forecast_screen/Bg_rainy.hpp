#ifndef RAINY_BG
#define RAINY_BG

#include "lvgl.h"
#include "backend_logic/apiconnections.hpp"
#include <sstream>
#include <iomanip>
#include <string>

#include "forecast_screen/raindrop_helper.hpp"


lv_obj_t* draw_rainy_ui(lv_obj_t *tile, lv_obj_t* condLabel) {

    lv_obj_t *cont = lv_obj_create(tile);
    lv_obj_remove_style_all(cont);          // No borders, no bg, no padding, no shadow

    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));  // Fill 100% of parent

    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);  // Optional: remove scrollbars
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 0);

    lv_obj_move_background(cont);

    lv_color_t text_color = lv_color_hex(0xFFFFFF);

    // ----- Cloud (same shape style as sleet) -----
    lv_obj_t *cloud = lv_obj_create(cont);
    lv_obj_set_size(cloud, 480, 40);
    lv_obj_set_style_bg_color(cloud, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_radius(cloud, 60, 0);
    lv_obj_align(cloud, LV_ALIGN_TOP_MID, 0, 60);

    lv_obj_t *cloud2 = lv_obj_create(cont);
    lv_obj_set_size(cloud2, 300, 40);
    lv_obj_set_style_bg_color(cloud2, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_radius(cloud2, 40, 0);
    lv_obj_align(cloud2, LV_ALIGN_TOP_MID, 20, 40);

    lv_obj_move_background(cloud);
    lv_obj_move_background(cloud2);

    // ----- Raindrops -----
    int start_y = 130;
    int rows    = 6;
    int cols    = 7;
    int dx      = 70;
    int dy      = 45;
    int start_x = 30;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int rx = start_x + c * dx + ((r & 1) ? 30 : 0);
            int ry = start_y + r * dy;
            create_raindrop(cont, rx, ry);
        }
    }

    lv_label_set_text(condLabel, "Rainy");

    return cont;
}

#endif