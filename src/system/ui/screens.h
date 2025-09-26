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
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_SPLASHSCREEN = 1,
    SCREEN_ID_HOMESCREEN = 2,
    SCREEN_ID_SETTINGS = 3,
    SCREEN_ID_SETTINGS_WIFI = 4,
    SCREEN_ID_SETTINGS_WIFI_PASSWORD = 5,
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

void create_user_widget_app_button_widget(lv_obj_t *parent_obj, int startWidgetIndex);
void tick_user_widget_app_button_widget(int startWidgetIndex);

    void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();



#ifdef __cplusplus
}
#endif


#endif /*EEZ_LVGL_UI_SCREENS_H*/


  