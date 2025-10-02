#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_open_app(lv_event_t * e);
extern void action_open_settings(lv_event_t * e);
extern void action_open_home_screen(lv_event_t * e);
extern void action_open_wifi_settings(lv_event_t * e);
extern void action_settings_wifi_screen_loaded(lv_event_t * e);
extern void action_settings_wifi_screen_created(lv_event_t * e);
extern void action_save_wifi_settings(lv_event_t * e);
extern void action_open_wifi_password_screen(lv_event_t * e);
extern void action_show_wifi_settings_keyboard(lv_event_t * e);
extern void action_hide_wifi_settings_keyboard(lv_event_t * e);
extern void action_return_wifi_settings(lv_event_t * e);
extern void action_keyboard_value_changed(lv_event_t * e);
extern void action_register_header_bar(lv_event_t * e);
extern void action_attach_header_bar(lv_event_t * e);
extern void action_open_weather_app(lv_event_t * e);
extern void action_on_weather_screen_gesture(lv_event_t * e);
extern void action_get_gps_coordinates(lv_event_t * e);
extern void action_open_weather_settings(lv_event_t * e);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/