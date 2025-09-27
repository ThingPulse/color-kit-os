#include "images.h"

#include <string.h>
const ext_img_desc_t images[69] = {
    { "logo", &img_logo },
    { "weatherApp", &img_weather_app },
    { "settings", &img_settings },
    { "weather", &img_weather },
    { "moon-phase-15", &img_moon_phase_15 },
    { "weather-clear-day", &img_weather_clear_day },
    { "weather-clear-day-64px", &img_weather_clear_day_64px },
    { "weather-fog-64px", &img_weather_fog_64px },
    { "weather-sleet-64px", &img_weather_sleet_64px },
    { "cog", &img_cog },
    { "moon-phase-0", &img_moon_phase_0 },
    { "moon-phase-1", &img_moon_phase_1 },
    { "moon-phase-10", &img_moon_phase_10 },
    { "moon-phase-11", &img_moon_phase_11 },
    { "moon-phase-12", &img_moon_phase_12 },
    { "moon-phase-13", &img_moon_phase_13 },
    { "moon-phase-14", &img_moon_phase_14 },
    { "moon-phase-16", &img_moon_phase_16 },
    { "moon-phase-17", &img_moon_phase_17 },
    { "moon-phase-18", &img_moon_phase_18 },
    { "moon-phase-19", &img_moon_phase_19 },
    { "moon-phase-2", &img_moon_phase_2 },
    { "moon-phase-20", &img_moon_phase_20 },
    { "moon-phase-21", &img_moon_phase_21 },
    { "moon-phase-22", &img_moon_phase_22 },
    { "moon-phase-23", &img_moon_phase_23 },
    { "moon-phase-24", &img_moon_phase_24 },
    { "moon-phase-25", &img_moon_phase_25 },
    { "moon-phase-26", &img_moon_phase_26 },
    { "moon-phase-27", &img_moon_phase_27 },
    { "moon-phase-28", &img_moon_phase_28 },
    { "moon-phase-29", &img_moon_phase_29 },
    { "moon-phase-3", &img_moon_phase_3 },
    { "moon-phase-30", &img_moon_phase_30 },
    { "moon-phase-31", &img_moon_phase_31 },
    { "moon-phase-4", &img_moon_phase_4 },
    { "moon-phase-5", &img_moon_phase_5 },
    { "moon-phase-6", &img_moon_phase_6 },
    { "moon-phase-7", &img_moon_phase_7 },
    { "moon-phase-8", &img_moon_phase_8 },
    { "moon-phase-9", &img_moon_phase_9 },
    { "weather-clear-night", &img_weather_clear_night },
    { "weather-clear-night-64px", &img_weather_clear_night_64px },
    { "weather-cloudy", &img_weather_cloudy },
    { "weather-cloudy-64px", &img_weather_cloudy_64px },
    { "weather-drizzle", &img_weather_drizzle },
    { "weather-drizzle-64px", &img_weather_drizzle_64px },
    { "weather-extreme-rain", &img_weather_extreme_rain },
    { "weather-extreme-rain-64px", &img_weather_extreme_rain_64px },
    { "weather-fog", &img_weather_fog },
    { "weather-light-rain", &img_weather_light_rain },
    { "weather-light-rain-64px", &img_weather_light_rain_64px },
    { "weather-n-a", &img_weather_n_a },
    { "weather-n-a-64px", &img_weather_n_a_64px },
    { "weather-partly-cloudy-day", &img_weather_partly_cloudy_day },
    { "weather-partly-cloudy-day-64px", &img_weather_partly_cloudy_day_64px },
    { "weather-partly-cloudy-night", &img_weather_partly_cloudy_night },
    { "weather-partly-cloudy-night-64px", &img_weather_partly_cloudy_night_64px },
    { "weather-rain", &img_weather_rain },
    { "weather-rain-64px", &img_weather_rain_64px },
    { "weather-sleet", &img_weather_sleet },
    { "weather-snow", &img_weather_snow },
    { "weather-snow-64px", &img_weather_snow_64px },
    { "weather-thunderstorm", &img_weather_thunderstorm },
    { "weather-thunderstorm-64px", &img_weather_thunderstorm_64px },
    { "weather-unknown", &img_weather_unknown },
    { "weather-unknown-64px", &img_weather_unknown_64px },
    { "wind_arrow", &img_wind_arrow },
    { "wind_rose", &img_wind_rose },
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