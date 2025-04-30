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
#include <config.h>
#include "config/stock_ticker_config.h"
#include "stock_ticker_setup.h"

#include "gui/mainbar/mainbar.h"
#include "gui/mainbar/main_tile/main_tile.h"
#include "gui/statusbar.h"
#include "gui/keyboard.h"
#include "gui/widget_factory.h"
#include "gui/widget_styles.h"
#include "hardware/ble/gadgetbridge.h"
#include "hardware/motor.h"
#include "hardware/gpsctl.h"
#include "utils/bluejsonrequest.h"
#include "ui/screens.h"
#include "stock_ticker_app.h"


#ifdef NATIVE_64BIT
    #include "utils/logging.h"
#else
    #include <Arduino.h>
#endif

using namespace stock_ticker;

lv_obj_t *stock_ticker_setup_tile = NULL;

lv_obj_t * stock_ticker_symbol_setup_container[MAX_STOCK_TICKER_SYMBOLS];
lv_obj_t * stock_ticker_symbol_field[MAX_STOCK_TICKER_SYMBOLS];

static void stock_ticker_textarea_event_cb( lv_obj_t * obj, lv_event_t event );
static void exit_stock_ticker_setup_event_cb( lv_obj_t * obj, lv_event_t event );

void stock_ticker_setup_tile_setup( uint32_t tile_num ) {

    stock_ticker_setup_tile = mainbar_get_tile_obj( tile_num );

    create_screen_setup(stock_ticker_setup_tile);

    lv_obj_t *header = wf_add_settings_header( stock_ticker_setup_tile, "open stock ticker setup", exit_stock_ticker_setup_event_cb );
    lv_obj_align( header, stock_ticker_setup_tile, LV_ALIGN_IN_TOP_LEFT, 10, 10 );

    lv_obj_set_event_cb( objects.stock_ticker_api_field, stock_ticker_textarea_event_cb );
    lv_textarea_set_text(objects.stock_ticker_api_field, "");
    lv_textarea_set_cursor_hidden(objects.stock_ticker_api_field, true);


    stock_ticker_symbol_setup_container[0] = objects.stock_ticker_symbol_container;
    stock_ticker_symbol_field[0] = objects.stock_ticker_symbol_field;

    lv_obj_t * src, * clone;

    for (int i = 1; i < MAX_STOCK_TICKER_SYMBOLS; i++) {
        stock_ticker_symbol_setup_container[i] = lv_cont_create(stock_ticker_setup_tile, objects.stock_ticker_symbol_container);
        clone = stock_ticker_symbol_setup_container[i];
        src = objects.stock_ticker_symbol_container;
        lv_obj_set_size(clone, lv_obj_get_width(src), lv_obj_get_height(src));
        lv_obj_set_pos(clone, lv_obj_get_x(src), lv_obj_get_y(src) + i * (lv_obj_get_height(src)));


        stock_ticker_symbol_field[i] = lv_textarea_create(stock_ticker_symbol_setup_container[i], objects.stock_ticker_symbol_field);
        clone = stock_ticker_symbol_field[i];
        src = objects.stock_ticker_symbol_field;
        lv_obj_set_size(clone, lv_obj_get_width(src), lv_obj_get_height(src));
        lv_obj_set_pos(clone, lv_obj_get_x(src), lv_obj_get_y(src)); 


    }

    stock_ticker_config_t * config  = get_stock_ticker_config();
    lv_textarea_set_text(objects.stock_ticker_api_field, config->apikey);
    for (int i = 0; i < MAX_STOCK_TICKER_SYMBOLS; i++) {
        lv_textarea_set_text(stock_ticker_symbol_field[i], config->stock_symbols[i]);
        lv_obj_set_event_cb( stock_ticker_symbol_field[i], stock_ticker_textarea_event_cb );
        lv_textarea_set_cursor_hidden(stock_ticker_symbol_field[i], true);

    }


}

static void stock_ticker_textarea_event_cb( lv_obj_t * obj, lv_event_t event ) {
    if( event == LV_EVENT_CLICKED ) {
        keyboard_set_textarea( obj );
    }
}

static void exit_stock_ticker_setup_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):           
            keyboard_hide();
            stock_ticker_config_t * config = get_stock_ticker_config();
            strlcpy(config->apikey, lv_textarea_get_text(objects.stock_ticker_api_field), sizeof(config->apikey));

            for (int i = 0; i < MAX_STOCK_TICKER_SYMBOLS; i++) {
                strlcpy(config->stock_symbols[i], lv_textarea_get_text(stock_ticker_symbol_field[i]), sizeof(config->stock_symbols[i]));
                log_i("content of value: %s", config->stock_symbols[i]);
            }

            stock_ticker_save_config();
            mainbar_jump_back();
            break;
    }
}
