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
#include <stdio.h>

#include "config.h"
#include <time.h>
#include "gui.h"
#include "string.h"

#include "statusbar.h"

#include "hardware/powermgm.h"
#include "hardware/wifictl.h"
#include "hardware/blectl.h"
#include "hardware/rtcctl.h"
#include "hardware/motion.h"
#include "hardware/pmu.h"
#include "hardware/sound.h"
#include "hardware/display.h"
#include "hardware/gpsctl.h"
#include "hardware/timesync.h"

#include "gui/widget_factory.h"
#include "gui/widget_styles.h"
#include "gui/mainbar/mainbar.h"


#ifdef NATIVE_64BIT
    #include "utils/logging.h"
#else
    #include <Arduino.h>
    #include <WiFi.h>
    #include <Ticker.h>
    #include "FS.h"
    #include "SD.h"
#endif

static bool statusbar_init = false;
static bool statusbar_expanded = false;
static bool statusbar_refresh_update = false;
static bool force_dark_mode = false;

static lv_obj_t *statusbar = NULL;
static lv_obj_t *statusbar_wifi = NULL;
static lv_obj_t *statusbar_wifilabel = NULL;
static lv_obj_t *statusbar_wifiiplabel = NULL;
static lv_obj_t *statusbar_bluetooth = NULL;
static lv_obj_t *statusbar_gps = NULL;
static lv_obj_t *statusbar_timelabel = NULL;
static lv_obj_t *statusbar_volume_slider = NULL;
static lv_obj_t *statusbar_brightness_slider = NULL;
static lv_obj_t *statusbar_sound_icon = NULL;
static lv_obj_t *statusbar_brightness_icon = NULL;
static lv_style_t statusbarstyle[ STATUSBAR_STYLE_NUM ];

lv_color_t statusbar_retracted_color;
lv_color_t statusbar_extended_color;

LV_IMG_DECLARE(wifi_64px);
LV_IMG_DECLARE(bluetooth_64px);
LV_IMG_DECLARE(alarm_16px);
LV_IMG_DECLARE(brightness_32px);
LV_IMG_DECLARE(sound_32px);
LV_IMG_DECLARE(sound_mute_32px);
LV_IMG_DECLARE(gps_64px);
LV_FONT_DECLARE(Ubuntu_16px);
LV_FONT_DECLARE(Ubuntu_32px);

lv_status_bar_t statusicon[ STATUSBAR_NUM ] = 
{
    { NULL, NULL, LV_ALIGN_RIGHT_MID, &statusbarstyle[ STATUSBAR_STYLE_WHITE ] },
    { NULL, LV_SYMBOL_BATTERY_FULL, LV_ALIGN_OUT_LEFT_MID, &statusbarstyle[ STATUSBAR_STYLE_WHITE ] },
    { NULL, LV_SYMBOL_BLUETOOTH, LV_ALIGN_OUT_LEFT_MID, &statusbarstyle[ STATUSBAR_STYLE_WHITE ] },
    { NULL, LV_SYMBOL_WIFI, LV_ALIGN_OUT_LEFT_MID, &statusbarstyle[ STATUSBAR_STYLE_WHITE ] },
    { NULL, LV_SYMBOL_VOLUME_MAX, LV_ALIGN_OUT_LEFT_MID, &statusbarstyle[ STATUSBAR_STYLE_WHITE ] },
    { NULL, LV_SYMBOL_GPS, LV_ALIGN_OUT_LEFT_MID, &statusbarstyle[ STATUSBAR_STYLE_WHITE ] },
    { NULL, LV_SYMBOL_BELL, LV_ALIGN_OUT_LEFT_MID, &statusbarstyle[ STATUSBAR_STYLE_WHITE ] },
    { NULL, LV_SYMBOL_WARNING, LV_ALIGN_OUT_LEFT_MID, &statusbarstyle[ STATUSBAR_STYLE_WHITE ] },
    { NULL, LV_SYMBOL_BELL , LV_ALIGN_OUT_LEFT_MID, &statusbarstyle[ STATUSBAR_STYLE_WHITE ] },
};

bool should_save_brightness_config = false;
bool should_save_sound_config = false;

void statusbar_event( lv_event_t * e );
void statusbar_wifi_event_cb( lv_event_t * e );
void statusbar_bluetooth_event_cb( lv_event_t * e );
void statusbar_volume_slider_event_handler_cb( lv_event_t * e );
bool statusbar_gpsctl_event_cb( EventBits_t event, void *arg );
void statusbar_sound_event_cb( lv_event_t * e );
void statusbar_display_event_cb( lv_event_t * e );
void statusbar_brightness_slider_event_handler_cb( lv_event_t * e );

bool statusbar_soundctl_event_cb( EventBits_t event, void *arg );
bool statusbar_blectl_event_cb( EventBits_t event, void *arg );
bool statusbar_wifictl_event_cb( EventBits_t event, void *arg );
bool statusbar_rtcctl_event_cb( EventBits_t event, void *arg );
bool statusbar_pmuctl_event_cb( EventBits_t event, void *arg );
bool statusbar_displayctl_event_cb( EventBits_t event, void *arg );
bool statusbar_style_event_cb( EventBits_t event, void *arg );

void statusbar_wifi_set_state( bool state, const char *wifiname );
void statusbar_wifi_set_ip_state( bool state, const char *ip );
void statusbar_bluetooth_set_state( bool state );
void statusbar_gps_event_cb( lv_event_t * e );
void statusbar_set_dark( bool dark_mode );

void statusbar_update_time();

lv_timer_t * statusbar_task;
void statusbar_update_task( lv_timer_t * task );

void statusbar_setup( void )
{
    if ( statusbar_init ) {
        log_e("statusbar already init");
        return;
    }

    statusbar_retracted_color = lv_color_white();
    statusbar_extended_color = lv_color_black();

#if defined( BIG_THEME )
    const lv_font_t *statusbar_font = &lv_font_montserrat_32;
#else
    const lv_font_t *statusbar_font = &lv_font_montserrat_14;
#endif

#if defined( ROUND_DISPLAY )
    #define STATUSBAR_ICON_X_OFFSET     -70
#else
    #define STATUSBAR_ICON_X_OFFSET     -5
#endif

    /*Copy a built-in style to initialize the new style*/
    lv_style_init(&statusbarstyle[ STATUSBAR_STYLE_NORMAL ] );
    lv_style_set_radius(&statusbarstyle[ STATUSBAR_STYLE_NORMAL ],  0);
    lv_style_set_bg_color(&statusbarstyle[ STATUSBAR_STYLE_NORMAL ],  lv_color_white() );
    lv_style_set_bg_opa(&statusbarstyle[ STATUSBAR_STYLE_NORMAL ],  LV_OPA_20);
    lv_style_set_border_width(&statusbarstyle[ STATUSBAR_STYLE_NORMAL ],  0);
    lv_style_set_text_color(&statusbarstyle[ STATUSBAR_STYLE_NORMAL ],  lv_color_white());
    lv_style_set_img_recolor(&statusbarstyle[ STATUSBAR_STYLE_NORMAL ],  lv_color_white());
    lv_style_set_text_font( &statusbarstyle[ STATUSBAR_STYLE_NORMAL ], statusbar_font );

    lv_style_init( &statusbarstyle[ STATUSBAR_STYLE_WHITE ] );
    lv_style_set_bg_opa(&statusbarstyle[ STATUSBAR_STYLE_WHITE ],  LV_OPA_0);
    lv_style_set_text_color(&statusbarstyle[ STATUSBAR_STYLE_WHITE ],  statusbar_retracted_color );
    lv_style_set_img_recolor(&statusbarstyle[ STATUSBAR_STYLE_WHITE ],  statusbar_retracted_color );

    lv_style_init( &statusbarstyle[ STATUSBAR_STYLE_BLACK ] );
    lv_style_set_bg_opa(&statusbarstyle[ STATUSBAR_STYLE_BLACK ],  LV_OPA_0);
    lv_style_set_text_color(&statusbarstyle[ STATUSBAR_STYLE_BLACK ],  lv_color_black());
    lv_style_set_img_recolor(&statusbarstyle[ STATUSBAR_STYLE_BLACK ],  lv_color_black());

    lv_style_init( &statusbarstyle[ STATUSBAR_STYLE_RED ] );
    lv_style_set_bg_opa(&statusbarstyle[ STATUSBAR_STYLE_RED ],  LV_OPA_0);
    lv_style_set_text_color(&statusbarstyle[ STATUSBAR_STYLE_RED ],  lv_palette_main(LV_PALETTE_RED));
    lv_style_set_img_recolor(&statusbarstyle[ STATUSBAR_STYLE_RED ],  lv_palette_main(LV_PALETTE_RED));

    lv_style_init( &statusbarstyle[ STATUSBAR_STYLE_GRAY ] );
    lv_style_set_bg_opa(&statusbarstyle[ STATUSBAR_STYLE_GRAY ],  LV_OPA_0);
    lv_style_set_text_color(&statusbarstyle[ STATUSBAR_STYLE_GRAY ],  lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_img_recolor(&statusbarstyle[ STATUSBAR_STYLE_GRAY ],  lv_palette_main(LV_PALETTE_GREY));

    lv_style_init( &statusbarstyle[ STATUSBAR_STYLE_GREEN ] );
    lv_style_set_bg_opa(&statusbarstyle[ STATUSBAR_STYLE_GREEN ],  LV_OPA_0);
    lv_style_set_text_color(&statusbarstyle[ STATUSBAR_STYLE_GREEN ],  lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_img_recolor(&statusbarstyle[ STATUSBAR_STYLE_GREEN ],  lv_palette_main(LV_PALETTE_GREEN));

    lv_style_init( &statusbarstyle[ STATUSBAR_STYLE_YELLOW ] );
    lv_style_set_bg_opa(&statusbarstyle[ STATUSBAR_STYLE_YELLOW ],  LV_OPA_0);
    lv_style_set_text_color(&statusbarstyle[ STATUSBAR_STYLE_YELLOW ],  lv_palette_main(LV_PALETTE_YELLOW));
    lv_style_set_img_recolor(&statusbarstyle[ STATUSBAR_STYLE_YELLOW ],  lv_palette_main(LV_PALETTE_YELLOW));

    lv_style_init( &statusbarstyle[ STATUSBAR_STYLE_BLUE ] );
    lv_style_set_bg_opa(&statusbarstyle[ STATUSBAR_STYLE_BLUE ],  LV_OPA_0);
    lv_style_set_text_color(&statusbarstyle[ STATUSBAR_STYLE_BLUE ],  lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_img_recolor(&statusbarstyle[ STATUSBAR_STYLE_BLUE ],  lv_palette_main(LV_PALETTE_BLUE));

    statusbar = lv_obj_create( lv_scr_act() );
    lv_obj_set_width( statusbar, lv_disp_get_hor_res( lv_disp_get_default() ) );
    lv_obj_set_height( statusbar, STATUSBAR_HEIGHT );
    lv_obj_remove_style_all(statusbar);
    lv_obj_add_style( statusbar, &statusbarstyle[ STATUSBAR_STYLE_NORMAL ], 0 );
    lv_obj_align( statusbar, LV_ALIGN_TOP_MID, 0, 0 );
    lv_obj_add_event_cb( statusbar, statusbar_event, LV_EVENT_ALL, NULL );

    for( int i = 0 ; i < STATUSBAR_NUM ; i++ ) {
        if ( statusicon[i].symbol == NULL ) {
            statusicon[i].icon = lv_label_create( statusbar );
            lv_label_set_text( statusicon[i].icon, "100%" );
        }
        else {
            statusicon[i].icon = lv_img_create( statusbar );
            lv_img_set_src( statusicon[i].icon, statusicon[i].symbol );
        }
        lv_obj_remove_style_all(statusicon[i].icon);
        lv_obj_add_style( statusicon[i].icon, statusicon[i].style, 0 );
        if ( i == 0 ) {
            lv_obj_align(statusicon[i].icon, LV_ALIGN_TOP_RIGHT, STATUSBAR_ICON_X_OFFSET, 0 );
        }
        else {
            lv_obj_align_to(statusicon[i].icon, statusicon[i-1].icon, LV_ALIGN_OUT_LEFT_MID, -5, 0);
        }
    }

    static lv_style_t style;
    lv_style_init( &style );

    statusbar_wifi = wf_add_image_button_old( statusbar, &wifi_64px, statusbar_wifi_event_cb, &style );
    lv_obj_add_flag(statusbar_wifi, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_align( statusbar_wifi, LV_ALIGN_TOP_MID, 0, STATUSBAR_HEIGHT );
    lv_obj_add_state(statusbar_wifi, LV_STATE_CHECKED | LV_STATE_PRESSED);

    /*Create a label on the Image button*/
    statusbar_wifilabel = lv_label_create( statusbar );
    lv_obj_remove_style_all(statusbar_wifilabel);
    lv_obj_add_style( statusbar_wifilabel, &statusbarstyle[ STATUSBAR_STYLE_GREEN ], 0 );
    lv_label_set_text( statusbar_wifilabel, "");
    lv_obj_align_to( statusbar_wifilabel, statusbar_wifi, LV_ALIGN_OUT_BOTTOM_MID, 0, 0 );

    /*Create a label on the Image button*/
    statusbar_wifiiplabel = lv_label_create( statusbar );
    lv_obj_remove_style_all(statusbar_wifiiplabel);
    lv_obj_add_style( statusbar_wifiiplabel, &statusbarstyle[ STATUSBAR_STYLE_GREEN ], 0 );
    lv_label_set_text(statusbar_wifiiplabel, "");
    lv_obj_align_to(statusbar_wifiiplabel, statusbar_wifilabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 0 );

    statusbar_bluetooth = wf_add_image_button_old( statusbar, &bluetooth_64px, statusbar_bluetooth_event_cb, &style );
    lv_obj_add_flag(statusbar_bluetooth, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_align( statusbar_bluetooth, LV_ALIGN_TOP_RIGHT, -8, STATUSBAR_HEIGHT );
    lv_obj_add_state(statusbar_bluetooth, LV_STATE_CHECKED | LV_STATE_PRESSED);

    statusbar_gps = wf_add_image_button_old( statusbar, &gps_64px, statusbar_gps_event_cb, &style );
    lv_obj_add_flag(statusbar_gps, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_align( statusbar_gps, LV_ALIGN_TOP_LEFT, 8, STATUSBAR_HEIGHT );
    lv_obj_add_state(statusbar_gps, LV_STATE_CHECKED | LV_STATE_PRESSED);

    statusbar_timelabel = lv_label_create(statusbar );
    lv_obj_remove_style_all(statusbar_timelabel);
    lv_obj_add_style( statusbar_timelabel, &statusbarstyle[ STATUSBAR_STYLE_WHITE ], 0 );
    lv_label_set_text( statusbar_timelabel, "");
    lv_obj_align( statusbar_timelabel, LV_ALIGN_LEFT_MID, 5, 0 );

    lv_obj_t *statusbar_volume_cont = lv_obj_create( statusbar );
    lv_obj_add_style( statusbar_volume_cont, &style, 0 );
    lv_obj_set_size( statusbar_volume_cont, lv_disp_get_hor_res( lv_disp_get_default() ) , 36 );
    lv_obj_align( statusbar_volume_cont, LV_ALIGN_BOTTOM_MID, 0, STATUSBAR_EXPAND_HEIGHT - STATUSBAR_HEIGHT );
    statusbar_volume_slider = lv_slider_create( statusbar_volume_cont );
    lv_obj_set_size( statusbar_volume_slider, lv_disp_get_hor_res( lv_disp_get_default() ) - 100 , 10 );
    lv_obj_align( statusbar_volume_slider, LV_ALIGN_RIGHT_MID, -30, 0 );
    lv_obj_add_flag( statusbar_volume_slider, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_add_style( statusbar_volume_slider, ws_get_slider_style(), LV_PART_INDICATOR );
    lv_obj_add_style( statusbar_volume_slider, ws_get_slider_style(), LV_PART_KNOB );
    lv_slider_set_range( statusbar_volume_slider, 0, 100 );
    lv_obj_add_event_cb( statusbar_volume_slider, statusbar_volume_slider_event_handler_cb, LV_EVENT_ALL, NULL ) ;
    statusbar_sound_icon = lv_img_create( statusbar_volume_cont );
    lv_obj_add_flag( statusbar_sound_icon, LV_OBJ_FLAG_CLICKABLE );
    lv_obj_add_event_cb( statusbar_sound_icon, statusbar_sound_event_cb, LV_EVENT_ALL, NULL );
    lv_img_set_src( statusbar_sound_icon, &sound_32px );
    lv_obj_align( statusbar_sound_icon, LV_ALIGN_LEFT_MID, 15, 0 );

    lv_obj_t *statusbar_brightness_cont = lv_obj_create( statusbar );
    lv_obj_add_style( statusbar_brightness_cont, &style, 0 );
    lv_obj_set_size( statusbar_brightness_cont, lv_disp_get_hor_res( lv_disp_get_default() ) , 40 );
    lv_obj_align_to( statusbar_brightness_cont, statusbar_volume_cont, LV_ALIGN_OUT_TOP_MID, 0, 0 );
    statusbar_brightness_slider = lv_slider_create( statusbar_brightness_cont );
    lv_obj_set_size( statusbar_brightness_slider, lv_disp_get_hor_res( lv_disp_get_default() ) - 100 , 10 );
    lv_obj_align( statusbar_brightness_slider, LV_ALIGN_RIGHT_MID, -30, 0 );
    lv_slider_set_range( statusbar_brightness_slider, DISPLAY_MIN_BRIGHTNESS, DISPLAY_MAX_BRIGHTNESS );
    lv_obj_add_flag( statusbar_brightness_slider, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_add_style( statusbar_brightness_slider, ws_get_slider_style(), LV_PART_INDICATOR );
    lv_obj_add_style( statusbar_brightness_slider, ws_get_slider_style(), LV_PART_KNOB );
    lv_obj_add_event_cb( statusbar_brightness_slider, statusbar_brightness_slider_event_handler_cb, LV_EVENT_ALL, NULL ) ;
    statusbar_brightness_icon = lv_img_create( statusbar_brightness_cont );
    lv_obj_add_flag( statusbar_brightness_icon, LV_OBJ_FLAG_CLICKABLE );
    lv_obj_add_event_cb( statusbar_brightness_icon, statusbar_display_event_cb, LV_EVENT_ALL, NULL );
    lv_img_set_src( statusbar_brightness_icon, &brightness_32px );
    lv_obj_align( statusbar_brightness_icon, LV_ALIGN_LEFT_MID, 15, 0 );

    lv_slider_set_value( statusbar_brightness_slider, display_get_brightness(), LV_ANIM_OFF );
    lv_slider_set_value( statusbar_volume_slider, sound_get_volume_config(), LV_ANIM_OFF );

    /*
     * init fin
     */
    statusbar_init = true;

    statusbar_hide_icon( STATUSBAR_BELL );
    statusbar_hide_icon( STATUSBAR_WARNING );
    statusbar_hide_icon( STATUSBAR_WIFI );
    statusbar_hide_icon( STATUSBAR_BLUETOOTH );
    statusbar_hide_icon( STATUSBAR_VOLUME );
    statusbar_hide_icon( STATUSBAR_GPS );

    if ( rtcctl_get_alarm_data()->enabled ) {
        statusbar_show_icon( STATUSBAR_ALARM );
    }
    else {
        statusbar_hide_icon( STATUSBAR_ALARM );
    }

    statusbar_style_icon( STATUSBAR_BLUETOOTH, STATUSBAR_STYLE_GRAY );

    blectl_register_cb( BLECTL_CONNECT | BLECTL_DISCONNECT | BLECTL_ON | BLECTL_OFF, statusbar_blectl_event_cb, "statusbar bluetooth" );
    wifictl_register_cb( WIFICTL_CONNECT | WIFICTL_DISCONNECT | WIFICTL_OFF | WIFICTL_ON | WIFICTL_MSG | WIFICTL_WPS_SUCCESS | WIFICTL_WPS_FAILED | WIFICTL_CONNECT_IP, statusbar_wifictl_event_cb, "statusbar wifi" );
    rtcctl_register_cb( RTCCTL_ALARM_ENABLED | RTCCTL_ALARM_DISABLED, statusbar_rtcctl_event_cb, "statusbar rtc" );
    pmu_register_cb( PMUCTL_STATUS, statusbar_pmuctl_event_cb, "statusbar pmu");
    display_register_cb( DISPLAYCTL_BRIGHTNESS, statusbar_displayctl_event_cb, "statusbar display" );
    gpsctl_register_cb( GPSCTL_ENABLE | GPSCTL_DISABLE | GPSCTL_FIX | GPSCTL_NOFIX, statusbar_gpsctl_event_cb, "statusbar gps" );
    styles_register_cb( STYLE_DARKMODE | STYLE_LIGHTMODE, statusbar_style_event_cb, "statusbar style event" );

    statusbar_task = lv_timer_create( statusbar_update_task, 250, NULL );

    if( sound_get_available() ) {
        sound_register_cb( SOUNDCTL_ENABLED | SOUNDCTL_VOLUME, statusbar_soundctl_event_cb, "statusbar sound");
    }
    else {
        lv_obj_add_flag( statusbar_volume_cont, LV_OBJ_FLAG_HIDDEN );
    }

    statusbar_hide( false );
}

void statusbar_update_task( lv_timer_t * task ) {
    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return;
    }

    if ( statusbar_refresh_update ) {
        statusbar_refresh();
        statusbar_refresh_update = false;
    }
    statusbar_update_time();

}

bool statusbar_style_event_cb( EventBits_t event, void *arg ) {
    switch( event ) {
        case STYLE_LIGHTMODE:   statusbar_retracted_color = lv_color_white();
                                statusbar_extended_color = lv_color_black();
                                statusbar_set_dark( false );
                                break;
        case STYLE_DARKMODE:    statusbar_retracted_color = lv_color_black();
                                statusbar_extended_color = lv_color_black();
                                statusbar_set_dark( true );
    }
    
    return( true );
}

void statusbar_update_time() {
    /*
     * check if statusbar alread initialized
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return;
    }

    time_t now;
    static time_t last = 0;
    struct tm  info, last_info;
    char time_str[64]="";
    char info_str[64]="";
    /*
     * copy current time into now and convert it local time info
     */
    time( &now );
    localtime_r( &now, &info );
    /*
     * convert last time_t into tm from
     * last check if last equal zero (first run condition)
     */
    if ( last != 0 ) {
        localtime_r( &last, &last_info );
    }
    /*
     * Time:
     * only update while time changes or force is set
     * Display has a minute resolution
     */
    if ( last == 0 || info.tm_min != last_info.tm_min || info.tm_hour != last_info.tm_hour ) {
        timesync_get_current_timestring( time_str, sizeof(time_str) );
        log_d("renew time: %s", time_str );
        lv_label_set_text( statusbar_timelabel, time_str );
        /*
         * Save for next loop
         */
        last = now;
    }
}

bool statusbar_gpsctl_event_cb( EventBits_t event, void *arg ) {
    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return( true );
    }

    switch( event ) {
        case GPSCTL_DISABLE:  
            statusbar_hide_icon( STATUSBAR_GPS );
            statusbar_style_icon( STATUSBAR_GPS, STATUSBAR_STYLE_GRAY );
            lv_obj_add_state(statusbar_gps, LV_STATE_CHECKED);
            break;
        case GPSCTL_ENABLE:  
            statusbar_show_icon( STATUSBAR_GPS );
            statusbar_style_icon( STATUSBAR_GPS, STATUSBAR_STYLE_GRAY );
            lv_obj_clear_state(statusbar_gps, LV_STATE_CHECKED);
            break;
        case GPSCTL_FIX:  
            statusbar_show_icon( STATUSBAR_GPS );
            statusbar_style_icon( STATUSBAR_GPS, STATUSBAR_STYLE_WHITE );
            break;
        case GPSCTL_NOFIX:  
            statusbar_show_icon( STATUSBAR_GPS );
            statusbar_style_icon( STATUSBAR_GPS, STATUSBAR_STYLE_GRAY );
            break;
    }
    return( true );

}

bool statusbar_soundctl_event_cb( EventBits_t event, void *arg ) {
    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return( true );
    }

    switch( event ) {
        case SOUNDCTL_ENABLED:  
            if ( *(bool*)arg ) {
                statusbar_show_icon( STATUSBAR_VOLUME );
                lv_img_set_src( statusbar_sound_icon, &sound_32px );
            }
            else {
                statusbar_hide_icon( STATUSBAR_VOLUME );
                lv_img_set_src( statusbar_sound_icon, &sound_mute_32px );
                lv_slider_set_value( statusbar_volume_slider, 0, LV_ANIM_OFF );
            }
            break;
        case SOUNDCTL_VOLUME:
            lv_slider_set_value( statusbar_volume_slider, sound_get_volume_config(), LV_ANIM_OFF );
            break;
    }
    return( true );
}

bool statusbar_displayctl_event_cb( EventBits_t event, void *arg ) {
    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return( true );
    }

    switch( event ) {
        case DISPLAYCTL_BRIGHTNESS:  
            lv_slider_set_value( statusbar_brightness_slider, display_get_brightness(), LV_ANIM_OFF );
            break;
    }
    return( true );
}

static void statusbar_pmuctl_update_batt( int32_t percent, bool charging, bool plug) {
    char level[8]="";

    if ( percent >= 0 && percent <= 100 ) {
        snprintf( level, sizeof( level ), "%d%%", percent );
    }
    else if ( percent > 100 ) {
        snprintf( level, sizeof( level ), "!%d%%", percent );
    }
    else {
        snprintf( level, sizeof( level ), "?" );
        percent = 0;
    }
    lv_label_set_text( statusicon[  STATUSBAR_BATTERY_PERCENT ].icon, (const char *)level );
    if ( !plug ) {
        if ( percent >= 75 ) { 
            lv_img_set_src( statusicon[ STATUSBAR_BATTERY ].icon, LV_SYMBOL_BATTERY_FULL );
        } else if( percent >=50 && percent < 74) {
            lv_img_set_src( statusicon[ STATUSBAR_BATTERY ].icon, LV_SYMBOL_BATTERY_3 );
        } else if( percent >=35 && percent < 49) {
            lv_img_set_src( statusicon[ STATUSBAR_BATTERY ].icon, LV_SYMBOL_BATTERY_2 );
        } else if( percent >=15 && percent < 34) {
            lv_img_set_src( statusicon[ STATUSBAR_BATTERY ].icon, LV_SYMBOL_BATTERY_1 );
        } else if( percent >=0 && percent < 14) {
            lv_img_set_src( statusicon[ STATUSBAR_BATTERY ].icon, LV_SYMBOL_BATTERY_EMPTY );
        }

        if ( percent >= 25 ) {
            statusbar_style_icon( STATUSBAR_BATTERY, STATUSBAR_STYLE_WHITE );
        } else if ( percent >= 15 ) {
            statusbar_style_icon( STATUSBAR_BATTERY, STATUSBAR_STYLE_YELLOW );
        } else {
            statusbar_style_icon( STATUSBAR_BATTERY, STATUSBAR_STYLE_RED );
        }       
    }

    if ( charging ) {
        statusbar_style_icon( STATUSBAR_BATTERY, STATUSBAR_STYLE_RED );
    }
    else {
        statusbar_style_icon( STATUSBAR_BATTERY, STATUSBAR_STYLE_WHITE );
    }

    if ( plug ) {
        lv_img_set_src( statusicon[ STATUSBAR_BATTERY ].icon, LV_SYMBOL_CHARGE );
        statusbar_style_icon( STATUSBAR_BATTERY, STATUSBAR_STYLE_GREEN );
    }
}

bool statusbar_pmuctl_event_cb( EventBits_t event, void *arg ) {
    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return( true );
    }

    switch( event ) {
        case PMUCTL_STATUS:
            statusbar_pmuctl_update_batt(*(int32_t*)arg & PMUCTL_STATUS_PERCENT,
                                         *(int32_t*)arg & PMUCTL_STATUS_CHARGING,
                                         *(int32_t*)arg & PMUCTL_STATUS_PLUG);
            break;
    }
    return( true );
}

bool statusbar_rtcctl_event_cb( EventBits_t event, void *arg ) {
    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return( true );
    }

    switch( event ) {
        case RTCCTL_ALARM_ENABLED:  
            statusbar_show_icon( STATUSBAR_ALARM );
            break;
        case RTCCTL_ALARM_DISABLED: 
            statusbar_hide_icon( STATUSBAR_ALARM );
            break;
    }
    statusbar_refresh_update = true;
    return( true );
}

bool statusbar_blectl_event_cb( EventBits_t event, void *arg ) {
    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return( true );
    }

    switch( event ) {
        case BLECTL_ON:             statusbar_show_icon( STATUSBAR_BLUETOOTH );
                                    statusbar_style_icon( STATUSBAR_BLUETOOTH, STATUSBAR_STYLE_GRAY );
                                    lv_obj_clear_state(statusbar_bluetooth, LV_STATE_CHECKED);
                                    break;
        case BLECTL_OFF:            statusbar_hide_icon( STATUSBAR_BLUETOOTH );
                                    lv_obj_add_state(statusbar_bluetooth, LV_STATE_CHECKED);
                                    break;
        case BLECTL_CONNECT:        statusbar_style_icon( STATUSBAR_BLUETOOTH, STATUSBAR_STYLE_WHITE );
                                    break;
        case BLECTL_DISCONNECT:     statusbar_style_icon( STATUSBAR_BLUETOOTH, STATUSBAR_STYLE_GRAY );
                                    break;
    }
    statusbar_refresh_update = true;
    return( true );
}

bool statusbar_wifictl_event_cb( EventBits_t event, void *arg ) {
    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return( true );
    }

    switch( event ) {
        case WIFICTL_CONNECT:       statusbar_style_icon( STATUSBAR_WIFI, STATUSBAR_STYLE_WHITE );
                                    statusbar_wifi_set_state( true, (char *)arg );
                                    statusbar_show_icon( STATUSBAR_WIFI );
                                    break;
        case WIFICTL_CONNECT_IP:    statusbar_style_icon( STATUSBAR_WIFI, STATUSBAR_STYLE_WHITE );
                                    statusbar_wifi_set_ip_state( true, (char *)arg );
                                    statusbar_show_icon( STATUSBAR_WIFI );
                                    break;
        case WIFICTL_DISCONNECT:    statusbar_style_icon( STATUSBAR_WIFI, STATUSBAR_STYLE_GRAY );
                                    statusbar_wifi_set_state( false, (char *)arg );
                                    statusbar_show_icon( STATUSBAR_WIFI );
                                    break;
        case WIFICTL_OFF:           statusbar_style_icon( STATUSBAR_WIFI, STATUSBAR_STYLE_GRAY );
                                    statusbar_hide_icon( STATUSBAR_WIFI );
                                    statusbar_wifi_set_state( false, "" );
                                    break;
        case WIFICTL_ON:            statusbar_style_icon( STATUSBAR_WIFI, STATUSBAR_STYLE_GRAY );
                                    statusbar_wifi_set_state( true, (char *)arg );
                                    statusbar_show_icon( STATUSBAR_WIFI );
                                    break;
        case WIFICTL_WPS_SUCCESS:   statusbar_style_icon( STATUSBAR_WIFI, STATUSBAR_STYLE_GRAY );
                                    statusbar_wifi_set_state( true, (char *)arg );
                                    statusbar_show_icon( STATUSBAR_WIFI );
                                    break;
        case WIFICTL_WPS_FAILED:    statusbar_style_icon( STATUSBAR_WIFI, STATUSBAR_STYLE_GRAY );
                                    statusbar_wifi_set_state( true, (char *)arg );
                                    statusbar_show_icon( STATUSBAR_WIFI );
                                    break;
        case WIFICTL_MSG:           statusbar_style_icon( STATUSBAR_WIFI, STATUSBAR_STYLE_GRAY );
                                    statusbar_wifi_set_state( true, (char *)arg );
                                    statusbar_show_icon( STATUSBAR_WIFI );
                                    break;
    }
    statusbar_refresh_update = true;
    return( true );
}

void statusbar_volume_slider_event_handler_cb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * volume_slider = lv_event_get_target(e);

    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return;
    }

    if(code == LV_EVENT_VALUE_CHANGED) {
        if( lv_slider_get_value( volume_slider ) == 0){
            sound_set_enabled_config( false );
            sound_set_volume_config( 1 );
        }
        else{
            if( !sound_get_enabled_config() ){
                sound_set_enabled_config( true );
            }
            sound_set_volume_config( lv_slider_get_value( volume_slider ) );
        }
        should_save_sound_config = true;
    }
}

void statusbar_brightness_slider_event_handler_cb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * brightness_slider = lv_event_get_target(e);

    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return;
    }

    if(code == LV_EVENT_VALUE_CHANGED) {
        log_d("Brightness value: %d\n", lv_slider_get_value( brightness_slider ));
        display_set_brightness( lv_slider_get_value( brightness_slider ));
        should_save_brightness_config = true;
    }
}


void statusbar_display_event_cb( lv_event_t * e ){
    lv_event_code_t code = lv_event_get_code(e);

    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return;
    }

    switch ( code ) {
        case ( LV_EVENT_LONG_PRESSED ):             
            statusbar_expand( false );
//            mainbar_jump_to_tilenumber( display_get_setup_tile_num(), LV_ANIM_OFF);
            break;
    }
    statusbar_refresh_update = true;
}

void statusbar_sound_event_cb( lv_event_t * e ) {
    lv_event_code_t code = lv_event_get_code(e);

    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return;
    }

    static uint8_t volume;

    switch ( code ) {
        case ( LV_EVENT_PRESSED ):             
            if ( sound_get_enabled_config() ) {
                volume = sound_get_volume_config();
                sound_set_enabled_config( false );
            }
            else {
                sound_set_enabled_config( true );
                sound_set_volume_config( volume );
            }
            break;
        case ( LV_EVENT_LONG_PRESSED ):             
            statusbar_expand( false );
//            mainbar_jump_to_tilenumber( sound_get_setup_tile_num(), LV_ANIM_OFF);
            break;
    }
    statusbar_refresh_update = true;
}

void statusbar_wifi_event_cb( lv_event_t * e ) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * wifi = lv_event_get_target(e);

    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return;
    }

    switch ( code ) {
        case ( LV_EVENT_VALUE_CHANGED ):
            if(lv_obj_has_state(wifi, LV_STATE_CHECKED)) {
                wifictl_off();
                                                        wifictl_set_autoon( false );
            } else {
                wifictl_on();
                                                        wifictl_set_autoon( true );
            }
            statusbar_refresh_update = true;
            break;
        case ( LV_EVENT_LONG_PRESSED ):             
            statusbar_expand( false );
//            mainbar_jump_to_tilenumber(wifi_get_setup_tile_num(), LV_ANIM_OFF);
            break;
    }
    statusbar_refresh_update = true;
}

void statusbar_gps_event_cb( lv_event_t * e ) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * gps = lv_event_get_target(e);

    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return;
    }

    switch ( code ) {
        case ( LV_EVENT_VALUE_CHANGED ):
            if(lv_obj_has_state(gps, LV_STATE_CHECKED)) {
                gpsctl_off();
            }
            else {
                gpsctl_on();
            }
            statusbar_refresh_update = true;
            break;
        case ( LV_EVENT_LONG_PRESSED ):             
            statusbar_expand( false );
//            mainbar_jump_to_tilenumber( gps_get_setup_tile_num() , LV_ANIM_OFF);
            statusbar_refresh_update = true;
            break;
    }
}

void statusbar_bluetooth_event_cb( lv_event_t * e ) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * bluetooth = lv_event_get_target(e);

    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return;
    }

    switch ( code ) {
        case ( LV_EVENT_VALUE_CHANGED ):
            if(lv_obj_has_state(bluetooth, LV_STATE_CHECKED)) {
                blectl_off();
            }
            else {
                blectl_on();
            }
            statusbar_refresh_update = true;
            break;
        case ( LV_EVENT_LONG_PRESSED ):             
            statusbar_expand( false );
//            mainbar_jump_to_tilenumber(bluetooth_get_setup_tile_num(), LV_ANIM_OFF);
            break;
    }
    statusbar_refresh_update = true;
}

void statusbar_wifi_set_state( bool state, const char *wifiname ) {
    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return;
    }

    if( state ) {
        lv_obj_clear_state(statusbar_wifi, LV_STATE_CHECKED);
    }
    else {
        lv_obj_add_state(statusbar_wifi, LV_STATE_CHECKED);
    }
    lv_label_set_text( statusbar_wifilabel, wifiname );
    lv_label_set_text( statusbar_wifiiplabel, "" );
    lv_obj_align_to( statusbar_wifilabel, statusbar_wifi, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_align_to( statusbar_wifiiplabel, statusbar_wifilabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
}

void statusbar_wifi_set_ip_state( bool state, const char *ip ) {
    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return;
    }

    lv_label_set_text( statusbar_wifiiplabel, ip );
    lv_obj_align_to( statusbar_wifiiplabel, statusbar_wifilabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
}

void statusbar_bluetooth_set_state( bool state ) {
    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return;
    }

    if ( state ) {
        lv_obj_clear_state(statusbar_bluetooth, LV_STATE_CHECKED);
    }
    else {
        lv_obj_add_state(statusbar_bluetooth, LV_STATE_PRESSED);
    }
}

void statusbar_hide_icon( statusbar_icon_t icon ) {
    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return;
    }

    if ( icon >= STATUSBAR_NUM ) {
        return;
    }

    lv_obj_add_flag( statusicon[ icon ].icon, LV_OBJ_FLAG_HIDDEN );
    statusbar_refresh_update = true;
}

void statusbar_show_icon( statusbar_icon_t icon ) {
    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return;
    }

    if ( icon >= STATUSBAR_NUM ) {
        return;
    }

    lv_obj_clear_flag( statusicon[ icon ].icon, LV_OBJ_FLAG_HIDDEN );
    statusbar_refresh_update = true;
}

void statusbar_style_icon( statusbar_icon_t icon, statusbar_style_t style ) {
    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return;
    }

    if ( icon >= STATUSBAR_NUM || style >= STATUSBAR_STYLE_NUM ) {
        return;
    }

    statusicon[ icon ].style = &statusbarstyle[ style ];
    statusbar_refresh_update = true;
}

void statusbar_refresh( void ) {
    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return;
    }

    lv_obj_t *last_visible = NULL;

    for ( int i = 0 ; i < STATUSBAR_NUM ; i++ ) {
        if ( !lv_obj_has_flag( statusicon[ i ].icon, LV_OBJ_FLAG_HIDDEN ) ) {
            if ( last_visible == NULL ) {
                if ( !statusbar_expanded ) {
                    lv_obj_align(statusicon[i].icon, LV_ALIGN_TOP_RIGHT, STATUSBAR_ICON_X_OFFSET, 0 );
                }
            } else {
                lv_obj_align_to( statusicon[ i ].icon, last_visible, LV_ALIGN_OUT_LEFT_MID, -5, 0);
            }
            lv_obj_remove_style_all(statusicon[ i ].icon);
            lv_obj_add_style( statusicon[ i ].icon, statusicon[i].style, 0 );
            last_visible = statusicon[ i ].icon;
        }
    }
}

void statusbar_event( lv_event_t * e ) {
    lv_event_code_t code = lv_event_get_code(e);

    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return;
    }

    static bool expand = false;

    switch( code ) {
        case LV_EVENT_PRESSED:
            if ( expand ) {
                statusbar_expand( false );
                expand = false;
            } 
            else {
                statusbar_expand( true );
                expand = true;
            }
        default:
            break;
    }
}

void statusbar_set_force_dark( bool dark_mode ) {
    if( dark_mode ) {
        force_dark_mode = true;
    }
    else {
        force_dark_mode = false;
    }
    statusbar_refresh_update = true;
}

bool statusbar_get_force_dark( void ) {
    return( force_dark_mode );
}


void statusbar_set_dark( bool dark_mode ) {
    if ( dark_mode || force_dark_mode ) {
        lv_style_set_bg_opa(&statusbarstyle[ STATUSBAR_STYLE_NORMAL ], LV_OPA_90);
        lv_obj_remove_style_all(statusbar);
        lv_obj_add_style( statusbar, &statusbarstyle[ STATUSBAR_STYLE_NORMAL ], 0 );

        lv_style_set_bg_color(&statusbarstyle[ STATUSBAR_STYLE_WHITE ], statusbar_extended_color );
        lv_style_set_text_color(&statusbarstyle[ STATUSBAR_STYLE_WHITE ], statusbar_extended_color );
        lv_style_set_img_recolor(&statusbarstyle[ STATUSBAR_STYLE_WHITE ], statusbar_extended_color );
    }
    else {
        lv_style_set_bg_opa(&statusbarstyle[ STATUSBAR_STYLE_NORMAL ], LV_OPA_20);
        lv_obj_remove_style_all(statusbar);
        lv_obj_add_style( statusbar, &statusbarstyle[ STATUSBAR_STYLE_NORMAL ], 0 );

        lv_style_set_bg_color(&statusbarstyle[ STATUSBAR_STYLE_WHITE ], statusbar_retracted_color );
        lv_style_set_text_color(&statusbarstyle[ STATUSBAR_STYLE_WHITE ], statusbar_retracted_color );
        lv_style_set_img_recolor(&statusbarstyle[ STATUSBAR_STYLE_WHITE ], statusbar_retracted_color );
    }
}

void statusbar_expand( bool expand ) {
    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return;
    }

    if ( expand ) {
        lv_obj_set_height( statusbar, STATUSBAR_EXPAND_HEIGHT );
        statusbar_set_dark( true );
        statusbar_expanded = true;
    } 
    else {
        lv_obj_set_height( statusbar, STATUSBAR_HEIGHT );
        statusbar_set_dark( false );
        statusbar_expanded = false;
        //Save config here if anything has changed
        if( should_save_brightness_config ){
            display_save_config();
            should_save_brightness_config = false;
        }
        if( should_save_sound_config ){
            sound_save_config();
            should_save_sound_config = false;
        }
    }
    statusbar_refresh_update = true;
}

void statusbar_hide( bool hide ) {
    /*
     * check if statusbar ready
     */
    if ( !statusbar_init ) {
        log_e("statusbar not initialized");
        return;
    }

    #if defined( LILYGO_WATCH_2021 )
        lv_obj_add_flag( statusbar, LV_OBJ_FLAG_HIDDEN );
        statusbar_refresh_update = true;
    #else
        if(hide) lv_obj_add_flag( statusbar, LV_OBJ_FLAG_HIDDEN );
        else lv_obj_clear_flag( statusbar, LV_OBJ_FLAG_HIDDEN );
        statusbar_refresh_update = true;
    #endif
}

bool statusbar_get_hidden_state( void ) {
    statusbar_refresh_update = true;
    return lv_obj_has_flag( statusbar, LV_OBJ_FLAG_HIDDEN );
}