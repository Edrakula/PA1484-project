
#ifndef HISTORIC_DATA_SCREEN_SLIDER
#define HISTORIC_DATA_SCREEN_SLIDER

#include <Arduino.h>
#include <LV_Helper.h>
#include <LilyGo_AMOLED.h>
#include <TFT_eSPI.h>
#include <lvgl.h>


void create_slider(){
  lv_obj_t *slider = lv_slider_create(lv_scr_act());
  
  lv_obj_align(slider, LV_ALIGN_CENTER, 0,-10);
  lv_slider_set_range(slider, 0, 11);
  
  lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

static void slider_event_cb(lv_event_t * e){
  lv_obj_t * slider = lv_event_get_target(e);
  int value = lv_slider_get_value(slider);
}






#endif