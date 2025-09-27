#ifndef EEZ_LVGL_UI_IMAGES_H
#define EEZ_LVGL_UI_IMAGES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const lv_img_dsc_t img_logo;
extern const lv_img_dsc_t img_weather_app;
extern const lv_img_dsc_t img_settings;
extern const lv_img_dsc_t img_weather;
extern const lv_img_dsc_t img_moon_phase_15;
extern const lv_img_dsc_t img_weather_clear_day;
extern const lv_img_dsc_t img_weather_clear_day_64px;
extern const lv_img_dsc_t img_weather_fog_64px;
extern const lv_img_dsc_t img_weather_sleet_64px;
extern const lv_img_dsc_t img_wind_n;

#ifndef EXT_IMG_DESC_T
#define EXT_IMG_DESC_T
typedef struct _ext_img_desc_t {
    const char *name;
    const lv_img_dsc_t *img_dsc;
} ext_img_desc_t;
#endif

extern const ext_img_desc_t images[10];


extern const lv_img_dsc_t img_undefined;

const void *getLvglImageByName(const char *name);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_IMAGES_H*/