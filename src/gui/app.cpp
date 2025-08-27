/****************************************************************************
 *   Sep 2 08:21:51 2020
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
#include "app.h"
#include "gui/mainbar/mainbar.h"
#include "gui/mainbar/app_tile/app_tile.h"
#include "utils/alloc.h"

#ifdef NATIVE_64BIT
    #include "utils/logging.h"
#else
    #include <Arduino.h>
#endif

size_t app_autocall_counter = 0;                   /** @brief counter for registered setup functions */
app_autocall_table_t *app_autocall_table = NULL;  /** @brief table for registered setup functions */

/**
 * @brief register a app with prio
 * 
 * @param function      pointer to a function
 * @param prio          priority of the function, 0 first, 1 second, ...
 * @return int 
 */
int app_autocall_function( APP_AUTOCALL_FUNC function, size_t prio ) {
    /**
     * register a setup function
     */
    app_autocall_counter++;
    /**
     * alloc or realloc the table for a new entry
     */
    if( !app_autocall_table ) 
        app_autocall_table = (app_autocall_table_t *)malloc( sizeof( app_autocall_table_t ) );
    else
        app_autocall_table = (app_autocall_table_t *)realloc( app_autocall_table, ( app_autocall_counter ) * sizeof( app_autocall_table_t ) );
    /**
     * store registration function
     */
    app_autocall_table[ app_autocall_counter - 1 ].function = function;
    app_autocall_table[ app_autocall_counter - 1 ].prio = prio;
    
    return( 1 );
}

void app_autocall_all_setup_functions( void ) {
    /**
     * start core servies
     */
    if( app_autocall_counter && app_autocall_table ) {
        /**
         * call all registered setup functions
         */
        for( size_t prio = 0 ; prio < 32 ; prio++ ) {
            for( size_t i = 0 ; i < app_autocall_counter ; i++ ) {
                if( app_autocall_table[ i ].prio == prio )
                    app_autocall_table[ i ].function();
            }
        }
        /**
         * free the table
         */
        app_autocall_counter = 0;
        free( app_autocall_table );
        app_autocall_table = NULL;
    }
}

icon_t *app_register( const char* appname, const lv_img_dsc_t *icon, lv_event_cb_t event_cb ) {

    icon_t *app = app_tile_get_free_app_icon();

    if ( app == NULL ) {
        log_e("no free app icon");
        return( NULL );
    }

    app->active = true;
    // setup label
    lv_label_set_text( app->label, appname );
    lv_obj_align( app->label , LV_ALIGN_OUT_BOTTOM_MID, 0, 0 );
    lv_obj_set_style_text_align( app->label, LV_TEXT_ALIGN_CENTER, 0 );
    lv_obj_clear_flag( app->icon_cont, LV_OBJ_FLAG_HIDDEN );
    lv_obj_clear_flag( app->label, LV_OBJ_FLAG_HIDDEN );
    // setup icon and set event callback
    app->icon_img = lv_imgbtn_create( app->icon_cont );
    lv_imgbtn_set_src( app->icon_img, LV_IMGBTN_STATE_RELEASED, icon, NULL, NULL);
    lv_imgbtn_set_src( app->icon_img, LV_IMGBTN_STATE_PRESSED, icon, NULL, NULL);
    lv_imgbtn_set_src( app->icon_img, LV_IMGBTN_STATE_CHECKED_RELEASED, icon, NULL, NULL);
    lv_imgbtn_set_src( app->icon_img, LV_IMGBTN_STATE_CHECKED_PRESSED, icon, NULL, NULL);
    
    lv_obj_align( app->icon_img , LV_ALIGN_CENTER, 0, 0 );
    lv_obj_add_event_cb( app->icon_img, event_cb, LV_EVENT_ALL, NULL );
    // setup icon indicator
    app->icon_indicator = lv_img_create( app->icon_cont );
    lv_img_set_src( app->icon_indicator, &info_ok_16px );
    lv_obj_align( app->icon_indicator, LV_ALIGN_TOP_RIGHT, 0, 0 );
    lv_obj_add_flag( app->icon_indicator, LV_OBJ_FLAG_HIDDEN );
    mainbar_add_slide_element( app->icon_img );
    
    lv_obj_invalidate( lv_scr_act() );

    return( app );
}

void app_set_indicator( icon_t *app, icon_indicator_t indicator ) {
    if ( app == NULL ) {
        return;
    }

    if ( app->active == false ) {
        return;
    }

    switch( indicator ) {
        case ICON_INDICATOR_OK:     lv_img_set_src( app->icon_indicator, &info_ok_16px );
                                    break;
        case ICON_INDICATOR_FAIL:   lv_img_set_src( app->icon_indicator, &info_fail_16px );
                                    break;
        case ICON_INDICATOR_UPDATE: lv_img_set_src( app->icon_indicator, &info_update_16px );
                                    break;
        case ICON_INDICATOR_1:      lv_img_set_src( app->icon_indicator, &info_1_16px );
                                    break;
        case ICON_INDICATOR_2:      lv_img_set_src( app->icon_indicator, &info_2_16px );
                                    break;
        case ICON_INDICATOR_3:      lv_img_set_src( app->icon_indicator, &info_3_16px );
                                    break;
        case ICON_INDICATOR_N:      lv_img_set_src( app->icon_indicator, &info_n_16px );
                                    break;
    }
    lv_obj_align( app->icon_indicator, LV_ALIGN_TOP_RIGHT, 0, 0 );
    lv_obj_clear_flag( app->icon_indicator, LV_OBJ_FLAG_HIDDEN );
    lv_obj_invalidate( lv_scr_act() );
}

void app_hide_indicator( icon_t *app ) {
    if ( app == NULL ) {
        return;
    }

    if ( app->active == false ) {
        return;
    }

    lv_obj_add_flag( app->icon_indicator, LV_OBJ_FLAG_HIDDEN );
    lv_obj_invalidate( lv_scr_act() );
}

void app_set_icon( icon_t *app, lv_obj_t *icon ) {
    if ( app == NULL ) {
        return;
    }

    if ( app->active == false ) {
        return;
    }

    lv_imgbtn_set_src( app->icon_img, LV_IMGBTN_STATE_RELEASED, icon, NULL, NULL);
    lv_imgbtn_set_src( app->icon_img, LV_IMGBTN_STATE_PRESSED, icon, NULL, NULL);
    lv_imgbtn_set_src( app->icon_img, LV_IMGBTN_STATE_CHECKED_RELEASED, icon, NULL, NULL);
    lv_imgbtn_set_src( app->icon_img, LV_IMGBTN_STATE_CHECKED_PRESSED, icon, NULL, NULL);
    
    lv_obj_align( app->icon_img, LV_ALIGN_TOP_LEFT, 0, 0 );
    lv_obj_invalidate( lv_scr_act() );
}

int32_t app_get_active_app_entrys( void ) {
    return( app_tile_get_active_app_entrys() );
}