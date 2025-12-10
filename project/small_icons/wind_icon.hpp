#ifndef WIND_ICON
#define WIND_ICON

#include "lvgl.h"
#include "backend_logic/apiconnections.hpp"
#include <sstream>
#include <iomanip>
#include <string>


lv_obj_t* get_wind_icon(lv_obj_t* parent, int x) {

    /* --- Container --- */
    lv_obj_t *cont = lv_obj_create(parent);
    lv_obj_remove_style_all(cont);           // LVGL 8.4 safe
    lv_obj_set_size(cont, 30, 30);           // enough to hold both drops
    lv_obj_set_pos(cont, x, 62);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);


    /* --- First drop --- */
    lv_obj_t *flake1 = lv_obj_create(cont);
    lv_obj_remove_style_all(flake1);         // clear all styles
    lv_obj_set_size(flake1, 16, 8);
    lv_obj_set_style_radius(flake1, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_opa(flake1, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(flake1, 2, 0);
    lv_obj_set_style_border_color(flake1, lv_color_hex(0x000000), 0);
    lv_obj_set_style_border_opa(flake1, LV_OPA_COVER, 0);

    // Only show TOP + RIGHT borders
    lv_obj_set_style_border_side(
        flake1,
        LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT,
        0
    );
    lv_obj_set_pos(flake1, 0, 0);


    /* --- Second drop --- */
    lv_obj_t *flake2 = lv_obj_create(cont);
    lv_obj_remove_style_all(flake2);         // clear all styles
    lv_obj_set_size(flake2, 16, 8);
    lv_obj_set_style_radius(flake2, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_opa(flake2, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(flake2, 2, 0);
    lv_obj_set_style_border_color(flake2, lv_color_hex(0x000000), 0);
    lv_obj_set_style_border_opa(flake2, LV_OPA_COVER, 0);

    // Only show TOP + RIGHT borders
    lv_obj_set_style_border_side(
        flake2,
        LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_RIGHT,
        0
    );
    lv_obj_set_pos(flake2, 0, 10);

    return cont;
}



#endif


