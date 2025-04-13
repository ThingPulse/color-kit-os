#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *time_container;
    lv_obj_t *label_time;
    lv_obj_t *label_date;
    lv_obj_t *today_container;
    lv_obj_t *label_wind_speed;
    lv_obj_t *label_pressure;
    lv_obj_t *label_humidity;
    lv_obj_t *label_current_description;
    lv_obj_t *image_wind;
    lv_obj_t *image_current_weather;
    lv_obj_t *label_current_temperature;
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
    lv_obj_t *last_update;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
};

void create_screen_main(lv_obj_t *tile);
void tick_screen_main();

void create_screens();
void tick_screen(int screen_index);



#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/