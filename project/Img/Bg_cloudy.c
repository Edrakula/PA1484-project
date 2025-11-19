#include "lvgl.h"

void draw_cloudy_ui(lv_obj_t *tile)
{
    // ----- Background (gray sky) -----
    lv_obj_set_style_bg_color(tile, lv_color_hex(0xB0C4DE), 0); // light grayish blue
    lv_obj_set_style_bg_grad_color(tile, lv_color_hex(0xD3DCE6), 0);
    lv_obj_set_style_bg_grad_dir(tile, LV_GRAD_DIR_VER, 0);
    lv_obj_set_style_border_width(tile, 0, 0);

    // ----- Clouds -----
    lv_obj_t *cloud1 = lv_obj_create(tile);
    lv_obj_set_size(cloud1, 220, 90);
    lv_obj_set_style_radius(cloud1, 50, 0);
    lv_obj_set_style_bg_color(cloud1, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(cloud1, LV_ALIGN_TOP_LEFT, 40, 50);

    lv_obj_t *cloud2 = lv_obj_create(tile);
    lv_obj_set_size(cloud2, 180, 70);
    lv_obj_set_style_radius(cloud2, 50, 0);
    lv_obj_set_style_bg_color(cloud2, lv_color_hex(0xE0E0E0), 0);
    lv_obj_align(cloud2, LV_ALIGN_TOP_RIGHT, -40, 70);

    // ----- Temperature box -----
    lv_obj_t *temp_box = lv_obj_create(tile);
    lv_obj_set_size(temp_box, 150, 70);
    lv_obj_set_style_bg_color(temp_box, lv_color_hex(0x2F2F2F), 0);
    lv_obj_set_style_radius(temp_box, 18, 0);
    lv_obj_set_style_border_width(temp_box, 0, 0);
    lv_obj_set_style_pad_all(temp_box, 8, 0);
    lv_obj_align(temp_box, LV_ALIGN_LEFT_MID, 20, -20);

    // Temperature label
    lv_obj_t *temp_label = lv_label_create(temp_box);
    lv_label_set_text(temp_label, "11° C"); // placeholder
    lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_28, 0);
    lv_obj_center(temp_label);

    // ----- Condition text ("Cloudy") -----
    lv_obj_t *cond_label = lv_label_create(tile);
    lv_label_set_text(cond_label, "Cloudy");
    lv_obj_set_style_text_font(cond_label, &lv_font_montserrat_28, 0);
    lv_obj_align(cond_label, LV_ALIGN_LEFT_MID, 25, 30);

    // ----- City name -----
    lv_obj_t *city_label = lv_label_create(tile);
    lv_label_set_text(city_label, "Karlskrona");
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
        lv_obj_t *lbl = lv_label_create(card);
        lv_label_set_text(lbl, days[i]);
        lv_obj_align(lbl, LV_ALIGN_TOP_LEFT, x, 10);

        lv_obj_t *t = lv_label_create(card);
        lv_label_set_text(t, "11°"); // placeholder
        lv_obj_align(t, LV_ALIGN_TOP_LEFT, x, 40);

        x += 55;
    }
}
