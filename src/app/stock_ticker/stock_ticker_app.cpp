/****************************************************************************
 *   June 04 02:01:00 2021
 *   Copyright  2021  Dirk Sarodnick
 *   Email: programmer@dirk-sarodnick.de
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

#include "stock_ticker_app.h"
#include "stock_ticker_app_main.h"
#include "stock_ticker_setup.h"

#include "gui/mainbar/mainbar.h"
#include "gui/statusbar.h"
#include "gui/app.h"
#include "gui/widget.h"

#ifdef NATIVE_64BIT
    #include "utils/logging.h"
    #include "utils/millis.h"
#else
    #include <Arduino.h>
    #include "esp_task_wdt.h"
#endif
/*
 * app tiles
 */
uint32_t stock_ticker_app_main_tile_num;
uint32_t stock_ticker_setup_tile_num;
/*
 * app icon
 */
icon_t *stock_ticker_app = NULL;

stock_ticker_config_t stock_ticker_config;

static void stock_ticker_update_task( lv_task_t * task );

lv_task_t * stock_ticker_tile_task = NULL;
/*
 * declare you images or fonts you need
 */
LV_IMG_DECLARE(stock_ticker_app_64px);
/*
 * automatic register the app setup function with explicit call in main.cpp
 */
static int registed = app_autocall_function( &stock_ticker_app_setup, 15 );           /** @brief app autocall function */
/*
 * setup routine for example app
 */
void stock_ticker_app_setup( void ) {
    /*
     * check if app already registered for autocall
     */
    if( !registed ) {
        return;
    }
    /*
     * register app
     */
    stock_ticker_app_main_tile_num = mainbar_add_app_tile( 2, 2, "st app" );
    stock_ticker_setup_tile_num = stock_ticker_app_main_tile_num + 1;
    stock_ticker_app = app_register( "Stock\nTicker", &stock_ticker_app_64px, enter_stock_ticker_app_event_cb );
    stock_ticker_load_config();
    stock_ticker_app_main_setup( stock_ticker_app_main_tile_num );
    stock_ticker_setup_tile_setup(stock_ticker_setup_tile_num);

}

static void stock_ticker_update_task( lv_task_t * task ) {
    static unsigned long last = 0;
   
    if( (millis() - last) > 4 * 60 * 1000 ) {
        stock_ticker_update_data();
        last = millis();
    }
}

/**
 * @brief Get the app main tile num object
 * 
 * @return uint32_t 
 */
uint32_t stock_ticker_app_get_app_main_tile_num( void ) {
    return( stock_ticker_app_main_tile_num );
}
/**
 * @brief call back function for enter app
 * 
 * @param obj           object
 * @param event         event
 */
void enter_stock_ticker_app_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       
            mainbar_jump_to_tilenumber( stock_ticker_app_main_tile_num, LV_ANIM_OFF, true );
            app_hide_indicator( stock_ticker_app );
            if( !stock_ticker_tile_task ) {
                stock_ticker_tile_task = lv_task_create( stock_ticker_update_task, 10000, LV_TASK_PRIO_MID, NULL );
            }
            break;
    }
}
/**
 * @brief call back function for exit app
 * 
 * @param obj           object
 * @param event         event
 */
void exit_stock_ticker_app_main_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       mainbar_jump_back();
                                        break;
    }
}

void stock_ticker_jump_to_setup( void ) {
    mainbar_jump_to_tilenumber( stock_ticker_setup_tile_num, LV_ANIM_ON, true );    
}

void stock_ticker_save_config( void ) {
    stock_ticker_config.save();
}

void stock_ticker_load_config( void ) {
    stock_ticker_config.load();
}

stock_ticker_config_t * get_stock_ticker_config() {
    return &stock_ticker_config;
}