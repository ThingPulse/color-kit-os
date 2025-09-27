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
#ifndef _WEATHER_ICONS_H
#define _WEATHER_ICONS_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

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


const void * resolve_owm_icon(const char *iconname);

#ifdef __cplusplus
}
#endif

#endif // _WEATHER_ICONS_H