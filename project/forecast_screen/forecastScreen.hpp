
#ifndef FORECAST_SCREEN
#define FORECAST_SCREEN

#include "lvgl.h"
#include "backend_logic/apiconnections.hpp"
#include <sstream>
#include <iomanip>
#include <string>
#include "small_icons/sunny_icon.hpp"
#include "small_icons/snow_icon.hpp"
#include "small_icons/cloud_icon.hpp"
#include "small_icons/rain_icon.hpp"
#include "forecast_screen/Bg_sunny.hpp"
#include "forecast_screen/Bg_cloudy.hpp"
#include "forecast_screen/Bg_rainy.hpp"
#include <algorithm>

lv_obj_t *temperatures[7];
lv_obj_t *daysLabels[7]; 
const char *days[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
lv_obj_t *temp_label;
lv_obj_t *card;
lv_obj_t* bg_obj;

lv_obj_t* dayIcons[7] = {nullptr};

lv_obj_t *cond_label;

lv_obj_t* draw_forecast_screen(lv_obj_t *tile, std::string name)
{
    // common text color
    lv_color_t text_color = lv_color_hex(0xFFFFFF);

    // ----- Condition text ("Sunny") -----
    cond_label = lv_label_create(tile);
    lv_label_set_text(cond_label, "Sunny");
    lv_obj_set_style_text_font(cond_label, &lv_font_montserrat_28, 0);
    lv_obj_set_style_text_color(cond_label, lv_color_hex(0x000000), 0);
    lv_obj_align(cond_label, LV_ALIGN_LEFT_MID, 25, 30);

    


    // ----- Temperature box -----
    lv_obj_t *temp_box = lv_obj_create(tile);
    lv_obj_set_size(temp_box, 150, 70);
    lv_obj_set_style_bg_color(temp_box, lv_color_hex(0x2F2F2F), 0);
    lv_obj_set_style_radius(temp_box, 18, 0);
    lv_obj_set_style_border_width(temp_box, 0, 0);
    lv_obj_set_style_pad_all(temp_box, 8, 0);
    lv_obj_align(temp_box, LV_ALIGN_LEFT_MID, 20, -20);

    // Temperature label inside the box
    temp_label = lv_label_create(temp_box);
    lv_label_set_text(temp_label, "11° C");              // placeholder
    lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_28, 0);
    lv_obj_set_style_text_color(temp_label, text_color, 0);
    lv_obj_center(temp_label);

    // ----- City name -----
    lv_obj_t *city_label = lv_label_create(tile);
    lv_label_set_text(city_label, name.c_str());
    lv_obj_set_style_text_color(city_label, lv_color_hex(0x0000000), 0);
    lv_obj_align(city_label, LV_ALIGN_TOP_MID, 0, 10);

    // ----- Weekly forecast card -----
    card = lv_obj_create(tile);
    lv_obj_set_size(card, 420, 120);
    lv_obj_set_style_bg_color(card, lv_color_hex(0x3A3A3A), 0);
    lv_obj_set_style_radius(card, 25, 0);
    lv_obj_set_style_border_width(card, 0, 0);
    lv_obj_align(card, LV_ALIGN_BOTTOM_MID, 0, -40);

    int x = 20;

    for (int i = 0; i < 7; i++) {
        // day name
        daysLabels[i] = lv_label_create(card);
        lv_label_set_text(daysLabels[i], days[i]);
        lv_obj_set_style_text_color(daysLabels[i], text_color, 0);
        lv_obj_align(daysLabels[i], LV_ALIGN_TOP_LEFT, x, 10);

        // small temperature
        temperatures[i] = lv_label_create(card);
        lv_obj_t *t = temperatures[i];
        lv_label_set_text(t, "11°");        // placeholder for day temp
        lv_obj_set_style_text_color(t, text_color, 0);
        lv_obj_align(t, LV_ALIGN_TOP_LEFT, x, 40);

        x += 55;
    }

    return city_label;
}

void update_temperatures(std::vector<ForecastTemp> forecastTemps, lv_obj_t* tile) {
    int y = 7;
    size_t forecastTempsSize = forecastTemps.size();
    if (forecastTemps.size() < 7) {
        y = forecastTemps.size();
    }

    std::ostringstream oss0;
    oss0 << std::fixed << std::setprecision(1) << forecastTemps[0].temp;

    std::string s0 = oss0.str();

    lv_label_set_text(temp_label, (s0 + "°").c_str()); 

    int x = 22;

    // bg_obj = draw_rainy_ui(tile, cond_label);

    if (forecastTemps[0].rainProbability > 0.7) bg_obj = draw_rainy_ui(tile, cond_label);
    else if (forecastTemps[0].cloudAreaFraction > 4) bg_obj = draw_cloudy_ui(tile, cond_label);
    else bg_obj = draw_sunny_ui(tile, cond_label);


    for (int i = 0; i < y; i++) {
        lv_obj_t *t = temperatures[i];

        ForecastTemp curr_temp = forecastTemps[i];

        if (dayIcons[i] != nullptr) lv_obj_del(dayIcons[i]);

        if (curr_temp.snowProbability > 0.5) dayIcons[i] = get_snow_icon(card, x);
        else if (curr_temp.rainProbability > 0.7) dayIcons[i] = get_rain_icon(card, x);
        else if (curr_temp.cloudAreaFraction > 4) dayIcons[i] = get_cloud_icon(card, x);
        else dayIcons[i] = get_sunny_icon(card, x);


        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << forecastTemps[i].temp;

        std::string s = oss.str();

        lv_label_set_text(t, (s + "°").c_str());
        Serial.println((forecastTemps[i].date.ymdhms() + " : " + std::to_string(forecastTemps[i].date.weekday())).c_str());
        lv_label_set_text(daysLabels[i], days[forecastTemps[i].date.weekday() % 7]);

        x += 55;
    }

}

#endif