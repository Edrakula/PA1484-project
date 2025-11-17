#include "lvgl.h"

lv_obj_t* create_gradient_bg(lv_obj_t *parent)
{
    // Skapa bakgrunds-objekt
    lv_obj_t *bg = lv_obj_create(parent);
    lv_obj_set_size(bg, 450, 600);
    lv_obj_center(bg);

    // Ta bort border och padding
    lv_obj_remove_style_all(bg);

    // Skapa stil
    static lv_style_t style_bg;
    lv_style_init(&style_bg);

    // Gradient färger
    lv_style_set_bg_color(&style_bg, lv_color_hex(0x16B5FF));      // Start
    lv_style_set_bg_grad_color(&style_bg, lv_color_hex(0xC0F5FF)); // Slut

    // Vertikal gradient
    lv_style_set_bg_grad_dir(&style_bg, LV_GRAD_DIR_VER);

    // Applicera stil
    lv_obj_add_style(bg, &style_bg, 0);

    return bg;
}