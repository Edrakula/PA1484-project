#include "lvgl.h"
#include "backend_logic/apiconnections.hpp"
#include <sstream>
#include <iomanip>
#include <string>

lv_obj_t *temperatures[7];
lv_obj_t *temp_label;

// --- Helper for sleet drop ---
static void create_sleet_drop(lv_obj_t *parent, lv_coord_t x, lv_coord_t y)
{
    // diagonal line (icy blue)
    lv_obj_t *line = lv_obj_create(parent);
    lv_obj_set_size(line, 4, 14);
    lv_obj_set_style_radius(line, 2, 0);
    lv_obj_set_style_bg_color(line, lv_color_hex(0xAEE6FF), 0);
    lv_obj_set_style_border_width(line, 0, 0);
    lv_obj_align(line, LV_ALIGN_TOP_MID,-240 + x, y);
    lv_obj_move_background(line);

    // dot
    lv_obj_t *dot = lv_obj_create(parent);
    lv_obj_set_size(dot, 6, 6);
    lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(dot, lv_color_hex(0x000000), 0);

    // ----- Border (black) -----
    lv_obj_set_style_border_width(dot, 2, 0);               // thickness
    lv_obj_set_style_border_color(dot, lv_color_hex(0x000000), 0); // black
    lv_obj_set_style_border_opa(dot, LV_OPA_COVER, 0);      // visible

    lv_obj_align(dot, LV_ALIGN_TOP_MID,240 -x, y);
    lv_obj_move_background(dot);

}

void draw_sleet_ui(lv_obj_t *tile)
{
    lv_color_t text_color = lv_color_hex(0xFFFFFF);

    // ----- Background -----
    lv_obj_set_style_bg_color(tile, lv_color_hex(0x546270), 0);
    lv_obj_set_style_bg_grad_color(tile, lv_color_hex(0xC9E7F5), 0);
    lv_obj_set_style_bg_grad_dir(tile, LV_GRAD_DIR_VER, 0);

    // ----- Cloud -----
    lv_obj_t *cloud = lv_obj_create(tile);
    lv_obj_set_size(cloud, 480, 40);
    lv_obj_set_style_bg_color(cloud, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_radius(cloud, 60, 0);
    lv_obj_align(cloud, LV_ALIGN_TOP_MID, 0, 60);
    lv_obj_set_style_border_color(cloud,lv_color_hex(0xFFFFFF),0);

    lv_obj_t *cloud2 = lv_obj_create(tile);
    lv_obj_set_size(cloud2, 300, 40);
    lv_obj_set_style_bg_color(cloud2, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_radius(cloud2, 40, 0);
    lv_obj_align(cloud2, LV_ALIGN_TOP_MID, 20, 40);
    lv_obj_set_style_border_color(cloud2,lv_color_hex(0xFFFFFF),0);

    // ----- Temperature box -----
    lv_obj_t *temp_box = lv_obj_create(tile);
    lv_obj_set_size(temp_box, 150, 70);
    lv_obj_set_style_bg_color(temp_box, lv_color_hex(0x2F2F2F), 0);
    lv_obj_set_style_radius(temp_box, 18, 0);
    lv_obj_set_style_pad_all(temp_box, 8, 0);
    lv_obj_align(temp_box, LV_ALIGN_LEFT_MID, 20, -20);

    // Temperature label (same pointer as sunny!)
    temp_label = lv_label_create(temp_box);
    lv_label_set_text(temp_label, "0° C");
    lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_28, 0);
    lv_obj_set_style_text_color(temp_label, text_color, 0);
    lv_obj_center(temp_label);

    // ----- Condition -----
    lv_obj_t *cond_label = lv_label_create(tile);
    lv_label_set_text(cond_label, "Sleet");
    lv_obj_set_style_text_font(cond_label, &lv_font_montserrat_28, 0);
    lv_obj_set_style_text_color(cond_label, lv_color_hex(0x000000), 0);
    lv_obj_align(cond_label, LV_ALIGN_LEFT_MID, 25, 30);

    // ----- City -----
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

    const char *days[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    int x = 20;

    for (int i = 0; i < 7; i++) {
        // Day label
        lv_obj_t *lbl = lv_label_create(card);
        lv_label_set_text(lbl, days[i]);
        lv_obj_set_style_text_color(lbl, text_color, 0);
        lv_obj_align(lbl, LV_ALIGN_TOP_LEFT, x, 10);

        // Temperature label (same array as sunny!)
        temperatures[i] = lv_label_create(card);
        lv_obj_t *t = temperatures[i];
        lv_label_set_text(t, "0°");
        lv_obj_set_style_text_color(t, text_color, 0);
        lv_obj_align(t, LV_ALIGN_TOP_LEFT, x, 40);

        x += 55;
    }

    // ----- Sleet drops -----
    int start_y = 130;
    int rows    = 5;
    int cols    = 5;
    int dx      = 70;
    int dy      = 45;
    int start_x = 30;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int sx = start_x + c * dx + ((r & 1) ? 30 : 0);
            int sy = start_y + r * dy;
            create_sleet_drop(tile, sx, sy);
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
