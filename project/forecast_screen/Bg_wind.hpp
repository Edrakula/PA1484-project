#ifndef WINDY_BG
#define WINDY_BG


#include "lvgl.h"
#include "backend_logic/apiconnections.hpp"
#include <sstream>
#include <iomanip>
#include <string>


lv_obj_t* create_windy_bg(lv_obj_t *parent, lv_obj_t* condLabel) {

    lv_obj_t *cont = lv_obj_create(parent);
    lv_obj_remove_style_all(cont);          // No borders, no bg, no padding, no shadow

    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));  // Fill 100% of parent

    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);  // Optional: remove scrollbars
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 0);

    lv_obj_move_background(cont);

    lv_label_set_text(condLabel, "Windy");

    // Helper to create a horizontal "line" using a container
    auto make_wind_bar = [&](int y, int width) {
        lv_obj_t *bar = lv_obj_create(cont);
        lv_obj_remove_style_all(bar);

        lv_obj_set_size(bar, width, 20);

        // --- Transparent background ---
        lv_obj_set_style_bg_opa(bar, LV_OPA_TRANSP, 0);

        // --- Border settings ---
        lv_obj_set_style_border_color(bar, lv_color_hex(0x000000), 0); // border color
        lv_obj_set_style_border_width(bar, 3, 0);
        lv_obj_set_style_border_opa(bar, LV_OPA_COVER, 0);

        // Only show TOP + RIGHT borders
        lv_obj_set_style_border_side(
            bar,
            LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT,
            0
        );

        // Rounded corners (still works)
        lv_obj_set_style_radius(bar, LV_RADIUS_CIRCLE, 0);

        lv_obj_align(bar, LV_ALIGN_TOP_MID, 0, y);

        return bar;
    };

    // Three centered bars
    make_wind_bar(150, 180);   // Line 1
    make_wind_bar(190, 220);   // Line 2
    make_wind_bar(230, 170);   // Line 3

    return cont;
}



#endif