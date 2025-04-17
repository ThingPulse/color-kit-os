#include "weather_i18n.h"

language_t current_language = LANG_EN;

const char* weather_lang_strings_en[] = {
    [STR_LANG]             = "en",        // ID 0
    [STR_SUN]              = "Sun",     // ID 1
    [STR_MOON]             = "Moon",     // ID 2
    [STR_SUNDAY]           = "Sun",
    [STR_MONDAY]           = "Mon",
    [STR_TUESDAY]          = "Tue",
    [STR_WEDNESDAY]        = "Wed",
    [STR_THURSDAY]         = "Thu",
    [STR_FRIDAY]           = "Fri",
    [STR_SATURDAY]         = "Sat",
    [STR_JANUARY]          = "Jan",
    [STR_FEBRUARY]         = "Feb",
    [STR_MARCH]            = "Mar",
    [STR_APRIL]            = "Apr",
    [STR_MAY]              = "May",
    [STR_JUNE]             = "Jun",
    [STR_JULY]             = "Jul",
    [STR_AUGUST]           = "Aug",
    [STR_SEPTEMBER]        = "Sep",
    [STR_OCTOBER]          = "Oct",
    [STR_NOVEMBER]         = "Nov",
    [STR_DECEMBER]         = "Dec",
    [STR_NEW_MOON]         = "New Moon",
    [STR_WAXING_CRESCENT]  = "Waxing Crescent",
    [STR_FIRST_QUARTER]    = "First Quarter",
    [STR_WAXING_GIBBOUS]   = "Waxing Gibbous",
    [STR_FULL_MOON]        = "Full Moon",
    [STR_WANING_GIBBOUS]   = "Waning Gibbous",
    [STR_LAST_QUARTER]     = "Last Quarter",
    [STR_WANING_CRESCENT]  = "Waning Crescent",
    [STR_FORECAST_DETAIL]  = "Next 24h"
};

const char* weather_lang_strings_de[] = {
    [STR_LANG]             = "de",        // ID 0
    [STR_SUN]              = "Sonne",// ID 1
    [STR_MOON]             = "Mond",      // ID 2
    [STR_SUNDAY]           = "So",
    [STR_MONDAY]           = "Mo",
    [STR_TUESDAY]          = "Di",
    [STR_WEDNESDAY]        = "Mi",
    [STR_THURSDAY]         = "Do",
    [STR_FRIDAY]           = "Fr",
    [STR_SATURDAY]         = "Sa",
    [STR_JANUARY]          = "Jan",
    [STR_FEBRUARY]         = "Feb",
    [STR_MARCH]            = "Mar",
    [STR_APRIL]            = "Apr",
    [STR_MAY]              = "Mai",
    [STR_JUNE]             = "Jun",
    [STR_JULY]             = "Jul",
    [STR_AUGUST]           = "Aug",
    [STR_SEPTEMBER]        = "Sep",
    [STR_OCTOBER]          = "Okt",
    [STR_NOVEMBER]         = "Nov",
    [STR_DECEMBER]         = "Dez",
    [STR_NEW_MOON]         = "Neumond",
    [STR_WAXING_CRESCENT]  = "Zunehmende Sichel",
    [STR_FIRST_QUARTER]    = "Erstes Viertel",
    [STR_WAXING_GIBBOUS]   = "Zunehmender Mond",
    [STR_FULL_MOON]        = "Vollmond",
    [STR_WANING_GIBBOUS]   = "Abnehmender Mond",
    [STR_LAST_QUARTER]     = "Letztes Viertel",
    [STR_WANING_CRESCENT]  = "Abnehmende Sichel",
    [STR_FORECAST_DETAIL]  = "Nächste 24h"
};

const char* get_string(string_id_t id) {
    switch (current_language) {
        case LANG_EN: return weather_lang_strings_en[id];
        case LANG_DE: return weather_lang_strings_de[id];
        default: return weather_lang_strings_en[id];
    }
}

void set_language(language_t lang) {
    if (lang >= LANG_MAX) return;
    current_language = lang;
}

bool set_language_by_code(const char* lang_code) {
    if (strcmp(lang_code, "en") == 0) {
        set_language(LANG_EN);
        return true;
    } else if (strcmp(lang_code, "de") == 0) {
        set_language(LANG_DE);
        return true;
    }
    // Add more as needed
    return false; // Unsupported language code
}

int get_current_language_index() {
    return current_language;
}