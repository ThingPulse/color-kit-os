#ifndef WEATHER_I18N_H
#define WEATHER_I18N_H

#include <string.h>

typedef enum {
    LANG_EN,
    LANG_DE,
    // Add more languages as needed
    LANG_MAX
} language_t;



typedef enum {
    STR_LANG,
    STR_SUN,
    STR_MOON,
    STR_SUNDAY,
    STR_MONDAY,
    STR_TUESDAY,
    STR_WEDNESDAY,
    STR_THURSDAY,
    STR_FRIDAY,
    STR_SATURDAY,
    STR_JANUARY,
    STR_FEBRUARY,
    STR_MARCH,
    STR_APRIL,
    STR_MAY,
    STR_JUNE,
    STR_JULY,
    STR_AUGUST,
    STR_SEPTEMBER,
    STR_OCTOBER,
    STR_NOVEMBER,
    STR_DECEMBER,
    STR_NEW_MOON,
    STR_WAXING_CRESCENT,
    STR_FIRST_QUARTER,
    STR_WAXING_GIBBOUS,
    STR_FULL_MOON,
    STR_WANING_GIBBOUS,
    STR_LAST_QUARTER,
    STR_WANING_CRESCENT,
    STR_DIR_N,
    STR_DIR_NNE,
    STR_DIR_NE,
    STR_DIR_ENE,
    STR_DIR_E,
    STR_DIR_ESE,
    STR_DIR_SE,
    STR_DIR_SSE,
    STR_DIR_S,
    STR_DIR_SSW,
    STR_DIR_SW,
    STR_DIR_WSW,
    STR_DIR_W,
    STR_DIR_WNW,
    STR_DIR_NW,
    STR_DIR_NNW,
    STR_FORECAST_DETAIL,
    STR_MAX
} string_id_t;

extern language_t current_language;

const char* get_string(string_id_t id);

void set_language(language_t lang);

bool set_language_by_code(const char* lang_code);

int get_current_language_index();

#endif