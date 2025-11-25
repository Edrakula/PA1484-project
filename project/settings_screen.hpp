#ifndef SETTINGS_SCREEN_HPP
#define SETTINGS_SCREEN_HPP

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <time.h>
#include <LilyGo_AMOLED.h>
#include <LV_Helper.h>
#include <lvgl.h>

void create_settings_screen(lv_obj_t* tileview, lv_obj_t* tile)
{
    //Weather dropdown.
    lv_obj_t* weatherDropdown = lv_dropdown_create(tile);
    lv_dropdown_set_options(
        weatherDropdown,
        "Temperature\n"
        "Humidity\n"
        "Wind speed\n"
        "Air pressure"
    );

    lv_obj_add_event_cb(weatherDropdown, weather_dropdown_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    //City dropdown.
    lv_obj_t* cityDropdown = lv_dropdown_create(tile);
    lv_dropdown_set_options(
        cityDropdown,
        "Karlskrona\n"
        "Stockholm\n"
        "Göteborg\n"
        "Malmö\n"
        "Kiruna"
    );

    lv_obj_add_event_cb(cityDropdown, city_dropdown_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

void weather_dropdown_event_cb(lv_event_t* event)
{
    lv_obj_t* dropDownObject = lv_event_get_target(event);
    uint16_t selected = lv_dropdown_get_selected(dropDownObject);
    switch (selected)
    {
        case 0:
            //Visa temperature.
            break;
        case 1:
            //Visa humidity.
            break;
        case 2:
            //Visa wind speed.
            break;
        case 3:
            //Visa air pressure.
            break;
    }
}

void city_dropdown_event_cb(lv_event_t* event)
{
    lv_obj_t* dropdownObject = lv_event_get_target(event);
    uint16_t selected = lv_dropdown_get_selected(dropdownObject);
    switch (selected)
    {
        case 0:
            //Visa Karlskrona.
            break;
        case 1:
            //Visa Stockholm.
            break;
        case 2:
            //Visa Göteborg.
            break;
        case 3:
            //Visa Malmö.
            break;
        case 4:
            //Visa Kiruna.
            break;
    }
}

#endif
