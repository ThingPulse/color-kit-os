/****************************************************************************
 *   July 23 00:23:05 2020
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
#include "weather.h"
#include "weather_fetch.h"
#include "weather_forecast.h"
#include "hardware/powermgm.h"
#include "utils/json_psram_allocator.h"
#include "utils/uri_load/uri_load.h"
#include "i18n/weather_i18n.h"

string_id_t WIND_DIRECTION_KEYS[] = {STR_DIR_N,
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
    STR_DIR_NNW};

/**
 * Utility function to convert numbers to directions
 */
static void weather_wind_to_string( weather_forcast_t* container, int speed, int directionDegree);

int weather_fetch_today( weather_config_t *weather_config, weather_forcast_t *weather_today ) {
    char url[512]="";
    int httpcode = -1;
    const char* weather_units_symbol = weather_config->imperial ? "F" : "C";
    const char* weather_units_char = weather_config->imperial ? "imperial" : "metric";
    const char* weather_lang = weather_config->lang;
    /**
     * build uri string
     */
    snprintf( url, sizeof( url ), "http://%s/data/2.5/weather?lat=%s&lon=%s&appid=%s&units=%s&lang=%s", 
        OWM_HOST, weather_config->lat, weather_config->lon, weather_config->apikey, weather_units_char, weather_lang);
    log_d("http get: %s", url );
    /**
     * load uri file into ram
     */
    uri_load_dsc_t *uri_load_dsc = uri_load_to_ram( url );
    /**
     * if was success, pharse the json
     */
    if ( uri_load_dsc ) {
        SpiRamJsonDocument doc( uri_load_dsc->size * 3 );
        log_i("Http code: %d", httpcode);

        DeserializationError error = deserializeJson( doc, uri_load_dsc->data );
        if (error) {
            log_e("weather today deserializeJson() failed: %s (%d bytes)", error.c_str(), uri_load_dsc->size );
            doc.clear();
            uri_load_free_all( uri_load_dsc );
            weather_today->valide = false;
            return( httpcode );
        }

        if( doc.containsKey("cod") ) {
            if ( doc["cod"].as<uint32_t>() == 200 ) {
                weather_today->valide = true;
                snprintf( weather_today->temp, sizeof( weather_today->temp ), "%0.1f°%s", doc["main"]["temp"].as<float>(), weather_units_symbol);
                snprintf( weather_today->humidity, sizeof( weather_today->humidity ),"%.0f%%", doc["main"]["humidity"].as<float>() );
                snprintf( weather_today->pressure, sizeof( weather_today->pressure ),"%.0fhPa", doc["main"]["pressure"].as<float>() );
                strcpy( weather_today->icon, doc["weather"][0]["icon"] );
                strcpy( weather_today->description, doc["weather"][0]["description"] );
                strcpy( weather_today->name, doc["name"] );

                int directionDegree = doc["wind"]["deg"].as<int>();
                int speed = doc["wind"]["speed"].as<int>();
                weather_wind_to_string( weather_today, speed, directionDegree );
                weather_today->wind_deg = directionDegree;
                httpcode = 200;
            } else {
                weather_today->valide = false;
            }
        } else {
            weather_today->valide = false;
        }
        /**
         * clear json
         */
        doc.clear();
    }
    else {
        weather_today->valide = false;
        httpcode = -1;
    }
    /**
     * clear uri dsc
     */
    uri_load_free_all( uri_load_dsc );

    return( httpcode );
}

int weather_fetch_forecast( weather_config_t *weather_config, weather_forcast_t * weather_forecast, weather_forcast_t * hourly_forecast ) {
    char url[512]="";
    int httpcode = -1;
    const char* weather_units_symbol = weather_config->imperial ? "F" : "C";
    const char* weather_units_char = weather_config->imperial ? "imperial" : "metric";
    const char* weather_lang = weather_config->lang;
    /**
     * build uri string
     */
    snprintf( url, sizeof( url ), "http://%s/data/2.5/forecast?lat=%s&lon=%s&appid=%s&units=%s&lang=%s", 
        OWM_HOST, weather_config->lat, weather_config->lon, weather_config->apikey, weather_units_char, weather_lang);
    log_d("http get: %s", url );
    /**
     * load uri file into ram
     */
    uri_load_dsc_t *uri_load_dsc = uri_load_to_ram( url );
    /**
     * if was success, pharse the json
     */
    if ( uri_load_dsc ) {
        SpiRamJsonDocument doc( uri_load_dsc->size * 3 );

        DeserializationError error = deserializeJson( doc, uri_load_dsc->data );
        if (error) {
            log_e("weather forecast deserializeJson() failed: %s (%d bytes)", error.c_str(), uri_load_dsc->size );
            doc.clear();
            uri_load_free_all( uri_load_dsc );
            weather_forecast[0].valide = false;
            return( httpcode );
        }

        if( doc.containsKey("cod") ) {
            httpcode = doc["cod"].as<uint32_t>();
            if ( httpcode == 200 ) {
                for (int i = 0; i < MAX_FORECAST_HOURS; i++) {
                    hourly_forecast[i].timestamp = doc["list"][i]["dt"].as<long>() | 0;
                    strncpy( hourly_forecast[i].icon, doc["list"][i]["weather"][0]["icon"] | "n/a", sizeof(  hourly_forecast[i].icon ) );
                    hourly_forecast[ i ].pop = doc["list"][i]["pop"].as<float>() * 100.0f;
                    strncpy( hourly_forecast[i].description, doc["list"][i]["weather"][0]["description"] | "", sizeof(  hourly_forecast[i].description ) );
                    hourly_forecast[i].temp_min = doc["list"][i]["main"]["temp_min"].as<float>();
                    hourly_forecast[i].temp_max = doc["list"][i]["main"]["temp_max"].as<float>();
                }
                weather_forecast_raw_t daily_forecast[7];
                weather_forecast[0].valide = true;
                int hourly_forecast_count = doc["cnt"].as<uint32_t>();

                
                for (int i = 0; i < MAX_FORECAST_DAYS; i++) {
                    daily_forecast[i].min_temp = 200.0f;
                    daily_forecast[i].max_temp = -200.0f;
                    daily_forecast[i].max_wind_speed = 0.0f;
                    daily_forecast[i].weekday = -1;
                }
                char observed_time[20];
                int current_forecast_index = -1;
                for ( int i = 0 ; i < hourly_forecast_count; i++) {
                    time_t forecastTimeUtc = doc["list"][i]["dt"].as<time_t>();
                    struct tm *forecastLocalTime = localtime(&forecastTimeUtc);
                    uint8_t weekday = forecastLocalTime->tm_wday;

                    if (current_forecast_index < 0 || daily_forecast[current_forecast_index].weekday != weekday) {
                        current_forecast_index++;
                        daily_forecast[current_forecast_index].weekday = weekday;
                        daily_forecast[current_forecast_index].timestamp = doc["list"][i]["dt"].as<long>() | 0;
                    }
                    if (current_forecast_index >= MAX_FORECAST_DAYS) {
                        break;
                    }
                    // Looking for forecast close to noon
                    if ( i == 0 || abs(12 - forecastLocalTime->tm_hour) < 3) {
                        strncpy( daily_forecast[current_forecast_index].icon_at_noon, doc["list"][i]["weather"][0]["icon"] | "n/a", sizeof(  daily_forecast[current_forecast_index].icon_at_noon ) );
                        log_i("Found forecast close to noon: %d:00 with icon: %s", forecastLocalTime->tm_hour, daily_forecast[current_forecast_index].icon_at_noon);
                    }

                    float observed_min_temp = doc["list"][i]["main"]["temp_min"].as<float>();
                    float observed_max_temp = doc["list"][i]["main"]["temp_max"].as<float>();
                    float observed_wind_speed = doc["list"][i]["wind"]["speed"].as<float>();
                    
                    strncpy( observed_time, doc["list"][i]["dt_txt"], sizeof( observed_time ) );
                    log_i("Observed time: %s, min temp: %.1f, max temp %.1f", observed_time, observed_min_temp, observed_max_temp);
                    if (observed_min_temp < daily_forecast[current_forecast_index].min_temp) {
                        daily_forecast[current_forecast_index].min_temp = observed_min_temp;
                    }
                    if (observed_max_temp > daily_forecast[current_forecast_index].max_temp) {
                        daily_forecast[current_forecast_index].max_temp = observed_max_temp;
                    }
                    if (observed_wind_speed > daily_forecast[current_forecast_index].max_wind_speed) {
                        daily_forecast[current_forecast_index].max_wind_speed = observed_wind_speed;
                    }


                }
                for (int i = 0; i < MAX_FORECAST_DAYS; i++) {
                    log_i("Weekday: %d, min temp %.1f, max temp %.1f, max wind speed: %.1f", i, daily_forecast[i].min_temp, daily_forecast[i].max_temp, daily_forecast[i].max_wind_speed);
                    snprintf( weather_forecast[ i ].temp, sizeof( weather_forecast[ i ].temp ),"%0.0f|%0.0f°%s", daily_forecast[i].min_temp, daily_forecast[i].max_temp, weather_units_symbol );
                    snprintf( weather_forecast[ i ].wind_speed, sizeof( weather_forecast[ i ].wind_speed ),"%0.1f|%0.1f°%s", daily_forecast[i].min_temp, daily_forecast[i].max_temp, weather_units_symbol );
                    weather_forecast[ i ].timestamp = daily_forecast[i].timestamp;
                    strncpy( weather_forecast[ i ].icon, daily_forecast[i].icon_at_noon, sizeof(  weather_forecast[ i ].icon ) );
                }

                httpcode = 200;
            } else {
                weather_forecast[0].valide = false;
                log_e("Call failed with code: %d", httpcode);
            }
        } else {
            weather_forecast[0].valide = false;
        }
        /**
         * clear json
         */
        doc.clear();
    }
    else {
        weather_forecast[0].valide = false;
        httpcode = -1;
    }
    /**
     * clear uri dsc
     */
    uri_load_free_all( uri_load_dsc );
    
    return( httpcode );
}

int weather_fetch_onecall( weather_config_t *weather_config, weather_forcast_t * weather_today, weather_forcast_t * weather_forecast ) {

    char url[512]="";
    int httpcode = -1;
    const char* weather_units_symbol = weather_config->imperial ? "F" : "C";
    const char* weather_units_char = weather_config->imperial ? "imperial" : "metric";
    /**
     * build uri string
     */
    snprintf( url, sizeof( url ), "http://%s/data/3.0/onecall?lat=%s&lon=%s&appid=%s&units=%s&exclude=minutely,hourly", OWM_HOST, weather_config->lat, weather_config->lon, weather_config->apikey, weather_units_char);
    log_d("http get: %s", url );
    /**
     * load uri file into ram
     */
    uri_load_dsc_t *uri_load_dsc = uri_load_to_ram( url );

    /**
     * if was success, pharse the json
     */
    if ( uri_load_dsc ) {
        SpiRamJsonDocument doc( uri_load_dsc->size * 3 );

        DeserializationError error = deserializeJson( doc, uri_load_dsc->data );
        if (error) {
            log_e("weather forecast deserializeJson() failed: %s (%d bytes)", error.c_str(), uri_load_dsc->size );
            doc.clear();
            uri_load_free_all( uri_load_dsc );
            return( httpcode );
        }
        if( doc.containsKey("current")) {
            weather_today->valide = true;
            snprintf( weather_today->temp, sizeof( weather_today->temp ), "%0.1f°%s", doc["current"]["temp"].as<float>(), weather_units_symbol);
            snprintf( weather_today->humidity, sizeof( weather_today->humidity ),"%.0f%%", doc["current"]["humidity"].as<float>() );
            snprintf( weather_today->pressure, sizeof( weather_today->pressure ),"%.0fhPa", doc["current"]["pressure"].as<float>() );
            strcpy( weather_today->icon, doc["current"]["weather"][0]["icon"] );
            strcpy( weather_today->description, doc["current"]["weather"][0]["description"] );
            strcpy( weather_today->name, "" );

            int directionDegree = doc["current"]["wind_deg"].as<int>();
            int speed = doc["current"]["wind_speed"].as<int>();
            weather_wind_to_string( weather_today, speed, directionDegree );
            weather_today->wind_deg = directionDegree;
            httpcode = 200;
        }
        if( doc.containsKey("daily") ) {
            weather_forecast[0].valide = true;
            for ( int i = 0 ; i < MAX_FORECAST_DAYS ; i++ ) {
                weather_forecast[ i ].timestamp = doc["daily"][i]["dt"].as<long>() | 0;
                snprintf( weather_forecast[ i ].temp, sizeof( weather_forecast[ i ].temp ),"%.0f|%.0f°%s", doc["daily"][i]["temp"]["min"].as<float>(), doc["daily"][i]["temp"]["max"].as<float>(), weather_units_symbol );
                snprintf( weather_forecast[ i ].min_temp, sizeof( weather_forecast[ i ].min_temp ),"%.0f°%s", doc["daily"][i]["temp"]["min"].as<float>(), weather_units_symbol );
                snprintf( weather_forecast[ i ].max_temp, sizeof( weather_forecast[ i ].max_temp ),"%.0f°%s", doc["daily"][i]["temp"]["max"].as<float>(), weather_units_symbol );
                snprintf( weather_forecast[ i ].humidity, sizeof( weather_forecast[ i ].humidity ),"%f%%", doc["daily"][i]["humidity"].as<float>() );
                snprintf( weather_forecast[ i ].pressure, sizeof( weather_forecast[ i ].pressure ),"%hpa", doc["daily"][i]["pressure"].as<float>() );
                strncpy( weather_forecast[ i ].icon, doc["daily"][i]["weather"][0]["icon"] | "n/a", sizeof(  weather_forecast[ i ].icon ) );
                strncpy( weather_forecast[ i ].name, doc["city"]["name"] | "n/a", sizeof( weather_forecast[ i ].name ) );

                int directionDegree = doc["daily"][i]["wind_deg"].as<int>() | 0;
                int speed = doc["daily"][i]["wind_speed"].as<int>() | 0;
                weather_wind_to_string( &weather_forecast[i], speed, directionDegree );
            }
  
        }
        /**
         * clear json
         */
        doc.clear();
    }
    else {
        httpcode = -1;
    }
    /**
     * clear uri dsc
     */
    uri_load_free_all( uri_load_dsc );
    
    return( httpcode );
}

void weather_wind_to_string( weather_forcast_t* container, int speed, int degrees ) {
    // Normalize degrees to 0–360
    while (degrees < 0) degrees += 360;
    while (degrees >= 360) degrees -= 360;

    // Divide compass into 16 segments of 22.5 degrees each
    int index = (int)((degrees + 11.25f) / 22.5f) % 16;

    snprintf( container->wind_speed, sizeof(container->wind_speed), "%d %s", speed, get_string(WIND_DIRECTION_KEYS[index]));

}
