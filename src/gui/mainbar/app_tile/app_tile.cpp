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
#include "gui/mainbar/mainbar.h"
#include "gui/widget_factory.h"
#include "gui/mainbar/app_tile/app_tile.h"
#include "gui/mainbar/main_tile/main_tile.h"
#include "gui/mainbar/note_tile/note_tile.h"
#include "gui/mainbar/setup_tile/setup_tile.h"

#include "utils/alloc.h"

#ifdef NATIVE_64BIT
    #include "utils/logging.h"
#else
    #include <Arduino.h>
#endif

static bool apptile_init = false;

icon_t *app_entry = NULL;
lv_obj_t *app_cont[ MAX_APPS_TILES ];
uint32_t app_tile_num[ MAX_APPS_TILES ];

static bool app_tile_button_event_cb( EventBits_t event, void *arg );

void app_tile_setup( void ) {
    /*
     * check if maintile alread initialized
     */
    if ( apptile_init ) {
        log_e("apptile already initialized");
        return;
    }
    app_entry = (icon_t*)MALLOC_ASSERT( sizeof( icon_t ) * MAX_APPS_ICON, "error while app_entry alloc" );
    /**
     * add tiles to to main tile
     */
    for ( int tiles = 0 ; tiles < MAX_APPS_TILES ; tiles++ ) {
    #if defined( M5PAPER )
        app_tile_num[ tiles ] = mainbar_add_tile( 0, 1 + tiles, "app tile", ws_get_mainbar_style() );
    #elif defined( LILYGO_WATCH_2020_V1 ) || defined( LILYGO_WATCH_2020_V2 ) || defined( LILYGO_WATCH_2020_V3 ) || defined( M5CORE2 )
        app_tile_num[ tiles ] = mainbar_add_tile( 1 + tiles, 0, "app tile", ws_get_mainbar_style() );
    #elif defined( LILYGO_WATCH_2021 )
        app_tile_num[ tiles ] = mainbar_add_tile( 1 + tiles, 0, "app tile", ws_get_mainbar_style() );
    #elif defined ( CKGPRO ) || defined ( CKGRANDE )
        app_tile_num[ tiles ] = mainbar_add_tile( 1 + tiles, 0, "app tile", ws_get_mainbar_style() );
    #else
        app_tile_num[ tiles ] = mainbar_add_tile( 1 + tiles, 0, "app tile", ws_get_mainbar_style() );
        #warning "not app tile setup"
    #endif
        app_cont[ tiles ] = mainbar_get_tile_obj( app_tile_num[ tiles ] );
        mainbar_add_tile_button_cb( app_tile_num[ tiles ], app_tile_button_event_cb );
    }
    /**
     * init all app icons
     */
    for ( int app = 0 ; app < MAX_APPS_ICON ; app++ ) {
        /*
         * set x, y and mark it as inactive
         */
        app_entry[ app ].x = APP_FIRST_X_POS + ( ( app % MAX_APPS_ICON_HORZ ) * ( APP_ICON_X_SIZE + APP_ICON_X_CLEARENCE ) ) + APP_ICON_X_OFFSET;
        app_entry[ app ].y = APP_FIRST_Y_POS + ( ( ( app % ( MAX_APPS_ICON_VERT * MAX_APPS_ICON_HORZ  ) ) / MAX_APPS_ICON_HORZ ) * ( APP_ICON_Y_SIZE + APP_ICON_Y_CLEARENCE ) ) + APP_ICON_Y_OFFSET;
        app_entry[ app ].active = false;
        /*
         * create app icon container
         */
        app_entry[ app ].icon_cont = lv_obj_create( app_cont[ app / ( MAX_APPS_ICON_HORZ * MAX_APPS_ICON_VERT ) ], NULL );
        mainbar_add_slide_element( app_entry[ app ].icon_cont);
        lv_obj_reset_style_list( app_entry[ app ].icon_cont, LV_OBJ_PART_MAIN );
        lv_obj_add_style( app_entry[ app ].icon_cont, LV_OBJ_PART_MAIN, APP_ICON_STYLE );
        lv_obj_set_size( app_entry[ app ].icon_cont, APP_ICON_X_SIZE, APP_ICON_Y_SIZE );
        lv_obj_align( app_entry[ app ].icon_cont , app_cont[ app / ( MAX_APPS_ICON_HORZ * MAX_APPS_ICON_VERT ) ], LV_ALIGN_IN_TOP_LEFT, app_entry[ app ].x, app_entry[ app ].y );
        /*
         * create app label
         */
        app_entry[ app ].label = lv_label_create( app_cont[ app / ( MAX_APPS_ICON_HORZ * MAX_APPS_ICON_VERT ) ], NULL );
        mainbar_add_slide_element(app_entry[ app ].label);
        lv_obj_reset_style_list( app_entry[ app ].label, LV_OBJ_PART_MAIN );
        lv_obj_add_style( app_entry[ app ].label, LV_OBJ_PART_MAIN, APP_ICON_LABEL_STYLE );
        lv_obj_set_size( app_entry[ app ].label, APP_LABEL_X_SIZE, APP_LABEL_Y_SIZE );
        lv_obj_align( app_entry[ app ].label , app_entry[ app ].icon_cont, LV_ALIGN_OUT_BOTTOM_MID, 3, 0 );
        lv_obj_set_hidden( app_entry[ app ].icon_cont, true );
        lv_obj_set_hidden( app_entry[ app ].label, true );

        //log_d("icon screen/x/y: %d/%d/%d", app / ( MAX_APPS_ICON_HORZ * MAX_APPS_ICON_VERT ), app_entry[ app ].x, app_entry[ app ].y );
    }

    apptile_init = true;
}

static bool app_tile_button_event_cb( EventBits_t event, void *arg ) {
    switch( event ) {
        case BUTTON_LEFT:   mainbar_jump_to_tilenumber( main_tile_get_tile_num(), LV_ANIM_ON );
                            mainbar_clear_history();
                            break;
        case BUTTON_RIGHT:  mainbar_jump_to_tilenumber( setup_get_tile_num(), LV_ANIM_ON );
                            mainbar_clear_history();
                            break;
        case BUTTON_EXIT:   mainbar_jump_to_maintile( LV_ANIM_ON );
                            break;
    }
    return( true );
}

lv_obj_t *app_tile_register_app( const char* appname ) {
    /*
     * check if apptile alread initialized
     */
    if ( !apptile_init ) {
        log_e("apptile not initialized");
        while( true );
    }
    /**
     * search for the next free app icon and use them
     */
    for( int app = 0 ; app < MAX_APPS_ICON ; app++ ) {
        if ( app_entry[ app ].active == false ) {
            app_entry[ app ].active = true;
            lv_label_set_text( app_entry[ app ].label, appname );
            lv_obj_align( app_entry[ app ].label , app_entry[ app ].icon_cont, LV_ALIGN_OUT_BOTTOM_MID, 0, 0 );
            lv_obj_set_hidden( app_entry[ app ].icon_cont, false );
            lv_obj_set_hidden( app_entry[ app ].label, false );
            return( app_entry[ app ].icon_cont );
        }
    }
    log_e("no space for an app icon");
    return( NULL );
}

icon_t *app_tile_get_free_app_icon( void ) {
    /*
     * check if apptile alread initialized
     */
    if ( !apptile_init ) {
        log_e("apptile not initialized");
        while( true );
    }
    /**
     * search for the next free app icon
     */
    for( int app = 0 ; app < MAX_APPS_ICON ; app++ ) {
        if ( app_entry[ app ].active == false ) {
            return( &app_entry[ app ] );
        }
    }
    log_e("no space for an app icon");
    return( NULL );
}

uint32_t app_tile_get_tile_num( void ) {
    /*
     * check if apptile alread initialized
     */
    if ( !apptile_init ) {
        log_e("apptile not initialized");
        while( true );
    }

    return( app_tile_num[ 0 ] );
}

int32_t app_tile_get_active_app_entrys( void ) {
    /*
     * check if apptile alread initialized
     */
    if ( !apptile_init ) {
        log_e("apptile not initialized");
        while( true );
    }
    
    int32_t _appentry = 0;
    
    for( int app = 0 ; app < MAX_APPS_ICON ; app++ ) {
        if ( app_entry[ app ].active == true ) {
            _appentry++;
        }
    }
    return( _appentry ); 
}

const char *app_get_appentrys_name( int32_t appentry ) {
    /*
     * check if apptile alread initialized
     */
    if ( !apptile_init ) {
        log_e("apptile not initialized");
        while( true );
    }
    
    int32_t _appentry = 0;
    const char *appname = NULL;
        
    for( int app = 0 ; app < MAX_APPS_ICON ; app++ ) {
        if ( app_entry[ app ].active == true ) {
            _appentry++;
            if ( _appentry == appentry ) {
                appname = (const char*)lv_label_get_text( app_entry[ app ].label );
            }
        }
    }
    return( appname ); 
}