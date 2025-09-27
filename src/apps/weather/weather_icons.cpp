/****************************************************************************
 *   Copyright  2020-2024  Dirk Brosswick
 *   Email: dirk.brosswick@googlemail.com
 ****************************************************************************/
 
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include "weather_icons.h"
#include <cstring>

struct owm_icon_map {
    const char* iconname;
    const void* icon;
};

static const owm_icon_map owm_icons[] = {
    { "01d", &img_weather_clear_day_64px },
    { "02d", &img_weather_partly_cloudy_day_64px },
    { "03d", &img_weather_cloudy_64px },
    { "04d", &img_weather_partly_cloudy_day_64px },
    { "09d", &img_weather_rain_64px }, // Changed from partly cloudy day to rain
    { "10d", &img_weather_rain_64px },
    { "11d", &img_weather_thunderstorm_64px },
    { "13d", &img_weather_snow_64px },
    { "50d", &img_weather_fog_64px },
    { "01n", &img_weather_clear_night_64px },
    { "02n", &img_weather_partly_cloudy_night_64px },
    { "03n", &img_weather_cloudy_64px },
    { "04n", &img_weather_partly_cloudy_night_64px },
    { "09n", &img_weather_rain_64px }, // Changed from partly cloudy night to rain
    { "10n", &img_weather_rain_64px },
    { "11n", &img_weather_thunderstorm_64px },
    { "13n", &img_weather_snow_64px },
    { "50n", &img_weather_fog_64px }
};

const void * resolve_owm_icon(const char *iconname) {
    for (size_t i = 0; i < (sizeof(owm_icons) / sizeof(owm_icons[0])); ++i) {
        if (strcmp(owm_icons[i].iconname, iconname) == 0) {
            return owm_icons[i].icon;
        }
    }
    // Return a default "Not Available" icon if no match is found
    return &img_weather_n_a_64px;
}