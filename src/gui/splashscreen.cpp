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

#include "splashscreen.h"
#include "hardware/display.h"
#include "hardware/framebuffer.h"
#include "gui/widget_factory.h"

#ifdef NATIVE_64BIT
    #include "utils/logging.h"
    #include "utils/delay.h"
#else
    #if defined( M5PAPER )
    #elif defined( M5CORE2 )
    #elif defined( LILYGO_WATCH_2020_V1 ) || defined( LILYGO_WATCH_2020_V2 ) || defined( LILYGO_WATCH_2020_V3 )
    #elif defined( LILYGO_WATCH_2021 )
    #elif defined( WT32_SC01 )
    #elif defined( CKGPRO ) || defined ( CKGRANDE )
    #else
        #error "not splashscreen pre hardware setup"
    #endif
#endif

lv_obj_t *logo = NULL;
lv_obj_t *preload = NULL;
lv_obj_t *preload_label = NULL;

LV_IMG_DECLARE(thingpulse_300px);

void splash_screen_stage_one( void ) {

    lv_img_cache_set_size(250);

    lv_obj_t *background = lv_bar_create(lv_scr_act());
    lv_obj_set_size( background, lv_disp_get_hor_res( lv_disp_get_default() ), lv_disp_get_ver_res( lv_disp_get_default() ) );
    lv_obj_add_style( background, BACKGROUND_STYLE, 0 );
    lv_obj_align( background, LV_ALIGN_CENTER, 0, 0 );

    logo = lv_img_create( background );

    // load boot logo from spiffs if exsist
    FILE* file;
    file = fopen( SPLASHSCREENLOGO, "rb" );

    if ( file ) {
        log_i("use custom boot logo from spiffs");
        fclose( file );
        lv_img_set_src( logo, SPLASHSCREENLOGO );
    }
    else {
        log_i("use default boot logo");
        lv_img_set_src( logo, &thingpulse_300px );
    }
    lv_obj_align( logo, LV_ALIGN_CENTER, 0, 0 );

    static lv_style_t style_preload;
    lv_style_init(&style_preload);
    lv_style_set_anim_time(&style_preload, 2000);

    preload = lv_bar_create( lv_scr_act() );
    lv_obj_set_size( preload, lv_disp_get_hor_res( lv_disp_get_default() ) - 80, 20 );
    lv_obj_add_style( preload, SYSTEM_ICON_STYLE, 0 );
    lv_obj_add_style(preload, &style_preload, LV_PART_INDICATOR);
    lv_obj_align_to( preload, logo, LV_ALIGN_OUT_BOTTOM_MID, 0, 30 );
    lv_bar_set_value( preload, 0, LV_ANIM_ON);
    lv_obj_add_flag( preload, LV_OBJ_FLAG_HIDDEN );

    preload_label = lv_label_create( lv_scr_act() );
    lv_label_set_text( preload_label, "booting" );
    lv_obj_add_style( preload_label, SYSTEM_ICON_LABEL_STYLE, 0 );
    lv_obj_align_to( preload_label, preload, LV_ALIGN_OUT_BOTTOM_MID, 0, 5 );

    lv_disp_trig_activity( lv_disp_get_default() );

    lv_obj_move_foreground( preload_label );

    lv_timer_handler();

    #ifdef NATIVE_64BIT
    #else
        #ifdef M5PAPER
            framebuffer_refresh();
        #elif defined( LILYGO_WATCH_2020_V1 ) || defined( LILYGO_WATCH_2020_V2 ) || defined( LILYGO_WATCH_2020_V3 )
            TTGOClass *ttgo = TTGOClass::getWatch();
            for( int bl = 0 ; bl < display_get_brightness() ; bl++ ) {
                ttgo->bl->adjust( bl );
                delay(5);
            }   
        #elif defined( LILYGO_WATCH_2021 )   
            for( int bl = 0 ; bl < display_get_brightness() ; bl++ ) {
                ledcWrite(0, bl );
                delay(5);
            }            
        #endif
    #endif
}

void splash_screen_stage_update( const char* msg, int value ) {
    lv_obj_move_foreground( preload );
    lv_disp_trig_activity( lv_disp_get_default() );
    lv_timer_handler();
    delay(100);
    lv_bar_set_value( preload, value, LV_ANIM_ON );
    lv_label_set_text( preload_label, msg );
    lv_obj_align_to( preload_label, preload, LV_ALIGN_OUT_BOTTOM_MID, 0, 5 );
    lv_timer_handler();
    delay(500);
}

void splash_screen_stage_finish( void ) {
    #ifdef NATIVE_64BIT
    #else
        #ifdef M5PAPER
        #elif defined( LILYGO_WATCH_2020_V1 ) || defined( LILYGO_WATCH_2020_V2 ) || defined( LILYGO_WATCH_2020_V3 )
            TTGOClass *ttgo = TTGOClass::getWatch();

            for( int bl = display_get_brightness() ; bl >= 0 ; bl-- ) {
                ttgo->bl->adjust( bl );
                delay(5);
            }
        #elif defined( LILYGO_WATCH_2021 )   
            for( int bl = display_get_brightness() ; bl >= 0 ; bl-- ) {
                ledcWrite(0, bl );
                delay(5);
            }   
        #endif
    #endif
    lv_obj_del( logo );
    lv_obj_del( preload );
    lv_obj_del( preload_label );
    lv_timer_handler();
}