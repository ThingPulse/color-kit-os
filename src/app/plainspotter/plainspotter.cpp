
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
#include "quickglui/quickglui.h"

#include "plainspotter.h"
#include "gui/mainbar/mainbar.h"
#include "gui/widget_styles.h"
#include "gui/app.h"
#include "hardware/motion.h"
// #include "hardware/blestepctl.h"
#include "hardware/motor.h"
#include "hardware/timesync.h"
#include "utils/millis.h"
#include "plainspotter_app_main.h"

// App icon must have an size of 64x64 pixel with an alpha channel
// Use https://lvgl.io/tools/imageconverter to convert your images and set "true color with alpha"
LV_IMG_DECLARE(move_64px);
LV_IMG_DECLARE(trash_32px);
LV_FONT_DECLARE(Ubuntu_16px);
LV_FONT_DECLARE(Ubuntu_32px);

#define YES "Yes"
#define NO  "No"

static SynchronizedApplication plainspotterApp;
static JsonConfig config("plainspotter.json");

// Options
static String size, length, goal_step, goal_dist;
// Widgets
static Label lblStepcounter;


static Style big, small;

/* Default message box callback */
static lv_event_cb_t default_msgbox_cb;

static void build_main_page();
static void refresh_main_page();
static void build_settings();
static void plainspotter_activate_cb();
static void plainspotter_reset_cb(lv_obj_t * obj, lv_event_t event);
static bool plainspotter_app_time_update_event_cb( EventBits_t event, void *arg );


/*
 * automatic register the app setup function with explicit call in main.cpp
 */
static int registed = app_autocall_function( &plainspotter_app_setup, 8 );           /** @brief app autocall function */
/*
 * setup routine for application
 */
void plainspotter_app_setup() {
    // Create and register new application
    //   params: name, icon, auto add "refresh" button (this app will use synchronize function of the SynchronizedApplication class).
    //   Also, you can configure count of the required pages in the next two params (to have more app screens).
    plainspotterApp.init("plainspotter", &move_64px, 1, 1);

    mainbar_add_tile_activate_cb( plainspotterApp.mainTileId(), plainspotter_activate_cb );

    // Build and configure application
    build_main_page();
    build_settings();


    // Executed when user click "refresh" button
    plainspotterApp.synchronizeActionHandler([](SyncRequestSource source) {
        if ( blectl_get_event( BLECTL_ON ) )
        {
            // blestepctl_update(true);
        }
        // Return feedback to user as nothing else changed
        motor_vibe(20);
    });

    // Add a trash button to reset counter
    plainspotterApp.mainPage().addAppButton(trash_32px, [](Widget btn) {
        static const char * btns[] ={YES, NO, ""};

        lv_obj_t * mbox1 = lv_msgbox_create(lv_scr_act(), NULL);
        lv_msgbox_set_text(mbox1, "Reset step counter?");
        lv_msgbox_add_btns(mbox1, btns);
        lv_obj_set_width(mbox1, 200);
        // Save default callback
        default_msgbox_cb = lv_obj_get_event_cb(mbox1);
        lv_obj_set_event_cb(mbox1, plainspotter_reset_cb);
        lv_obj_align(mbox1, NULL, LV_ALIGN_CENTER, 0, 0);
    });
    
    refresh_main_page();
}

void build_main_page()
{
    big = Style::Create(ws_get_mainbar_style(), true);
    big.textFont(&Ubuntu_32px)
      .textOpacity(LV_OPA_80);
    small = Style::Create(ws_get_mainbar_style(), true);
    small.textFont(&Ubuntu_16px)
      .textOpacity(LV_OPA_80);

    AppPage& screen = plainspotterApp.mainPage(); // This is parent for all main screen widgets

    plainspotter_app_main_setup(&screen);
    
}

void refresh_main_page()
{

    //refresh_map();
}

void plainspotter_activate_cb()
{
    refresh_main_page();
}

static void plainspotter_reset_cb(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
        const char *answer = lv_msgbox_get_active_btn_text(obj);
        if ( strcmp( answer, YES) == 0 ) {
            /* Reset counter */
            bma_reset_stepcounter();
            /* Refresh display immediately for user feedback */
            refresh_main_page();
        }
    }
    /* Call the default callback to retrieve the default behavior */
    default_msgbox_cb(obj, event);
}

void build_settings()
{
    // Create full options list and attach items to variables
    config.addString("Step length (cm)", 3, "50").setDigitsMode(true,"0123456789").assign(&length); // cm
    config.addString("Step Goal", 7, "10000").setDigitsMode(true,"0123456789").assign(&goal_step); // steps
    config.addString("Distance Goal", 7, "5000").setDigitsMode(true,"0123456789").assign(&goal_dist); // m

    plainspotterApp.useConfig(config, true); // true - auto create settings page widgets
}

static bool plainspotter_app_time_update_event_cb( EventBits_t event, void *arg ) {
    log_i("Updating time event");
    printf("Updating time event");
    /*switch( event ) {
        case TIME_SYNC_UPDATE:
            update_time();
            break;
    }*/
    return( true );
}
