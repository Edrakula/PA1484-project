#include "lvgl.h"

void draw_weather_ui() {
    lv_obj_t *scr = lv_scr_act();

    // Background gradient
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x88CFFF), 0);
    lv_obj_set_style_bg_grad_color(scr, lv_color_hex(0xC9EFFF), 0);
    lv_obj_set_style_bg_grad_dir(scr, LV_GRAD_DIR_VER, 0);

    // --- Sun ---
    lv_obj_t *sun = lv_obj_create(scr);
    lv_obj_set_size(sun, 130, 130);
    lv_obj_set_style_radius(sun, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(sun, lv_color_hex(0xFFD726), 0);
    lv_obj_set_style_border_width(sun, 0, 0);
    lv_obj_align(sun, LV_ALIGN_TOP_RIGHT, -40, 40);

    // --- Cloud ---
    lv_obj_t *cloud = lv_obj_create(scr);
    lv_obj_set_size(cloud, 220, 90);
    lv_obj_set_style_radius(cloud, 50, 0);
    lv_obj_set_style_bg_color(cloud, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_border_width(cloud, 0, 0);
    lv_obj_align(cloud, LV_ALIGN_TOP_LEFT, 20, 80);

    // --- Temperature ---
    lv_obj_t *temp_label = lv_label_create(scr);
    lv_label_set_text(temp_label, "11° C");
    lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_40, 0);
    lv_obj_align(temp_label, LV_ALIGN_LEFT_MID, 20, -40);

    // --- Description ---
    lv_obj_t *desc_label = lv_label_create(scr);
    lv_label_set_text(desc_label, "Soligt");
    lv_obj_set_style_text_font(desc_label, &lv_font_montserrat_28, 0);
    lv_obj_align(desc_label, LV_ALIGN_LEFT_MID, 20, 20);

    // --- City ---
    lv_obj_t *city_label = lv_label_create(scr);
    lv_label_set_text(city_label, "Karlskrona");
    lv_obj_align(city_label, LV_ALIGN_TOP_MID, 0, 10);

    // --- Weekly forecast card ---
    lv_obj_t *card = lv_obj_create(scr);
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
        lv_label_set_text(t, "11°");
        lv_obj_align(t, LV_ALIGN_TOP_LEFT, x, 40);

        x += 55;
    }
}
