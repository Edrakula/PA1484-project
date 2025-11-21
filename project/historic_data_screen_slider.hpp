
#ifndef HISTORIC_DATA_SCREEN_SLIDER
#define HISTORIC_DATA_SCREEN_SLIDER

#include <Arduino.h>
#include <LV_Helper.h>
#include <LilyGo_AMOLED.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include "apiconnections.hpp"
#include <sstream>
#include <iomanip>
#include <string>

#include "historic_data_screen.hpp"

extern std::pair<int,int> get_xy_dist_from_center_of_chart_point(int x_index, int y_value, int y_max);

static void slider_event_cb(lv_event_t * e){
  lv_obj_t* slider = lv_event_get_target(e);
  int value = lv_slider_get_value(slider);

  Serial.println(std::to_string(value).c_str());
}

typedef struct {
    lv_obj_t* label;
    std::vector<HistoricData>* data;
    lv_obj_t* red_dot;
    int max_value;
} slider_info_t;


static void slider_event_cb_with_historic_data_and_label(lv_event_t * e)
{
  lv_obj_t * slider = lv_event_get_target(e);

  if(lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
    slider_info_t * info = (slider_info_t *) lv_event_get_user_data(e);
    if(info->data == nullptr || info->red_dot == nullptr) return;

    int value = lv_slider_get_value(slider);

    HistoricData cur_data_point = info->data->at(value);

    std::pair<int,int> cords = get_xy_dist_from_center_of_chart_point(value, cur_data_point.data, info->max_value);

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << cur_data_point.data;
    std::string s = oss.str();
      
    lv_label_set_text(info->label, (cur_data_point.date.ymd() + " : " + s + " " + cur_data_point.unit).c_str());

    lv_obj_center(info->red_dot);
    lv_obj_align(info->red_dot, LV_ALIGN_CENTER, cords.first, cords.second);
  }
}

lv_obj_t* create_slider(lv_obj_t* tile, int length_of_slider){
  lv_obj_t* slider = lv_slider_create(tile);
  
  lv_obj_align(slider, LV_ALIGN_BOTTOM_MID, 0,-15);
  lv_slider_set_range(slider, 0, length_of_slider);
  
  lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  return slider;
}

lv_obj_t* create_slider_that_updates_label_based_on_historic_data(lv_obj_t* tile, int length_of_slider, std::vector<HistoricData>* data, lv_obj_t* label, lv_obj_t* red_dot, int max_value){ 
  lv_obj_t* slider = lv_slider_create(tile);
  
  lv_obj_align(slider, LV_ALIGN_BOTTOM_MID, 0,-15);
  lv_slider_set_range(slider, 0, length_of_slider);
  lv_obj_set_style_radius(slider, 10, LV_PART_KNOB);
  
  static slider_info_t info;
  info.label = label;
  info.data = data;
  info.red_dot = red_dot;
  info.max_value = max_value;

  lv_obj_add_event_cb(slider, slider_event_cb_with_historic_data_and_label, LV_EVENT_VALUE_CHANGED, &info);
  return slider;
}

void update_slider_range(lv_obj_t* slider ,int max) {
  lv_slider_set_range(slider, 0, max);
}

void slider_set_info(lv_obj_t *slider, slider_info_t *new_info) {
  lv_obj_remove_event_cb(slider, slider_event_cb_with_historic_data_and_label);
  lv_obj_add_event_cb(slider, slider_event_cb_with_historic_data_and_label, LV_EVENT_VALUE_CHANGED, new_info);
}

void slider_update_slider_range_and_info(lv_obj_t* slider ,int data_size, lv_obj_t* label, std::vector<HistoricData>* data, lv_obj_t* red_dot, int max_value) {
  static slider_info_t info;
  info.label = label;
  info.data = data;
  info.red_dot = red_dot;
  info.max_value = max_value;

  slider_set_info(slider, &info);
  update_slider_range(slider, data_size - 1);
}


#endif