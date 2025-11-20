#include "lvgl.h"
#include "apiconnections.hpp"
#include <sstream>
#include <iomanip>
#include <string>

lv_obj_t *temperatures[7];
lv_obj_t *temp_label;

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

void draw_rainy_ui(lv_obj_t *tile)
{
    lv_color_t text_color = lv_color_hex(0xFFFFFF);

    // ----- Background -----
    lv_obj_set_style_bg_color(tile, lv_color_hex(0x3F4E5A), 0);
    lv_obj_set_style_bg_grad_color(tile, lv_color_hex(0xD8F2FF), 0);
    lv_obj_set_style_bg_grad_dir(tile, LV_GRAD_DIR_VER, 0);

    // ----- Cloud (same shape style as sleet) -----
    lv_obj_t *cloud = lv_obj_create(tile);
    lv_obj_set_size(cloud, 480, 40);
    lv_obj_set_style_bg_color(cloud, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_radius(cloud, 60, 0);
    lv_obj_align(cloud, LV_ALIGN_TOP_MID, 0, 60);

    lv_obj_t *cloud2 = lv_obj_create(tile);
    lv_obj_set_size(cloud2, 300, 40);
    lv_obj_set_style_bg_color(cloud2, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_radius(cloud2, 40, 0);
    lv_obj_align(cloud2, LV_ALIGN_TOP_MID, 20, 40);

    lv_obj_move_background(cloud);
    lv_obj_move_background(cloud2);

    // ----- Temperature box -----
    lv_obj_t *temp_box = lv_obj_create(tile);
    lv_obj_set_size(temp_box, 150, 70);
    lv_obj_set_style_bg_color(temp_box, lv_color_hex(0x2F2F2F), 0);
    lv_obj_set_style_radius(temp_box, 18, 0);
    lv_obj_set_style_pad_all(temp_box, 8, 0);
    lv_obj_align(temp_box, LV_ALIGN_LEFT_MID, 20, -20);

    // Global temperature label
    temp_label = lv_label_create(temp_box);
    lv_label_set_text(temp_label, "11° C");
    lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_28, 0);
    lv_obj_set_style_text_color(temp_label, text_color, 0);
    lv_obj_center(temp_label);

    // ----- Condition text -----
    lv_obj_t *cond_label = lv_label_create(tile);
    lv_label_set_text(cond_label, "Rainy");
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
    lv_obj_align(card, LV_ALIGN_BOTTOM_MID, 0, -40);

    const char *days[7] = {"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};
    int x = 20;

    for (int i = 0; i < 7; i++) {
        lv_obj_t *lbl = lv_label_create(card);
        lv_label_set_text(lbl, days[i]);
        lv_obj_set_style_text_color(lbl, text_color, 0);
        lv_obj_align(lbl, LV_ALIGN_TOP_LEFT, x, 10);

        temperatures[i] = lv_label_create(card);
        lv_label_set_text(temperatures[i], "11°");
        lv_obj_set_style_text_color(temperatures[i], text_color, 0);
        lv_obj_align(temperatures[i], LV_ALIGN_TOP_LEFT, x, 40);

        x += 55;
    }

    // ----- Raindrops -----
    int start_y = 130;
    int rows    = 6;
    int cols    = 5;
    int dx      = 70;
    int dy      = 45;
    int start_x = 30;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int rx = start_x + c * dx + ((r & 1) ? 30 : 0);
            int ry = start_y + r * dy;
            create_raindrop(tile, rx, ry);
        }
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