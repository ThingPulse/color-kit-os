/****************************************************************************
 *   Tu May 22 21:23:51 2020
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
#ifndef _WEATHER_H
    #define _WEATHER_H

    #include "config/weather_config.h"

    #ifdef NATIVE_64BIT
        #include "utils/io.h"
        #include <time.h>
    #else
        #include <Arduino.h>
    #endif

    #define WEATHER_SYNC_REQUEST    _BV(0)

    typedef struct {
        bool valide = false;
        time_t timestamp = 0;
        char name[32] = "";
        char icon[16] = "";
        char description[32] = "";
        int wind_deg = 0;
        float pop = 0.0f;
        float temp = 0.0f;
        float temp_min = 0.0f;
        float temp_max = 0.0f;
        float humidity = 0.0f;
        float pressure = 0.0f;
        int wind_sp33d = 0.0f;
    } weather_forcast_t;

    typedef struct {
        int weekday = 0;
        float min_temp = 200.0f;
        float max_temp = -200.0f;
        float max_wind_speed = 0.0f;
        char icon_at_noon[16] = "";
        char description_at_noon[32] = "";
        long timestamp = 0;
    } weather_forecast_raw_t;

    void weather_app_setup( void );

    void weather_tile_setup( lv_obj_t *tile, lv_style_t *style, lv_coord_t hres, lv_coord_t vres );

    weather_config_t *weather_get_config( void );

    void weather_jump_to_forecast( void );

    void weather_jump_to_setup( void );

    void weather_jump_to_image( void );

    void weather_sync_request( void );

    void weather_save_config( void );

    void weather_load_config( void );
    
#endif // _WEATHER_H
