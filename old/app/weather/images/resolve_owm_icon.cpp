/****************************************************************************
 *   Thu July 23 12:17:01 2020
 *   Copyright  2020  Dirk Brosswick
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
#include "config.h"

#include "resolve_owm_icon.h"
#include "utils/logging.h"


LV_IMG_DECLARE(img_weather_unknown);
LV_IMG_DECLARE(img_weather_thunderstorm);
LV_IMG_DECLARE(img_weather_snow);
LV_IMG_DECLARE(img_weather_sleet);
LV_IMG_DECLARE(img_weather_rain);
LV_IMG_DECLARE(img_weather_partly_cloudy_night);
LV_IMG_DECLARE(img_weather_partly_cloudy_day);
LV_IMG_DECLARE(img_weather_n_a);
LV_IMG_DECLARE(img_weather_light_rain);
LV_IMG_DECLARE(img_weather_fog);
LV_IMG_DECLARE(img_weather_extreme_rain);
LV_IMG_DECLARE(img_weather_drizzle);
LV_IMG_DECLARE(img_weather_cloudy);
LV_IMG_DECLARE(img_weather_clear_night);
LV_IMG_DECLARE(img_weather_clear_day);

LV_IMG_DECLARE(img_weather_unknown_64px);
LV_IMG_DECLARE(img_weather_thunderstorm_64px);
LV_IMG_DECLARE(img_weather_snow_64px);
LV_IMG_DECLARE(img_weather_sleet_64px);
LV_IMG_DECLARE(img_weather_rain_64px);
LV_IMG_DECLARE(img_weather_partly_cloudy_night_64px);
LV_IMG_DECLARE(img_weather_partly_cloudy_day_64px);
LV_IMG_DECLARE(img_weather_n_a_64px);
LV_IMG_DECLARE(img_weather_light_rain_64px);
LV_IMG_DECLARE(img_weather_fog_64px);
LV_IMG_DECLARE(img_weather_extreme_rain_64px);
LV_IMG_DECLARE(img_weather_drizzle_64px);
LV_IMG_DECLARE(img_weather_cloudy_64px);
LV_IMG_DECLARE(img_weather_clear_night_64px);
LV_IMG_DECLARE(img_weather_clear_day_64px);

struct owm_icon owm_icon[ 18 ] = {
    { "01d", &img_weather_clear_day },
    { "02d", &img_weather_partly_cloudy_day },
    { "03d", &img_weather_cloudy },
    { "04d", &img_weather_partly_cloudy_day },
    { "09d", &img_weather_partly_cloudy_day },
    { "10d", &img_weather_rain },
    { "11d", &img_weather_thunderstorm },
    { "13d", &img_weather_snow },
    { "50d", &img_weather_fog },
    { "01n", &img_weather_clear_night },
    { "02n", &img_weather_partly_cloudy_night },
    { "03n", &img_weather_cloudy },
    { "04n", &img_weather_partly_cloudy_night },
    { "09n", &img_weather_partly_cloudy_night },
    { "10n", &img_weather_rain },
    { "11n", &img_weather_thunderstorm },
    { "13n", &img_weather_snow },
    { "50n", &img_weather_fog }
};

struct owm_icon owm_icon_64px[ 18 ] = {
    { "01d", &img_weather_clear_day_64px },
    { "02d", &img_weather_partly_cloudy_day_64px },
    { "03d", &img_weather_cloudy_64px },
    { "04d", &img_weather_partly_cloudy_day_64px },
    { "09d", &img_weather_partly_cloudy_day_64px },
    { "10d", &img_weather_rain_64px },
    { "11d", &img_weather_thunderstorm_64px },
    { "13d", &img_weather_snow_64px },
    { "50d", &img_weather_fog_64px },
    { "01n", &img_weather_clear_night_64px },
    { "02n", &img_weather_partly_cloudy_night_64px },
    { "03n", &img_weather_cloudy_64px },
    { "04n", &img_weather_partly_cloudy_night_64px },
    { "09n", &img_weather_partly_cloudy_night_64px },
    { "10n", &img_weather_rain_64px },
    { "11n", &img_weather_thunderstorm_64px },
    { "13n", &img_weather_snow_64px },
    { "50n", &img_weather_fog_64px }
};

const void * resolve_owm_icon( char *iconname, bool is64PxIcon ) {

    for ( uint32_t icon = 0 ; icon < 18 ; icon++ ) {
        if ( !strcmp( (const char*)owm_icon[ icon ].iconname , (const char*)iconname ) ){
            if ( is64PxIcon ) {
                return( owm_icon_64px[ icon ].icon );
            } else {
                return( owm_icon[ icon ].icon );
            }
        }
    }
    return( &img_weather_n_a );
}

