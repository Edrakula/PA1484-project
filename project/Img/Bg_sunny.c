#include "lvgl.h"

void draw_sunny_ui(lv_obj_t *tile)
{
    // common text color
    lv_color_t text_color = lv_color_hex(0xFFFFFF);

    // ----- Background (light blue sky) -----
    lv_obj_set_style_bg_color(tile, lv_color_hex(0x88CFFF), 0);
    lv_obj_set_style_bg_grad_color(tile, lv_color_hex(0xC9EFFF), 0);
    lv_obj_set_style_bg_grad_dir(tile, LV_GRAD_DIR_VER, 0);
    lv_obj_set_style_border_width(tile, 0, 0);

    // ----- Sun -----
    lv_obj_t *sun = lv_obj_create(tile);
    lv_obj_set_size(sun, 130, 130);
    lv_obj_set_style_radius(sun, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(sun, lv_color_hex(0xFFD726), 0);
    lv_obj_align(sun, LV_ALIGN_TOP_RIGHT, -40, 40);


    // ----- Temperature box -----
    lv_obj_t *temp_box = lv_obj_create(tile);
    lv_obj_set_size(temp_box, 150, 70);
    lv_obj_set_style_bg_color(temp_box, lv_color_hex(0x2F2F2F), 0);
    lv_obj_set_style_radius(temp_box, 18, 0);
    lv_obj_set_style_border_width(temp_box, 0, 0);
    lv_obj_set_style_pad_all(temp_box, 8, 0);
    lv_obj_align(temp_box, LV_ALIGN_LEFT_MID, 20, -20);

    // Temperature label inside the box
    lv_obj_t *temp_label = lv_label_create(temp_box);
    lv_label_set_text(temp_label, "11° C");              // placeholder
    lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_28, 0);
    lv_obj_set_style_text_color(temp_label, text_color, 0);
    lv_obj_center(temp_label);

    // ----- Condition text ("Sunny") -----
    lv_obj_t *cond_label = lv_label_create(tile);
    lv_label_set_text(cond_label, "Sunny");
    lv_obj_set_style_text_font(cond_label, &lv_font_montserrat_28, 0);
    lv_obj_set_style_text_color(cond_label, lv_color_hex(0x000000), 0);
    lv_obj_align(cond_label, LV_ALIGN_LEFT_MID, 25, 30);

    // ----- City name -----
    lv_obj_t *city_label = lv_label_create(tile);
    lv_label_set_text(city_label, "Karlskrona");
    lv_obj_set_style_text_color(city_label, lv_color_hex(0x000000), 0);
    lv_obj_align(city_label, LV_ALIGN_TOP_MID, 0, 10);

    // ----- Weekly forecast card -----
    lv_obj_t *card = lv_obj_create(tile);
    lv_obj_set_size(card, 420, 120);
    lv_obj_set_style_bg_color(card, lv_color_hex(0x3A3A3A), 0);
    lv_obj_set_style_radius(card, 25, 0);
    lv_obj_set_style_border_width(card, 0, 0);
    lv_obj_align(card, LV_ALIGN_BOTTOM_MID, 0, -40);

    const char *days[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    int x = 20;

    for (int i = 0; i < 7; i++) {
        // day name
        lv_obj_t *lbl = lv_label_create(card);
        lv_label_set_text(lbl, days[i]);
        lv_obj_set_style_text_color(lbl, text_color, 0);
        lv_obj_align(lbl, LV_ALIGN_TOP_LEFT, x, 10);

        // small temperature
        lv_obj_t *t = lv_label_create(card);
        lv_label_set_text(t, "11°");        // placeholder for day temp
        lv_obj_set_style_text_color(t, text_color, 0);
        lv_obj_align(t, LV_ALIGN_TOP_LEFT, x, 40);

        x += 55;
    }
}
