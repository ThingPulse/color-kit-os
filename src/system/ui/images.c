#include "images.h"

#include <string.h>
const ext_img_desc_t images[10] = {
    { "logo", &img_logo },
    { "weatherApp", &img_weather_app },
    { "settings", &img_settings },
    { "weather", &img_weather },
    { "moon-phase-15", &img_moon_phase_15 },
    { "weather-clear-day", &img_weather_clear_day },
    { "weather-clear-day-64px", &img_weather_clear_day_64px },
    { "weather-fog-64px", &img_weather_fog_64px },
    { "weather-sleet-64px", &img_weather_sleet_64px },
    { "wind-n", &img_wind_n },
};


const void *getLvglImageByName(const char *name) {
    for (size_t imageIndex = 0; imageIndex < sizeof(images) / sizeof(ext_img_desc_t); imageIndex++) {
        if (strcmp(images[imageIndex].name, name) == 0) {
            return images[imageIndex].img_dsc;
        }
    }
    return 0;
}

const lv_img_dsc_t img_undefined = {
    .header.w = 0,
    .header.h = 0,
    .header.cf = LV_COLOR_FORMAT_RAW_ALPHA,
    .data_size = 0,
    .data = NULL,
};