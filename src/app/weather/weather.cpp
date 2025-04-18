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
#include "config.h"
#include "weather.h"
#include "weather_fetch.h"
#include "weather_forecast.h"
#include "weather_detail.h"
#include "weather_setup.h"
#include "images/resolve_owm_icon.h"
#include "gui/gui.h"
#include "gui/app.h"
#include "gui/mainbar/mainbar.h"
#include "gui/statusbar.h"
#include "gui/keyboard.h"
#include "gui/widget.h"
#include "hardware/wifictl.h"
#include "utils/json_psram_allocator.h"
#include "hardware/timesync.h"

#ifdef NATIVE_64BIT
    #include <iostream>
    #include <fstream>
    #include <string.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <pwd.h>
    #include "utils/logging.h"

    using namespace std;
    #define String string
#else
    #include <Arduino.h>

    EventGroupHandle_t weather_sync_event_handle = NULL;
    TaskHandle_t _weather_sync_Task;
#endif
/*
 * app config
 */
weather_config_t weather_config;
weather_forcast_t weather_today;
/*
 * app tiles
 */
uint32_t weather_app_tile_num;
uint32_t weather_app_setup_tile_num;
uint32_t weather_app_image_tile_num;
/*
 * app icon
 */
icon_t *weather_app = NULL;

/*
 * declare callback functions for the app and widget icon to enter the app
 */
bool weather_wifictl_event_cb( EventBits_t event, void *arg );
void weather_widget_sync( void );
void weather_sync_Task( void * pvParameters );
/*
 * declare you images or fonts you need
 */
LV_IMG_DECLARE(owm01d_64px);
LV_IMG_DECLARE(img_weather_fog_64px);
LV_IMG_DECLARE(info_ok_16px);
LV_IMG_DECLARE(info_fail_16px);
LV_FONT_DECLARE(Ubuntu_16px);
/*
 * automatic register the app setup function with explicit call in main.cpp
 */
static int registed = app_autocall_function( &weather_app_setup, 0 );           /** @brief app autocall function */


void weather_app_setup( void ) {

    if( !registed ) {
        return;
    }
    

    weather_config.load();

    // get an app tile and copy mainstyle
    weather_app_tile_num = mainbar_add_app_tile( 2, 2, "Weather App" );
    weather_app_setup_tile_num = weather_app_tile_num + 1;
    weather_app_image_tile_num = weather_app_tile_num + 2;

    // init forecast and setup tile
    weather_forecast_tile_setup( weather_app_tile_num );
    weather_setup_tile_setup( weather_app_setup_tile_num );
    weather_image_tile_setup( weather_app_image_tile_num) ;

    weather_app = app_register( "weather", &owm01d_64px, enter_weather_widget_event_cb );  

    weather_widget_setup();




#ifdef NATIVE_64BIT

#else
    weather_sync_event_handle = xEventGroupCreate();
#endif
    weather_sync_request();

    wifictl_register_cb( WIFICTL_OFF | WIFICTL_CONNECT, weather_wifictl_event_cb, "weather" );

}

bool weather_wifictl_event_cb( EventBits_t event, void *arg ) {
    log_i("received weather_wifictl_event_cb");
    switch( event ) {
        case WIFICTL_CONNECT:       if ( weather_config.autosync ) {
                                        weather_sync_request();
                                    }
                                    break;
        case WIFICTL_OFF:           //widget_hide_indicator( weather_widget );
                                    break;
    }
    return( true );
}




void weather_jump_to_forecast( void ) {
    mainbar_jump_to_tilenumber( weather_app_tile_num, LV_ANIM_ON, true );
}

void weather_jump_to_setup( void ) {
    mainbar_jump_to_tilenumber( weather_app_setup_tile_num, LV_ANIM_ON, true );    
}

void weather_jump_to_image( void ) {
    mainbar_jump_to_tilenumber( weather_app_image_tile_num, LV_ANIM_ON, true );    
}

void weather_sync_request( void ) {
#ifdef NATIVE_64BIT
    weather_sync_Task( NULL );
#else
    if ( xEventGroupGetBits( weather_sync_event_handle ) & WEATHER_SYNC_REQUEST ) {
        return;
    } else {
        xEventGroupSetBits( weather_sync_event_handle, WEATHER_SYNC_REQUEST );
        //widget_hide_indicator( weather_widget );
        xTaskCreate(    weather_sync_Task,              /* Function to implement the task */
                        "weather sync Task",            /* Name of the task */
                        5000,                           /* Stack size in words */
                        NULL,                           /* Task input parameter */
                        1,                              /* Priority of the task */
                        &_weather_sync_Task );          /* Task handle. */
    }
#endif
}

weather_config_t *weather_get_config( void ) {
    return( &weather_config );
}

void weather_sync_Task( void * pvParameters ) {
#ifndef NATIVE_64BIT
    log_i("start weather widget task, heap: %d", ESP.getFreeHeap() );
    vTaskDelay( 250 );
    if ( xEventGroupGetBits( weather_sync_event_handle ) & WEATHER_SYNC_REQUEST ) {       
#endif
    log_i("Updating weather");

    weather_forecast_sync();

#ifndef NATIVE_64BIT
    }
    xEventGroupClearBits( weather_sync_event_handle, WEATHER_SYNC_REQUEST );
    log_i("finish weather widget task, heap: %d", ESP.getFreeHeap() );
    vTaskDelete( NULL );
#endif
}

void weather_save_config( void ) {
    weather_config.save();
}

void weather_load_config( void ) {
    weather_config.load();
}

void enter_weather_widget_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       mainbar_jump_to_tilenumber( weather_app_tile_num, LV_ANIM_OFF, true );
                                        break;
    }    
}



