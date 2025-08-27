/****************************************************************************
 *   Su Jan 17 23:05:51 2021
 *   Copyright  2021  Dirk Brosswick
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

#include "quickbar.h"
#include "screenshot.h"
#include "gui/mainbar/mainbar.h"
#include "gui/mainbar/setup_tile/setup_tile.h"
#include "gui/widget_factory.h"
#include "gui/widget_styles.h"

#include "hardware/blectl.h"
#include "hardware/wifictl.h"
#include "hardware/button.h"
#include "hardware/powermgm.h"
#include "hardware/motor.h"
#include "hardware/timesync.h"

#ifdef NATIVE_64BIT
    #include "utils/logging.h"
#else
    #include <Arduino.h>
#endif

#define THEME_PADDING 5
#define THEME_ICON_PADDING 5

static bool quickbar_init = false;
static bool wifistate = false;
static bool bluetoothstate = false;

static lv_obj_t *quickbar = NULL;
static lv_obj_t *quickbar_time_label = NULL;
static lv_obj_t *quickbar_maintile_btn = NULL;
static lv_obj_t *quickbar_setup_btn = NULL;
static lv_obj_t *quickbar_screenshot_btn = NULL;
static lv_obj_t *quickbar_bluetooth_btn = NULL;
static lv_obj_t *quickbar_wifi_btn = NULL;
static lv_style_t quickbarstyle[ QUICKBAR_STYLE_NUM ];

LV_IMG_DECLARE(maintile_32px);
LV_IMG_DECLARE(setup_32px);
LV_IMG_DECLARE(camera_32px);
LV_IMG_DECLARE(bluetooth_64px);
LV_IMG_DECLARE(wifi_64px);
LV_FONT_DECLARE(Ubuntu_48px);

lv_timer_t * quickbar_task;
static uint32_t quickbar_counter = 0;

/**
 * quickbar icon events
 */
static void quickbar_maintile_event_cb( lv_event_t * e );
static void quickbar_setup_event_cb( lv_event_t * e );
static void quickbar_screenshot_event_cb( lv_event_t * e );
static void quickbar_wifi_event_cb( lv_event_t * e );
static void quickbar_bluetooth_event_cb( lv_event_t * e );
/**
 * quickbar external events
 */
static bool quickbar_blectl_event_cb( EventBits_t event, void *arg );
static bool quickbar_wifictl_event_cb( EventBits_t event, void *arg );
static bool quickbar_button_event_cb( EventBits_t event, void *arg );
static bool quickbar_powermgm_event_cb( EventBits_t event, void *arg );
static void quickbar_counter_task( lv_timer_t * timer );

static void set_zoom_anim(void * var, int32_t v)
{
    lv_img_set_zoom((lv_obj_t*)var, v);
}

static void quickbar_image_button_fade_out_state_2( lv_anim_t *anim ) {
    lv_obj_add_flag( (lv_obj_t*)anim->var, LV_OBJ_FLAG_HIDDEN );
}

static void quickbar_image_button_fade_out( lv_obj_t *button, uint32_t duration, uint32_t delay ) {
    lv_anim_t wf_btn_icon_anim;

    lv_anim_init( &wf_btn_icon_anim );
	lv_anim_set_exec_cb( &wf_btn_icon_anim, set_zoom_anim );
	lv_anim_set_time( &wf_btn_icon_anim, duration );
    lv_anim_set_ready_cb( &wf_btn_icon_anim, quickbar_image_button_fade_out_state_2 );

    lv_anim_set_var( &wf_btn_icon_anim, lv_obj_get_child( button, 0 ) );
    lv_anim_set_values( &wf_btn_icon_anim, 256, 1 );
    lv_anim_set_delay( &wf_btn_icon_anim, delay );
    lv_anim_start( &wf_btn_icon_anim );
}

static void quickbar_image_button_fade_in_state_2( lv_anim_t *anim ) {
    lv_anim_set_exec_cb( anim, set_zoom_anim );
	lv_anim_set_time( anim, 100 );
    lv_anim_set_values( anim, 300, 256 );
    lv_anim_set_delay( anim, 0 );
    lv_anim_start( anim );
}

static void quickbar_image_button_fade_in( lv_obj_t *button, uint32_t duration, uint32_t delay ) {
    lv_anim_t wf_btn_icon_anim;

    lv_anim_init( &wf_btn_icon_anim );
	lv_anim_set_exec_cb( &wf_btn_icon_anim, set_zoom_anim );
	lv_anim_set_time( &wf_btn_icon_anim, duration );
    lv_anim_set_ready_cb( &wf_btn_icon_anim, quickbar_image_button_fade_in_state_2 );

    lv_anim_set_var( &wf_btn_icon_anim, lv_obj_get_child( button, 0 ) );
    lv_anim_set_values( &wf_btn_icon_anim, 1, 300 );
    lv_anim_set_delay( &wf_btn_icon_anim, delay );
    lv_anim_start( &wf_btn_icon_anim );

    lv_obj_clear_flag( button, LV_OBJ_FLAG_HIDDEN );
}

static lv_obj_t * quickbar_add_image_button(lv_obj_t *parent, const lv_img_dsc_t * image, lv_event_cb_t event_cb, lv_style_t *style){
    lv_obj_t *button = lv_btn_create( parent );
    lv_obj_add_style( button, ws_get_img_button_style(), 0 );
    lv_obj_set_width( button, image->header.w + THEME_PADDING * 2 );
    lv_obj_set_height( button, image->header.h + THEME_PADDING * 2 );

    lv_obj_t *button_img = lv_img_create( button );
    lv_img_set_src( button_img, image );
    lv_obj_align( button_img, LV_ALIGN_CENTER, 0, 0 );
    lv_obj_clear_flag( button_img, LV_OBJ_FLAG_CLICKABLE );

    if (style) {
        lv_obj_add_style( button_img, style, 0 );
    }

    if (event_cb != NULL) {
        lv_obj_add_event_cb( button, event_cb, LV_EVENT_CLICKED, NULL );
    }
    return button;
}

void quickbar_setup( void ){
    /*
     * check if quickbar already initialized
     */
    if ( quickbar_init ) {
        log_e("quickbar already initialized");
        return;
    }

    /*Copy a built-in style to initialize the new style*/
    lv_style_init( &quickbarstyle[ QUICKBAR_STYLE_NORMAL ] );
    lv_style_set_text_font( &quickbarstyle[ QUICKBAR_STYLE_NORMAL ], &Ubuntu_48px);
    lv_style_set_radius( &quickbarstyle[ QUICKBAR_STYLE_NORMAL ], 0 );
    lv_style_set_bg_color( &quickbarstyle[ QUICKBAR_STYLE_NORMAL ], lv_color_white() );
    lv_style_set_bg_opa( &quickbarstyle[ QUICKBAR_STYLE_NORMAL ], LV_OPA_80 );
    lv_style_set_border_width( &quickbarstyle[ QUICKBAR_STYLE_NORMAL ], 0 );
    lv_style_set_text_color( &quickbarstyle[ QUICKBAR_STYLE_NORMAL ], lv_color_white() );
    lv_style_set_radius( &quickbarstyle[ QUICKBAR_STYLE_NORMAL ], 16 );
    lv_style_set_img_recolor( &quickbarstyle[ QUICKBAR_STYLE_NORMAL ], lv_color_white() );

    lv_style_init( &quickbarstyle[ QUICKBAR_STYLE_LIGHT ] );
    lv_style_set_bg_color( &quickbarstyle[ QUICKBAR_STYLE_LIGHT ], lv_color_white() );
    lv_style_set_bg_opa( &quickbarstyle[ QUICKBAR_STYLE_LIGHT ], LV_OPA_0 );
    lv_style_set_text_color( &quickbarstyle[ QUICKBAR_STYLE_LIGHT ], lv_color_white() );
    lv_style_set_img_recolor( &quickbarstyle[ QUICKBAR_STYLE_LIGHT ], lv_color_white() );
    lv_style_set_img_recolor_opa( &quickbarstyle[ QUICKBAR_STYLE_LIGHT ], LV_OPA_COVER );

    lv_style_init( &quickbarstyle[ QUICKBAR_STYLE_DARK ] );
    lv_style_set_bg_color( &quickbarstyle[ QUICKBAR_STYLE_DARK ], lv_color_black() );
    lv_style_set_text_color( &quickbarstyle[ QUICKBAR_STYLE_DARK ], lv_color_black() );
    lv_style_set_img_recolor( &quickbarstyle[ QUICKBAR_STYLE_DARK ], lv_color_black() );
    lv_style_set_img_recolor_opa( &quickbarstyle[ QUICKBAR_STYLE_DARK ], LV_OPA_COVER );

    lv_style_init( &quickbarstyle[ QUICKBAR_STYLE_GREEN ] );
    lv_style_set_bg_color( &quickbarstyle[ QUICKBAR_STYLE_GREEN ], lv_color_black() );
    lv_style_set_text_color( &quickbarstyle[ QUICKBAR_STYLE_GREEN ], lv_color_black() );
    lv_style_set_img_recolor( &quickbarstyle[ QUICKBAR_STYLE_GREEN ], lv_palette_main(LV_PALETTE_GREEN) );
    lv_style_set_img_recolor_opa( &quickbarstyle[ QUICKBAR_STYLE_GREEN ], LV_OPA_COVER );

    lv_style_init( &quickbarstyle[ QUICKBAR_STYLE_RED ] );
    lv_style_set_bg_color( &quickbarstyle[ QUICKBAR_STYLE_RED ], lv_color_black() );
    lv_style_set_text_color( &quickbarstyle[ QUICKBAR_STYLE_RED ], lv_color_black() );
    lv_style_set_img_recolor( &quickbarstyle[ QUICKBAR_STYLE_RED ], lv_palette_main(LV_PALETTE_RED) );
    lv_style_set_img_recolor_opa( &quickbarstyle[ QUICKBAR_STYLE_RED ], LV_OPA_COVER );

    lv_style_init( &quickbarstyle[ QUICKBAR_STYLE_GRAY ] );
    lv_style_set_bg_color( &quickbarstyle[ QUICKBAR_STYLE_GRAY ], lv_color_black() );
    lv_style_set_text_color( &quickbarstyle[ QUICKBAR_STYLE_GRAY ], lv_color_black() );
    lv_style_set_img_recolor( &quickbarstyle[ QUICKBAR_STYLE_GRAY ], lv_palette_main(LV_PALETTE_GREY) );
    lv_style_set_img_recolor_opa( &quickbarstyle[ QUICKBAR_STYLE_GRAY ], LV_OPA_COVER );

    lv_style_init( &quickbarstyle[ QUICKBAR_STYLE_TRANS ] );
    lv_style_set_bg_color( &quickbarstyle[ QUICKBAR_STYLE_TRANS ], lv_color_black() );
    lv_style_set_bg_opa( &quickbarstyle[ QUICKBAR_STYLE_TRANS ], LV_OPA_TRANSP );
    lv_style_set_text_color( &quickbarstyle[ QUICKBAR_STYLE_TRANS ], lv_color_black() );
    lv_style_set_img_recolor_opa( &quickbarstyle[ QUICKBAR_STYLE_TRANS ], LV_OPA_COVER );

    quickbar = lv_obj_create( lv_scr_act() );
    lv_obj_set_width( quickbar, 48 * 3 );
    lv_obj_remove_style_all( quickbar );
    lv_obj_add_style( quickbar, &quickbarstyle[ QUICKBAR_STYLE_NORMAL ], 0 );
    
    #if defined( ROUND_DISPLAY )
        lv_obj_set_height( quickbar, 48 * 2 + 64 );
        lv_obj_align( quickbar, LV_ALIGN_CENTER, 0, 0 );
    #else
        lv_obj_set_height( quickbar, 48 * 2 );
        lv_obj_align( quickbar, LV_ALIGN_BOTTOM_MID, 0, 0 );
    #endif

    quickbar_time_label = lv_label_create( quickbar );
    lv_label_set_text( quickbar_time_label, "00:00");
    lv_obj_remove_style_all( quickbar_time_label );
    lv_obj_add_style( quickbar_time_label, &quickbarstyle[ QUICKBAR_STYLE_DARK ], 0  );
    lv_obj_align( quickbar_time_label, LV_ALIGN_TOP_MID, 0, 0);

    quickbar_setup_btn = quickbar_add_image_button( quickbar, &setup_32px, quickbar_setup_event_cb, &quickbarstyle[ QUICKBAR_STYLE_DARK ] );
    lv_obj_set_width( quickbar_setup_btn, 48 );
    lv_obj_set_height( quickbar_setup_btn, 48 );
    lv_obj_align_to( quickbar_setup_btn, quickbar_time_label, LV_ALIGN_OUT_BOTTOM_MID, 0, -8 );

    quickbar_maintile_btn = quickbar_add_image_button( quickbar, &maintile_32px, quickbar_maintile_event_cb, &quickbarstyle[ QUICKBAR_STYLE_DARK ] );
    lv_obj_set_width( quickbar_maintile_btn, 48 );
    lv_obj_set_height( quickbar_maintile_btn, 48 );
    lv_obj_align_to( quickbar_maintile_btn, quickbar_setup_btn, LV_ALIGN_OUT_LEFT_MID, 0, 0 );

    quickbar_screenshot_btn = quickbar_add_image_button( quickbar, &camera_32px, quickbar_screenshot_event_cb, &quickbarstyle[ QUICKBAR_STYLE_DARK ] );
    lv_obj_set_width( quickbar_screenshot_btn, 48 );
    lv_obj_set_height( quickbar_screenshot_btn, 48 );
    lv_obj_align_to( quickbar_screenshot_btn, quickbar_setup_btn, LV_ALIGN_OUT_RIGHT_MID, 0, 0 );
    
    quickbar_wifi_btn = quickbar_add_image_button( quickbar, &wifi_64px, quickbar_wifi_event_cb, &quickbarstyle[ QUICKBAR_STYLE_RED ] );
    lv_obj_set_width( quickbar_wifi_btn, wifi_64px.header.w + THEME_ICON_PADDING);
    lv_obj_set_height( quickbar_wifi_btn, wifi_64px.header.h + THEME_ICON_PADDING );
    lv_obj_align_to( quickbar_wifi_btn, quickbar, LV_ALIGN_BOTTOM_LEFT, 0, 0 );

    quickbar_bluetooth_btn = quickbar_add_image_button( quickbar, &bluetooth_64px, quickbar_bluetooth_event_cb, &quickbarstyle[ QUICKBAR_STYLE_RED ] );
    lv_obj_set_width( quickbar_bluetooth_btn, bluetooth_64px.header.w + THEME_ICON_PADDING );
    lv_obj_set_height( quickbar_bluetooth_btn, bluetooth_64px.header.h + THEME_ICON_PADDING );
    lv_obj_align_to( quickbar_bluetooth_btn, quickbar, LV_ALIGN_BOTTOM_RIGHT, 0, 0 );
    /*
     * quickbar init complete
     */
    quickbar_init = true;
    quickbar_hide( true );
    #ifndef ROUND_DISPLAY
        lv_obj_add_flag( quickbar_wifi_btn, LV_OBJ_FLAG_HIDDEN );
        lv_obj_add_flag( quickbar_bluetooth_btn, LV_OBJ_FLAG_HIDDEN );
    #endif
    /*
     * register pmu callback to detect long press and powermgm callback
     */
    blectl_register_cb( BLECTL_CONNECT | BLECTL_DISCONNECT | BLECTL_ON | BLECTL_OFF, quickbar_blectl_event_cb, "quickbar bluetooth" );
    wifictl_register_cb( WIFICTL_CONNECT | WIFICTL_DISCONNECT | WIFICTL_OFF | WIFICTL_ON, quickbar_wifictl_event_cb, "quickbar wifi" );
    button_register_cb( BUTTON_QUICKBAR, quickbar_button_event_cb, "quickbar pmu event");
    powermgm_register_cb( POWERMGM_SILENCE_WAKEUP | POWERMGM_STANDBY | POWERMGM_WAKEUP, quickbar_powermgm_event_cb, "quickbar powermgm event" );

    return;
}

static bool quickbar_blectl_event_cb( EventBits_t event, void *arg ) {
    switch( event ) {
        case BLECTL_ON:
            bluetoothstate = true;
            lv_obj_remove_style_all( lv_obj_get_child( quickbar_bluetooth_btn, 0 ) );
            lv_obj_add_style( lv_obj_get_child( quickbar_bluetooth_btn, 0 ), &quickbarstyle[ QUICKBAR_STYLE_GRAY ], 0 );
            break;
        case BLECTL_OFF:
            bluetoothstate = false;
            lv_obj_remove_style_all( lv_obj_get_child( quickbar_bluetooth_btn, 0 ) );
            lv_obj_add_style( lv_obj_get_child( quickbar_bluetooth_btn, 0 ), &quickbarstyle[ QUICKBAR_STYLE_RED ], 0 );
            break;
        case BLECTL_CONNECT:
            if( bluetoothstate ){
                lv_obj_remove_style_all( lv_obj_get_child( quickbar_bluetooth_btn, 0 ) );
                lv_obj_add_style( lv_obj_get_child( quickbar_bluetooth_btn, 0 ), &quickbarstyle[ QUICKBAR_STYLE_GREEN ], 0 );
            }
            break;
        case BLECTL_DISCONNECT:
            if( bluetoothstate ){
                lv_obj_remove_style_all( lv_obj_get_child( quickbar_bluetooth_btn, 0 ) );
                lv_obj_add_style( lv_obj_get_child( quickbar_bluetooth_btn, 0 ), &quickbarstyle[ QUICKBAR_STYLE_GRAY ], 0 );
            }
            break;
    }
    return( true );
}
static bool quickbar_wifictl_event_cb( EventBits_t event, void *arg ) {
    switch( event ) {
        case WIFICTL_ON:
            wifistate = true;
            lv_obj_remove_style_all( lv_obj_get_child( quickbar_wifi_btn, 0 ) );
            lv_obj_add_style( lv_obj_get_child( quickbar_wifi_btn, 0 ), &quickbarstyle[ QUICKBAR_STYLE_GRAY ], 0 );
            break;
        case WIFICTL_OFF:
            wifistate = false;
            lv_obj_remove_style_all( lv_obj_get_child( quickbar_wifi_btn, 0 ) );
            lv_obj_add_style( lv_obj_get_child( quickbar_wifi_btn, 0 ), &quickbarstyle[ QUICKBAR_STYLE_RED ], 0 );
            break;
        case WIFICTL_CONNECT:
            if( wifistate ){
                lv_obj_remove_style_all( lv_obj_get_child( quickbar_wifi_btn, 0 ) );
                lv_obj_add_style( lv_obj_get_child( quickbar_wifi_btn, 0 ), &quickbarstyle[ QUICKBAR_STYLE_GREEN ], 0 );
            }
            break;
        case WIFICTL_DISCONNECT:
            if( wifistate ){
                lv_obj_remove_style_all( lv_obj_get_child( quickbar_wifi_btn, 0 ) );
                lv_obj_add_style( lv_obj_get_child( quickbar_wifi_btn, 0 ), &quickbarstyle[ QUICKBAR_STYLE_GRAY ], 0 );
            }
            break;
    }
    return( true );
}

bool quickbar_powermgm_event_cb( EventBits_t event, void *arg ) {
    /*
     * check if quickar already initialized
     */
    if ( !quickbar_init ) {
        log_e("quickbar not initialized");
        return( true );
    }

    bool retval = true;
    
    switch (event) {
        case POWERMGM_WAKEUP:
            quickbar_hide( true );
            break;
        case POWERMGM_STANDBY:
            quickbar_hide( true );
            break;
        case POWERMGM_SILENCE_WAKEUP:
            quickbar_hide( true );
            break;
    }
    return( retval );
}

static bool quickbar_button_event_cb( EventBits_t event, void *arg ) {
    /*
     * check if quickar already initialized
     */
    if ( !quickbar_init ) {
        log_e("quickbar not initialized");
        return( true );
    }
    
    bool retval = true;
    char time[32] = "";
    
    switch ( event ) {
        case BUTTON_QUICKBAR:
            motor_vibe(3);
            lv_disp_trig_activity( lv_disp_get_default() );
            if( lv_obj_has_flag( quickbar, LV_OBJ_FLAG_HIDDEN ) ) {
                timesync_get_current_timestring( time, sizeof( time ) );
                lv_label_set_text( quickbar_time_label, time );
                lv_obj_align( quickbar_time_label, LV_ALIGN_TOP_MID, 0, 0);
                quickbar_hide( false );
            }
            else {
                quickbar_hide( true );
            }
            break;
    }
    return( retval );
}

void quickbar_hide( bool hide ) {
    /*
     * check if quickar already initialized
     */
    if ( !quickbar_init ) {
        log_e("quickbar not initialized");
        return;
    }

    if ( hide ) {
        lv_obj_add_flag( quickbar, LV_OBJ_FLAG_HIDDEN );
        lv_obj_invalidate( lv_scr_act() );

        quickbar_image_button_fade_out( quickbar_maintile_btn, 300, 0 );
        quickbar_image_button_fade_out( quickbar_setup_btn, 300, 0 );
        quickbar_image_button_fade_out( quickbar_screenshot_btn, 300, 0 );
        #if defined( ROUND_DISPLAY )
            quickbar_image_button_fade_out( quickbar_wifi_btn, 300, 0 );
            quickbar_image_button_fade_out( quickbar_bluetooth_btn, 300, 0 );
        #endif
    }
    else {
        lv_obj_clear_flag( quickbar, LV_OBJ_FLAG_HIDDEN );
        lv_obj_invalidate( lv_scr_act() );

        quickbar_image_button_fade_in( quickbar_maintile_btn, 300, 0 );
        quickbar_image_button_fade_in( quickbar_setup_btn, 300, 0 );
        quickbar_image_button_fade_in( quickbar_screenshot_btn, 300, 0 );
        #if defined( ROUND_DISPLAY )
            quickbar_image_button_fade_in( quickbar_wifi_btn, 300, 0 );
            quickbar_image_button_fade_in( quickbar_bluetooth_btn, 300, 0 );
        #endif
    }
}

static void quickbar_maintile_event_cb( lv_event_t * e ) {
    /*
     * check if quickar already initialized
     */
    if ( !quickbar_init ) {
        log_e("quickbar not initialized");
        return;
    }
    
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        quickbar_hide( true );
        mainbar_jump_to_maintile( LV_ANIM_OFF );
    }
}

static void quickbar_setup_event_cb( lv_event_t * e ) {
    /*
     * check if quickar already initialized
     */
    if ( !quickbar_init ) {
        log_e("quickbar not initialized");
        return;
    }
    
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        quickbar_hide( true );
        mainbar_jump_to_tilenumber( setup_get_tile_num(), LV_ANIM_OFF );
    }
}

static void quickbar_screenshot_event_cb( lv_event_t * e ) {
    /*
     * check if quickar already initialized
     */
    if ( !quickbar_init ) {
        log_e("quickbar not initialized");
        return;
    }
    
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        quickbar_hide( true );
        lv_disp_trig_activity( lv_disp_get_default() );
        lv_timer_handler();
        quickbar_counter = 3;
        quickbar_task = lv_timer_create( quickbar_counter_task, 1000, NULL );
    }
}

static void quickbar_wifi_event_cb( lv_event_t * e ) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED) {
        if( wifistate ) {
            wifictl_off();
            lv_obj_remove_style_all( lv_obj_get_child( obj, 0 ) );
            lv_obj_add_style( lv_obj_get_child( obj, 0 ), &quickbarstyle[ QUICKBAR_STYLE_RED ], 0 );
        }
        else {
            wifictl_on();
            lv_obj_remove_style_all( lv_obj_get_child( obj, 0 ) );
            lv_obj_add_style( lv_obj_get_child( obj, 0 ), &quickbarstyle[ QUICKBAR_STYLE_GRAY ], 0 );
        }
    }
}

static void quickbar_bluetooth_event_cb( lv_event_t * e ) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED) {
        if( bluetoothstate ) {
            blectl_off();
            lv_obj_remove_style_all( lv_obj_get_child( obj, 0 ) );
            lv_obj_add_style( lv_obj_get_child( obj, 0 ), &quickbarstyle[ QUICKBAR_STYLE_RED ], 0 );
        }
        else {
            blectl_on();
            lv_obj_remove_style_all( lv_obj_get_child( obj, 0 ) );
            lv_obj_add_style( lv_obj_get_child( obj, 0 ), &quickbarstyle[ QUICKBAR_STYLE_GRAY ], 0 );
        }
    }
}

static void quickbar_counter_task( lv_timer_t * timer ) {
    /*
     * check if quickar already initialized
     */
    if ( !quickbar_init ) {
        log_e("quickbar not initialized");
        return;
    }
    
    quickbar_counter--;
    if ( quickbar_counter == 0 ) {
        screenshot_take();
        screenshot_save();
        lv_timer_del( quickbar_task );
    }
}