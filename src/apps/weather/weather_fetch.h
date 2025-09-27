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
#ifndef _WEATHER_FETCH_H
#define _WEATHER_FETCH_H

#include <cstdint>

#define OWM_HOST "api.openweathermap.org"
#define OWM_PORT 80

#define MAX_FORECAST_DAYS 4
#define MAX_FORECAST_HOURS 9

typedef struct {
    bool imperial;
    const char* lang;
    const char* lat;
    const char* lon;
    const char* apikey;
} weather_config_t;

typedef struct {
    bool valide;
    long timestamp;
    float temp;
    float temp_min;
    float temp_max;
    float humidity;
    float pressure;
    char icon[16];
    char description[64];
    char name[64];
    int wind_speed;
    int wind_deg;
    float pop; // Probability of precipitation
} weather_forecast_t;

typedef struct {
    uint8_t weekday;
    long timestamp;
    float min_temp;
    float max_temp;
    float max_wind_speed;
    char icon_at_noon[16];
} weather_forecast_raw_t;

int weather_fetch_today(weather_config_t *weather_config, weather_forecast_t *weather_today);
int weather_fetch_forecast(weather_config_t *weather_config, weather_forecast_t *daily_forecast, weather_forecast_t *hourly_forecast);

#endif // _WEATHER_FETCH_H