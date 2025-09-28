#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>
#include "images.h"

#undef lv_obj_clear_flag
#define lv_obj_clear_flag(obj, flag) lv_obj_remove_flag((obj), (lv_obj_flag_t)(flag))


#ifdef __cplusplus
extern "C" {
#endif
    

    typedef struct _objects_t {
    lv_obj_t *splashscreen;
    lv_obj_t *homescreen;
    lv_obj_t *settings;
    lv_obj_t *settings_wifi;
    lv_obj_t *settings_wifi_password;
    lv_obj_t *weather_main;
    lv_obj_t *message_label;
    lv_obj_t *os_name_label;
    lv_obj_t *os_name_label_1;
    lv_obj_t *app_container;
    lv_obj_t *weather_app_label_1;
    lv_obj_t *weather_app_label_2;
    lv_obj_t *title_bar;
    lv_obj_t *title_bar_time;
    lv_obj_t *title_bar_wifi;
    lv_obj_t *settings_header_bar_container;
    lv_obj_t *obj0;
    lv_obj_t *ssid_button_matrix;
    lv_obj_t *obj1;
    lv_obj_t *obj2;
    lv_obj_t *ssid_input;
    lv_obj_t *obj3;
    lv_obj_t *password_input;
    lv_obj_t *wifi_settings_keyboard;
    lv_obj_t *time_container;
    lv_obj_t *weather_label_time;
    lv_obj_t *weather_label_date;
    lv_obj_t *today_container;
    lv_obj_t *weather_label_wind_speed;
    lv_obj_t *weather_label_pressure;
    lv_obj_t *weather_label_humidity;
    lv_obj_t *weather_label_current_description;
    lv_obj_t *weather_image_wind_rose;
    lv_obj_t *weather_image_wind_direction;
    lv_obj_t *weather_image_current_weather;
    lv_obj_t *weather_label_current_temp;
    lv_obj_t *forecast_container;
    lv_obj_t *label_temp_day0;
    lv_obj_t *label_day1;
    lv_obj_t *label_day2;
    lv_obj_t *label_day3;
    lv_obj_t *label_day0;
    lv_obj_t *label_temp_day1;
    lv_obj_t *label_temp_day2;
    lv_obj_t *label_temp_day3;
    lv_obj_t *image_day0;
    lv_obj_t *image_day1;
    lv_obj_t *image_day2;
    lv_obj_t *image_day3;
    lv_obj_t *message_container;
    lv_obj_t *label_error_message;
    lv_obj_t *image_moon;
    lv_obj_t *label_sun;
    lv_obj_t *label_moon;
    lv_obj_t *label_sunrise;
    lv_obj_t *label_sunset;
    lv_obj_t *label_moonrise;
    lv_obj_t *label_moonset;
    lv_obj_t *label_moon_phase;
    lv_obj_t *obj4;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_SPLASHSCREEN = 1,
    SCREEN_ID_HOMESCREEN = 2,
    SCREEN_ID_SETTINGS = 3,
    SCREEN_ID_SETTINGS_WIFI = 4,
    SCREEN_ID_SETTINGS_WIFI_PASSWORD = 5,
    SCREEN_ID_WEATHER_MAIN = 6,
};

void create_screen_splashscreen();
void tick_screen_splashscreen();

void create_screen_homescreen();
void tick_screen_homescreen();

void create_screen_settings();
void tick_screen_settings();

void create_screen_settings_wifi();
void tick_screen_settings_wifi();

void create_screen_settings_wifi_password();
void tick_screen_settings_wifi_password();

void create_screen_weather_main();
void tick_screen_weather_main();

void create_user_widget_app_button_widget(lv_obj_t *parent_obj, int startWidgetIndex);
void tick_user_widget_app_button_widget(int startWidgetIndex);

    void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();



#ifdef __cplusplus
}
#endif


#endif /*EEZ_LVGL_UI_SCREENS_H*/


  