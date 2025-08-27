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
#include <stdio.h>
#include "config.h"

#include "mainbar.h"
#include "main_tile/main_tile.h"
#include "setup_tile/setup_tile.h"
#include "note_tile/note_tile.h"
#include "app_tile/app_tile.h"
#include "gui/keyboard.h"
#include "gui/statusbar.h"
#include "gui/widget_styles.h"
#include "gui/gui.h"

#include "hardware/display.h"
#include "hardware/powermgm.h"
#include "hardware/rtcctl.h"
#include "hardware/button.h"

#include "utils/alloc.h"

#ifdef NATIVE_64BIT
    #include "utils/logging.h"
#else
    #include <Arduino.h>
#endif

mainbar_history_t *mainbar_history = NULL;

static lv_obj_t *mainbar = NULL;

static lv_tile_t *tile = NULL;
static uint32_t tile_entrys = 0;
static uint32_t app_tile_x_pos = MAINBAR_APP_TILE_X_START;
static uint32_t app_tile_y_pos = MAINBAR_APP_TILE_Y_START;
static volatile bool mainbar_alarm_occurred = false;

bool mainbar_button_event_cb( EventBits_t event, void *arg );
bool mainbar_powermgm_event_cb( EventBits_t event, void *arg );
bool mainbar_rtcctl_event_cb( EventBits_t event, void *arg );
void mainbar_add_current_tile_to_history( void );

static uint32_t get_tile_act_id(lv_obj_t* tileview) {
    lv_obj_t* act_tile = lv_tileview_get_tile_act(tileview);
    for(uint32_t i = 0; i < tile_entrys; i++) {
        if(tile[i].tile == act_tile) {
            return i;
        }
    }
    return 0;
}

void mainbar_setup( void ) {
    /*
     * check if mainbar already initialized
     */
    if ( mainbar ) {
        MAINBAR_ERROR_LOG("main already initialized");
        return;
    }

    mainbar_history = (mainbar_history_t*)MALLOC_ASSERT( sizeof( mainbar_history_t ), "error while alloc" );

    mainbar = lv_tileview_create( lv_scr_act());
    lv_obj_set_scrollbar_mode(mainbar, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style( mainbar, ws_get_mainbar_style(), 0 );
    powermgm_register_cb_with_prio( POWERMGM_STANDBY, mainbar_powermgm_event_cb, "mainbar powermgm", CALL_CB_FIRST );
    powermgm_register_cb_with_prio( POWERMGM_WAKEUP | POWERMGM_SILENCE_WAKEUP, mainbar_powermgm_event_cb, "mainbar powermgm", CALL_CB_LAST );
    rtcctl_register_cb( RTCCTL_ALARM_OCCURRED, mainbar_rtcctl_event_cb, "mainbar rtcctl" );
    button_register_cb( BUTTON_UP | BUTTON_RIGHT | BUTTON_LEFT | BUTTON_DOWN | BUTTON_ENTER | BUTTON_EXIT | BUTTON_REFRESH | BUTTON_SETUP | BUTTON_MENU | BUTTON_KEYBOARD, mainbar_button_event_cb, "mainbay button event" );

    mainbar_clear_history();
}

bool mainbar_button_event_cb( EventBits_t event, void *arg ) {
    uint32_t current_tile = -1;
    /*
     * check if mainbar already initialized
     */
    ASSERT( mainbar, "main not initialized" );
    /**
     * get the current tile number
     */
    current_tile = get_tile_act_id(mainbar);
    /**
     * call button callback for the current tile if exist
     */
    if ( current_tile != -1 ) {
        if ( tile[ current_tile ].button_cb != NULL ) {
            MAINBAR_INFO_LOG("call button cb for tile: %d", current_tile );
            tile[ current_tile ].button_cb( event, arg );
        }
        else {
            MAINBAR_INFO_LOG("no button cb for current tile: %d", current_tile );
            if ( event == BUTTON_EXIT ) {
                mainbar_jump_back();
            }
        }
    }
    /**
     * trigger activity
     */
    lv_disp_trig_activity( lv_disp_get_default() );
    
    return( true );
}

void mainbar_add_current_tile_to_history( lv_anim_enable_t anim ) {
    /*
     * check if mainbar already initialized
     */
    ASSERT( mainbar, "main not initialized" );

    if ( mainbar_history->entrys < MAINBAR_MAX_HISTORY ) {
        /**
         * get current tile
         */
        uint32_t act_tile_id = get_tile_act_id(mainbar);
        /**
         * store tile pos in history
         */
        mainbar_history->entrys++;
        mainbar_history->tile[ mainbar_history->entrys ].x = tile[act_tile_id].x;
        mainbar_history->tile[ mainbar_history->entrys ].y = tile[act_tile_id].y;
        mainbar_history->statusbar[ mainbar_history->entrys ] = statusbar_get_hidden_state();
        mainbar_history->anim[ mainbar_history->entrys ] = anim;
        mainbar_history->powermgm_state[ mainbar_history->entrys ] = powermgm_get_event( POWERMGM_SILENCE_WAKEUP | POWERMGM_STANDBY | POWERMGM_WAKEUP );
        MAINBAR_INFO_LOG("store tile to history: %d, %d, %d, %x, %d", tile[act_tile_id].x, tile[act_tile_id].y, statusbar_get_hidden_state(), powermgm_get_event( POWERMGM_SILENCE_WAKEUP | POWERMGM_STANDBY | POWERMGM_WAKEUP ), anim );
    }
}

void mainbar_clear_history( void ) {
    /*
     * check if mainbar already initialized
     */
    ASSERT( mainbar, "main not initialized" );

    mainbar_history->entrys = 0;
    mainbar_history->tile[ 0 ].x = 0;
    mainbar_history->tile[ 0 ].y = 0;
    mainbar_history->statusbar[ 0 ] = true;
    mainbar_history->powermgm_state[ 0 ] = 0;
    MAINBAR_INFO_LOG("clear mainbar history");
}

void mainbar_jump_back( void ) {
    /*
     * check if mainbar already initialized
     */
    ASSERT( mainbar, "main not initialized" );

    if ( mainbar_history->entrys > 0 && mainbar_history->entrys < MAINBAR_MAX_HISTORY ) {
        /**
         * get the current tile pos for later use
         */
        uint32_t current_tile = get_tile_act_id(mainbar);
        /**
         * jump back
         */
        MAINBAR_INFO_LOG("jump back to tile: %d, %d, %d", mainbar_history->tile[ mainbar_history->entrys ].x, mainbar_history->tile[ mainbar_history->entrys ].y, mainbar_history->statusbar[ mainbar_history->entrys ] );
        lv_obj_t* target_tile = NULL;
        for ( int i = 0 ; i < tile_entrys; i++ ) {
            if ( tile[i].x == mainbar_history->tile[ mainbar_history->entrys ].x && tile[i].y == mainbar_history->tile[ mainbar_history->entrys ].y ) {
                target_tile = tile[i].tile;
                break;
            }
        }
        if (target_tile) {
            lv_obj_scroll_to_view_recursive(target_tile, mainbar_history->anim[ mainbar_history->entrys ]);
        }

        statusbar_hide( mainbar_history->statusbar[ mainbar_history->entrys ] );
        gui_force_redraw( true );
        /**
         * restore powermgm state
         */
        switch( mainbar_history->powermgm_state[ mainbar_history->entrys ] ) {
            case( POWERMGM_STANDBY ):
                powermgm_set_event( POWERMGM_STANDBY_REQUEST );
                MAINBAR_INFO_LOG("mainbar send standby request");
                break;
            case( POWERMGM_WAKEUP ):
                powermgm_set_event( POWERMGM_WAKEUP_REQUEST );
                MAINBAR_INFO_LOG("mainbar send wakeup request");
                break;
            case( POWERMGM_SILENCE_WAKEUP ):
                powermgm_set_event( POWERMGM_SILENCE_WAKEUP_REQUEST );
                MAINBAR_INFO_LOG("mainbar send silence wakeup request");
                break;
            default:
                break;
        }
        /**
         * search for the hibernate cb
         */
        if ( tile[ current_tile ].hibernate_cb != NULL ) {
            for( int i = 0 ; i < tile[ current_tile ].hibernate_cb_entry_count ; i++ ) {
                if ( tile[ current_tile ].hibernate_cb[ i ] != NULL ) {
                    MAINBAR_INFO_LOG("call hibernate cb [%d] for tile: %d", i, current_tile );                            
                    tile[ current_tile ].hibernate_cb[ i ]();
                }
            }
        }
        /**
         * search for the activation cb
         */
        uint32_t new_tile_id = 0;
        for ( int i = 0 ; i < tile_entrys; i++ ) {
            if ( tile[i].x == mainbar_history->tile[ mainbar_history->entrys ].x && tile[i].y == mainbar_history->tile[ mainbar_history->entrys ].y ) {
                new_tile_id = i;
                break;
            }
        }
        if ( tile[ new_tile_id ].activate_cb != NULL ) {
            for( int i = 0 ; i < tile[ new_tile_id ].activate_cb_entry_count ; i++ ) {
                if ( tile[ new_tile_id ].activate_cb[ i ] != NULL ) {
                    MAINBAR_INFO_LOG("call activation cb [%d] for tile: %d", i, new_tile_id );
                    tile[ new_tile_id ].activate_cb[ i ]();
                }
            }
        }
        mainbar_history->entrys--;
    }
    else {
        mainbar_jump_to_maintile( LV_ANIM_OFF );
    }
}

bool mainbar_rtcctl_event_cb( EventBits_t event, void *arg ) {
    switch( event ) {
        case RTCCTL_ALARM_OCCURRED:
            mainbar_alarm_occurred = true;
            break;
    }
    return( true );
}

bool mainbar_powermgm_event_cb( EventBits_t event, void *arg ) {
    switch( event ) {
        case POWERMGM_STANDBY:
            if ( !mainbar_alarm_occurred ) {
                if ( !display_get_block_return_maintile() ) {
                    mainbar_jump_to_maintile( LV_ANIM_OFF );
                }
            }
            break;
        case POWERMGM_SILENCE_WAKEUP:
            mainbar_alarm_occurred = false;
            break;
        case POWERMGM_WAKEUP:
            break;
    }
    return( true );
}

uint32_t mainbar_add_tile( uint16_t x, uint16_t y, const char *id, lv_style_t *style ) {
    /*
     * check if mainbar already initialized
     */
    ASSERT( mainbar, "main not initialized" );

    tile_entrys++;

    if ( tile == NULL ) {
        tile = ( lv_tile_t * )MALLOC_ASSERT( sizeof( lv_tile_t ) * tile_entrys, "tile malloc faild" );
    }
    else {
        tile = ( lv_tile_t * )REALLOC_ASSERT( tile, sizeof( lv_tile_t ) * tile_entrys, "tile realloc faild" );
    }

    lv_obj_t *my_tile = lv_tileview_add_tile( mainbar, x, y, LV_ANIM_OFF );
    tile[ tile_entrys - 1 ].tile = my_tile;
    tile[ tile_entrys - 1 ].activate_cb_entry_count = 0;
    tile[ tile_entrys - 1 ].activate_cb = NULL;
    tile[ tile_entrys - 1 ].hibernate_cb_entry_count = 0;
    tile[ tile_entrys - 1 ].hibernate_cb = NULL;
    tile[ tile_entrys - 1 ].button_cb = NULL;
    tile[ tile_entrys - 1 ].x = x;
    tile[ tile_entrys - 1 ].y = y;
    tile[ tile_entrys - 1 ].id = id;
    lv_obj_add_style( tile[ tile_entrys - 1 ].tile, style, 0 );
    MAINBAR_INFO_LOG("add tile: x=%d, y=%d, id=%s", x, y, id );

    return( tile_entrys - 1 );
}

bool mainbar_add_tile_hibernate_cb( uint32_t tile_number, MAINBAR_CALLBACK_FUNC hibernate_cb ) {
    bool retval = false;
    /*
     * check if mainbar already initialized
     */
    ASSERT( mainbar, "main not initialized" );

    if ( tile_number < tile_entrys ) {
        tile[ tile_number ].hibernate_cb_entry_count++;

        if( tile[ tile_number ].hibernate_cb )
            tile[ tile_number ].hibernate_cb = (MAINBAR_CALLBACK_FUNC*)REALLOC_ASSERT( tile[ tile_number ].hibernate_cb, sizeof( MAINBAR_CALLBACK_FUNC* ) * tile[ tile_number ].hibernate_cb_entry_count, "maintile hibernate_cb reallocation failed" );
        else
            tile[ tile_number ].hibernate_cb = (MAINBAR_CALLBACK_FUNC*)MALLOC_ASSERT( sizeof( MAINBAR_CALLBACK_FUNC* ), "maintile hibernate_cb allocation failed" );

        tile[ tile_number ].hibernate_cb[ tile[ tile_number ].hibernate_cb_entry_count - 1 ] = hibernate_cb;
        
        retval = true;
    }
    else
        MAINBAR_ERROR_LOG("tile number %d do not exist", tile_number );

    return( retval );
}

bool mainbar_add_tile_activate_cb( uint32_t tile_number, MAINBAR_CALLBACK_FUNC activate_cb ) {
    bool retval = false;
    /*
     * check if mainbar already initialized
     */
    ASSERT( mainbar, "main not initialized" );
        
    if ( tile_number < tile_entrys ) {
        tile[ tile_number ].activate_cb_entry_count++;

        if( tile[ tile_number ].activate_cb )
            tile[ tile_number ].activate_cb = (MAINBAR_CALLBACK_FUNC*)REALLOC_ASSERT( tile[ tile_number ].activate_cb, sizeof(MAINBAR_CALLBACK_FUNC*) * tile[ tile_number ].activate_cb_entry_count, "maintile hibernate_cb reallocation failed" );
        else
            tile[ tile_number ].activate_cb = (MAINBAR_CALLBACK_FUNC*)MALLOC_ASSERT(sizeof(MAINBAR_CALLBACK_FUNC*), "maintile hibernate_cb allocation failed" );

        tile[ tile_number ].activate_cb[ tile[ tile_number ].activate_cb_entry_count - 1 ] = activate_cb;

        retval = true;
    }
    else
        MAINBAR_ERROR_LOG("tile number %d do not exist", tile_number );

    return( retval );
}

bool mainbar_add_tile_button_cb( uint32_t tile_number, CALLBACK_FUNC button_cb ) {
    bool retval = false;
    /*
     * check if mainbar already initialized
     */
    ASSERT( mainbar, "main not initialized" );

    if ( tile_number < tile_entrys ) {
        tile[ tile_number ].button_cb = button_cb;
        retval = true;
    }
    else
        MAINBAR_ERROR_LOG("tile number %d do not exist", tile_number );

    return( retval );
}

uint32_t mainbar_add_app_tile( uint16_t x, uint16_t y, const char *id ) {
    uint32_t retval = -1;
    /*
     * check if mainbar already initialized
     */
    ASSERT( mainbar, "main not initialized" );
    /*
     * prevent tile x pos goes out of range ( uint16_t )
     */
    if( ( app_tile_x_pos + x ) * lv_disp_get_hor_res( lv_disp_get_default() ) > 32000 ) {
        MAINBAR_INFO_LOG("max horz resolution, jump next vert line");
        app_tile_x_pos = 0;
        app_tile_y_pos = app_tile_y_pos + MAINBAR_APP_TILE_Y_START;
    }
    /**
     * crawl tiles
     */
    for ( int hor = 0 ; hor < x ; hor++ ) {
        for ( int ver = 0 ; ver < y ; ver++ ) {
            if ( retval == -1 )
                retval = mainbar_add_tile( hor + app_tile_x_pos, app_tile_y_pos + ver + MAINBAR_APP_TILE_Y_START, id, ws_get_app_style() );
            else
                mainbar_add_tile( hor + app_tile_x_pos, app_tile_y_pos + ver + MAINBAR_APP_TILE_Y_START, id, ws_get_app_style() );
        }
    }

    app_tile_x_pos = app_tile_x_pos + x + 1;

    return( retval );
}

uint32_t mainbar_add_setup_tile( uint16_t x, uint16_t y, const char *id ) {
    uint32_t retval = -1;
    /*
     * check if mainbar already initialized
     */
    ASSERT( mainbar, "main not initialized" );
    /*
     * prevent tile x pos goes out of range ( uint16_t )
     */
    if( ( app_tile_x_pos + x ) * lv_disp_get_hor_res( lv_disp_get_default() ) > 32000 ) {
        MAINBAR_INFO_LOG("max horz resolution, jump next vert line");
        app_tile_x_pos = 0;
        app_tile_y_pos = app_tile_y_pos + MAINBAR_APP_TILE_Y_START;
    }
    /**
     * crawl tiles
     */
    for ( int hor = 0 ; hor < x ; hor++ ) {
        for ( int ver = 0 ; ver < y ; ver++ ) {
            if ( retval == -1 )
                retval = mainbar_add_tile( hor + app_tile_x_pos, app_tile_y_pos + ver + MAINBAR_APP_TILE_Y_START, id, ws_get_setup_tile_style() );
            else
                mainbar_add_tile( hor + app_tile_x_pos, app_tile_y_pos + ver + MAINBAR_APP_TILE_Y_START, id, ws_get_setup_tile_style() );
        }
    }

    app_tile_x_pos = app_tile_x_pos + x + 1;

    return( retval );
}

lv_obj_t *mainbar_get_tile_obj( uint32_t tile_number ) {
    /*
     * check if mainbar already initialized
     */
    ASSERT( mainbar, "main not initialized" );

    if ( tile_number < tile_entrys )
        return( tile[ tile_number ].tile );
    else
        MAINBAR_ERROR_LOG( "tile number %d do not exist", tile_number );

    return( NULL );
}

void mainbar_jump_to_maintile( lv_anim_enable_t anim ) {
    /*
     * check if mainbar already initialized
     */
    ASSERT( mainbar, "main not initialized" );

    if ( tile_entrys != 0 ) {
        mainbar_jump_to_tilenumber( 0, anim );
        keyboard_hide();
        statusbar_hide( false );
        statusbar_expand( false );
        mainbar_clear_history();
    }
    else {
        MAINBAR_ERROR_LOG( "main tile do not exist" );
    }
}

void mainbar_jump_to_tilenumber( uint32_t tile_number, lv_anim_enable_t anim, bool statusbar ) {
    uint32_t current_tile = 0;
    /*
     * check if mainbar already initialized
     */
    ASSERT( mainbar, "main not initialized" );
    /**
     * get the current tile number
     */
    current_tile = get_tile_act_id(mainbar);
    /**
     * check if tile alread in mainbar history to prevent loops
     */
    if( current_tile == tile_number ) {
        MAINBAR_INFO_LOG("the destination tile is the current tile");
        return;
    }

    for ( int i = 0 ; i < mainbar_history->entrys; i++ ) {
        if ( tile[i].x == tile[current_tile].x && tile[i].y == tile[current_tile].y ) {
            MAINBAR_INFO_LOG("current tile already in mainbar_history");
            return;
        }
    }
    /**
     * jump
     */
    if ( tile_number < tile_entrys ) {
        /**
         * store current tile and statusbar state
         */
        mainbar_add_current_tile_to_history( anim );
        statusbar_hide( statusbar );
        /**
         * jump into tile
         */
        MAINBAR_INFO_LOG("jump to tile %d from tile %d", tile_number, current_tile );
        lv_obj_scroll_to_view_recursive(tile[tile_number].tile, anim);
        gui_force_redraw( true );
        /**
         * call hibernate callback for the current tile if exist
         */
        if ( tile[ current_tile ].hibernate_cb != NULL ) {
            for( int i = 0 ; i < tile[ current_tile ].hibernate_cb_entry_count ; i++ ) {
                if ( tile[ current_tile ].hibernate_cb[ i ] != NULL ) {
                    MAINBAR_INFO_LOG("call hibernate cb [%d] for tile: %d", i, tile_number );
                    tile[ current_tile ].hibernate_cb[ i ]();
                }
            }
        }
        /**
         * call activate callback for the new tile if exist
         */
        if ( tile[ tile_number ].activate_cb != NULL ) {
            for( int i = 0 ; i < tile[ tile_number ].activate_cb_entry_count ; i++ ) {
                if ( tile[ tile_number ].activate_cb[ i ] != NULL ) {
                    MAINBAR_INFO_LOG("call activation cb [%d] for tile: %d", i, tile_number );
                    tile[ tile_number ].activate_cb[ i ]();
                }
            }
        }
    }
    else {
        MAINBAR_ERROR_LOG( "tile number %d do not exist", tile_number );
    }    
}

void mainbar_jump_to_tilenumber( uint32_t tile_number, lv_anim_enable_t anim ) {
    mainbar_jump_to_tilenumber( tile_number, anim, statusbar_get_hidden_state() );
}

lv_obj_t * mainbar_obj_create(lv_obj_t *parent) {
    /*
     * check if mainbar already initialized
     */
    ASSERT( mainbar, "main not initialized" );

    lv_obj_t * child = lv_obj_create( parent );

    return child;
}

void mainbar_add_slide_element(lv_obj_t *element) {
    /*
     * check if mainbar already initialized
     */
    ASSERT( mainbar, "mainbar not initialized" );
}