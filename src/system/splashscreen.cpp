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
#include "ui/screens.h"

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


LV_IMG_DECLARE(thingpulse_300px);

void splash_screen_stage_one( void ) {

    lv_screen_load(objects.splashscreen);
    lv_label_set_text(objects.message_label, "loading");
    lv_task_handler();

}

void splash_screen_stage_update( const char* msg, int value ) {
    lv_label_set_text(objects.message_label, msg);
    lv_task_handler();
    delay(500);
}

void splash_screen_stage_finish( void ) {

    lv_task_handler();
}
