#include "lvgl.h"
#include "apiconnections.hpp"
#include <sstream>
#include <iomanip>
#include <string>

lv_obj_t *temperatures[7];
lv_obj_t *temp_label;

void update_temperatures(std::vector<ForecastTemp> forecastTemps) {
    int y = 7;
    if (forecastTemps.size() < 7) {
        y = forecastTemps.size();
    }

    std::ostringstream oss0;
    oss0 << std::fixed << std::setprecision(1) << forecastTemps[0].temp;

    std::string s0 = oss0.str();

    lv_label_set_text(temp_label, (s0 + "°").c_str()); 

    for (int i = 0; i < y; i++) {
        lv_obj_t *t = temperatures[i];

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << forecastTemps[i].temp;

        std::string s = oss.str();

        lv_label_set_text(t, (s + "°").c_str());
    }
}

void draw_cloudy_ui(lv_obj_t *tile)
{
    lv_color_t text_color = lv_color_hex(0xFFFFFF);

    // ----- Background (light grey/blueish sky) -----
    lv_obj_set_style_bg_color(tile, lv_color_hex(0x5D6A92), 0);
    lv_obj_set_style_bg_grad_color(tile, lv_color_hex(0x3E3A59), 0);
    lv_obj_set_style_bg_grad_dir(tile, LV_GRAD_DIR_VER, 0);
    lv_obj_set_style_border_width(tile, 0, 0);

    // ----- Cloud shape (multi-part) -----
    lv_obj_t *cloud_base = lv_obj_create(tile);
    lv_obj_set_size(cloud_base, 260, 70);
    lv_obj_set_style_bg_color(cloud_base, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_radius(cloud_base, 35, 0);
    lv_obj_set_style_border_width(cloud_base, 0, 0);
    lv_obj_align(cloud_base, LV_ALIGN_TOP_MID, 0, 70);

    lv_obj_t *cloud_left = lv_obj_create(tile);
    lv_obj_set_size(cloud_left, 120, 70);
    lv_obj_set_style_bg_color(cloud_left, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_radius(cloud_left, 35, 0);
    lv_obj_set_style_border_width(cloud_left, 0, 0);
    lv_obj_align_to(cloud_left, cloud_base, LV_ALIGN_OUT_LEFT_MID, 40, -15);

    lv_obj_t *cloud_right = lv_obj_create(tile);
    lv_obj_set_size(cloud_right, 120, 70);
    lv_obj_set_style_bg_color(cloud_right, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_radius(cloud_right, 35, 0);
    lv_obj_set_style_border_width(cloud_right, 0, 0);
    lv_obj_align_to(cloud_right, cloud_base, LV_ALIGN_OUT_RIGHT_MID, -40, -15);

    lv_obj_move_background(cloud_base);
    lv_obj_move_background(cloud_left);
    lv_obj_move_background(cloud_right);

    // ----- Temperature box -----
    lv_obj_t *temp_box = lv_obj_create(tile);
    lv_obj_set_size(temp_box, 150, 70);
    lv_obj_set_style_bg_color(temp_box, lv_color_hex(0x2F2F2F), 0);
    lv_obj_set_style_radius(temp_box, 18, 0);
    lv_obj_set_style_border_width(temp_box, 0, 0);
    lv_obj_set_style_pad_all(temp_box, 8, 0);
    lv_obj_align(temp_box, LV_ALIGN_LEFT_MID, 20, -20);

    // Temperature label – use GLOBAL temp_label
    temp_label = lv_label_create(temp_box);
    lv_label_set_text(temp_label, "11° C");              // placeholder
    lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_28, 0);
    lv_obj_set_style_text_color(temp_label, text_color, 0);
    lv_obj_center(temp_label);

    // ----- Condition text ("Cloudy") -----
    lv_obj_t *cond_label = lv_label_create(tile);
    lv_label_set_text(cond_label, "Windy");
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

        // small temperature – use GLOBAL temperatures[]
        temperatures[i] = lv_label_create(card);
        lv_obj_t *t = temperatures[i];
        lv_label_set_text(t, "11°");
        lv_obj_set_style_text_color(t, text_color, 0);
        lv_obj_align(t, LV_ALIGN_TOP_LEFT, x, 40);

        x += 55;
    }
}
void update_temperatures(std::vector<ForecastTemp> forecastTemps) {
    int y = 7;
    if (forecastTemps.size() < 7) {
        y = forecastTemps.size();
    }

    std::ostringstream oss0;
    oss0 << std::fixed << std::setprecision(1) << forecastTemps[0].temp;

    std::string s0 = oss0.str();

    lv_label_set_text(temp_label, (s0 + "°").c_str()); 

    for (int i = 0; i < 7; i++) {
        lv_obj_t *t = temperatures[i];


        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << forecastTemps[i].temp;

        std::string s = oss.str();

        lv_label_set_text(t, (s + "°").c_str());
    }

}