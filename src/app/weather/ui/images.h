#ifndef EEZ_LVGL_UI_IMAGES_H
#define EEZ_LVGL_UI_IMAGES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const lv_img_dsc_t img_weather_unknown;
extern const lv_img_dsc_t img_weather_thunderstorm;
extern const lv_img_dsc_t img_weather_snow;
extern const lv_img_dsc_t img_weather_sleet;
extern const lv_img_dsc_t img_weather_rain;
extern const lv_img_dsc_t img_weather_partly_cloudy_night;
extern const lv_img_dsc_t img_weather_partly_cloudy_day;
extern const lv_img_dsc_t img_weather_n_a;
extern const lv_img_dsc_t img_weather_light_rain;
extern const lv_img_dsc_t img_weather_fog;
extern const lv_img_dsc_t img_weather_extreme_rain;
extern const lv_img_dsc_t img_weather_drizzle;
extern const lv_img_dsc_t img_weather_cloudy;
extern const lv_img_dsc_t img_weather_clear_night;
extern const lv_img_dsc_t img_weather_clear_day;
extern const lv_img_dsc_t img_thingpulse_logo_260;
extern const lv_img_dsc_t img_cog;
extern const lv_img_dsc_t img_wind_nne;
extern const lv_img_dsc_t img_wind_ne;
extern const lv_img_dsc_t img_wind_ene;
extern const lv_img_dsc_t img_wind_e;
extern const lv_img_dsc_t img_wind_ese;
extern const lv_img_dsc_t img_wind_se;
extern const lv_img_dsc_t img_wind_sse;
extern const lv_img_dsc_t img_wind_s;
extern const lv_img_dsc_t img_wind_ssw;
extern const lv_img_dsc_t img_wind_sw;
extern const lv_img_dsc_t img_wind_wsw;
extern const lv_img_dsc_t img_wind_w;
extern const lv_img_dsc_t img_wind_wnw;
extern const lv_img_dsc_t img_wind_nw;
extern const lv_img_dsc_t img_wind_nnw;
extern const lv_img_dsc_t img_wind_n;
extern const lv_img_dsc_t img_weather_clear_night_64px;
extern const lv_img_dsc_t img_weather_cloudy_64px;
extern const lv_img_dsc_t img_weather_drizzle_64px;
extern const lv_img_dsc_t img_weather_extreme_rain_64px;
extern const lv_img_dsc_t img_weather_fog_64px;
extern const lv_img_dsc_t img_weather_light_rain_64px;
extern const lv_img_dsc_t img_weather_n_a_64px;
extern const lv_img_dsc_t img_weather_partly_cloudy_day_64px;
extern const lv_img_dsc_t img_weather_partly_cloudy_night_64px;
extern const lv_img_dsc_t img_weather_rain_64px;
extern const lv_img_dsc_t img_weather_sleet_64px;
extern const lv_img_dsc_t img_weather_snow_64px;
extern const lv_img_dsc_t img_weather_thunderstorm_64px;
extern const lv_img_dsc_t img_weather_unknown_64px;
extern const lv_img_dsc_t img_weather_clear_day_64px;
extern const lv_img_dsc_t img_moon_phase_31;
extern const lv_img_dsc_t img_moon_phase_30;
extern const lv_img_dsc_t img_moon_phase_29;
extern const lv_img_dsc_t img_moon_phase_28;
extern const lv_img_dsc_t img_moon_phase_27;
extern const lv_img_dsc_t img_moon_phase_26;
extern const lv_img_dsc_t img_moon_phase_25;
extern const lv_img_dsc_t img_moon_phase_24;
extern const lv_img_dsc_t img_moon_phase_23;
extern const lv_img_dsc_t img_moon_phase_22;
extern const lv_img_dsc_t img_moon_phase_21;
extern const lv_img_dsc_t img_moon_phase_20;
extern const lv_img_dsc_t img_moon_phase_19;
extern const lv_img_dsc_t img_moon_phase_18;
extern const lv_img_dsc_t img_moon_phase_17;
extern const lv_img_dsc_t img_moon_phase_16;
extern const lv_img_dsc_t img_moon_phase_15;
extern const lv_img_dsc_t img_moon_phase_14;
extern const lv_img_dsc_t img_moon_phase_13;
extern const lv_img_dsc_t img_moon_phase_12;
extern const lv_img_dsc_t img_moon_phase_11;
extern const lv_img_dsc_t img_moon_phase_10;
extern const lv_img_dsc_t img_moon_phase_9;
extern const lv_img_dsc_t img_moon_phase_8;
extern const lv_img_dsc_t img_moon_phase_7;
extern const lv_img_dsc_t img_moon_phase_6;
extern const lv_img_dsc_t img_moon_phase_5;
extern const lv_img_dsc_t img_moon_phase_4;
extern const lv_img_dsc_t img_moon_phase_3;
extern const lv_img_dsc_t img_moon_phase_2;
extern const lv_img_dsc_t img_moon_phase_1;
extern const lv_img_dsc_t img_moon_phase_0;

#ifndef EXT_IMG_DESC_T
#define EXT_IMG_DESC_T
typedef struct _ext_img_desc_t {
    const char *name;
    const lv_img_dsc_t *img_dsc;
} ext_img_desc_t;
#endif

extern const ext_img_desc_t images[80];


extern const lv_img_dsc_t img_undefined;

const void *getLvglImageByName(const char *name);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_IMAGES_H*/