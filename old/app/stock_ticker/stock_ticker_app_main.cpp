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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "stock_ticker_app.h"
#include "stock_ticker_app_main.h"

#include "gui/mainbar/app_tile/app_tile.h"
#include "gui/mainbar/main_tile/main_tile.h"
#include "gui/mainbar/mainbar.h"
#include "gui/statusbar.h"
#include "gui/widget_factory.h"
#include "gui/widget_styles.h"
#include "ui/screens.h"
#include "utils/alloc.h"
#include "stock_ticker_fetch.h"

using namespace stock_ticker;

lv_obj_t *stock_ticker_app_main_tile = NULL;
uint32_t stock_ticker_tile_num;

LV_FONT_DECLARE(Ubuntu_12px);
LV_FONT_DECLARE(Ubuntu_32px);

lv_obj_t * exit_stock_ticker_btn = NULL;
lv_obj_t * setup_stock_ticker_btn = NULL;
lv_obj_t * reload_stock_ticker_btn = NULL;

lv_obj_t * stock_ticker_symbol_container[MAX_STOCK_TICKER_SYMBOLS];
lv_obj_t * stock_ticker_symbol_label[MAX_STOCK_TICKER_SYMBOLS];
lv_obj_t * stock_ticker_current_price_label[MAX_STOCK_TICKER_SYMBOLS];
lv_obj_t * stock_ticker_change_label[MAX_STOCK_TICKER_SYMBOLS];
lv_obj_t * stock_ticker_change_percentage_label[MAX_STOCK_TICKER_SYMBOLS];

stock_ticker_data_t * stock_ticker_data;



static void exit_stock_ticker_widget_event_cb( lv_obj_t * obj, lv_event_t event );
static void setup_stocker_ticker_widget_event_cb( lv_obj_t * obj, lv_event_t event );
static void refresh_stock_ticker_widget_event_cb( lv_obj_t * obj, lv_event_t event );

void stock_ticker_app_main_setup( uint32_t tile_num ) {

    stock_ticker_app_main_tile = mainbar_get_tile_obj( tile_num );
    stock_ticker_tile_num = tile_num;

    stock_ticker_data = (stock_ticker_data_t*)CALLOC_ASSERT( sizeof( stock_ticker_data_t ) * MAX_STOCK_TICKER_SYMBOLS , 1, "weather forecast calloc faild" );

    create_screen_main(stock_ticker_app_main_tile);

    exit_stock_ticker_btn = wf_add_exit_button( stock_ticker_app_main_tile, exit_stock_ticker_widget_event_cb );
    lv_obj_align(exit_stock_ticker_btn, stock_ticker_app_main_tile, LV_ALIGN_IN_BOTTOM_LEFT, THEME_PADDING, -THEME_PADDING );

    setup_stock_ticker_btn = wf_add_setup_button( stock_ticker_app_main_tile, setup_stocker_ticker_widget_event_cb );
    lv_obj_align(setup_stock_ticker_btn, stock_ticker_app_main_tile, LV_ALIGN_IN_BOTTOM_RIGHT, -THEME_PADDING, -THEME_PADDING );

    reload_stock_ticker_btn = wf_add_refresh_button( stock_ticker_app_main_tile, refresh_stock_ticker_widget_event_cb );
    lv_obj_align(reload_stock_ticker_btn, stock_ticker_app_main_tile, LV_ALIGN_IN_TOP_RIGHT, -THEME_PADDING , THEME_PADDING );

    lv_obj_t * src, * clone;

    stock_ticker_symbol_container[0] = objects.symbol_container_template;
    stock_ticker_symbol_label[0] = objects.symbol_label_template;
    stock_ticker_current_price_label[0] = objects.current_price_label_template;
    stock_ticker_change_label[0] = objects.change_label_template;
    stock_ticker_change_percentage_label[0] = objects.change_percentage_label_template;

    for (int i = 1; i < MAX_STOCK_TICKER_SYMBOLS; i++) {
        stock_ticker_symbol_container[i] = lv_cont_create(stock_ticker_app_main_tile, objects.symbol_container_template);
        clone = stock_ticker_symbol_container[i];
        src = objects.symbol_container_template;
        lv_obj_set_size(clone, lv_obj_get_width(src), lv_obj_get_height(src));
        lv_obj_set_pos(clone, lv_obj_get_x(src), lv_obj_get_y(src) + i * (lv_obj_get_height(src)));

        stock_ticker_symbol_label[i] = lv_label_create(stock_ticker_symbol_container[i], objects.symbol_label_template);
        stock_ticker_current_price_label[i] = lv_label_create(stock_ticker_symbol_container[i], objects.current_price_label_template);
        stock_ticker_change_label[i] = lv_label_create(stock_ticker_symbol_container[i], objects.change_label_template);
        stock_ticker_change_percentage_label[i] = lv_label_create(stock_ticker_symbol_container[i], objects.change_percentage_label_template);

    }

    stock_ticker_update_data();
}

void stock_ticker_update_data( void ) {
    stock_ticker_fetch_symbols(stock_ticker_data);
    stock_ticker_config_t * config = get_stock_ticker_config();
    for (int i = 0; i < MAX_STOCK_TICKER_SYMBOLS; i++) {
        if (config->stock_symbols[i][0] != '\0') {
            lv_label_set_text(stock_ticker_symbol_label[i], config->stock_symbols[i]);
            lv_obj_set_hidden(stock_ticker_symbol_container[i], false);
        } else {
            lv_obj_set_hidden(stock_ticker_symbol_container[i], true);
        }
    }
}


static void exit_stock_ticker_widget_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       mainbar_jump_to_maintile(true);
                                        break;
    }
}

static void setup_stocker_ticker_widget_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       stock_ticker_jump_to_setup();
                                        break;
    }
}

static void refresh_stock_ticker_widget_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       /*weather_sync_request();*/
                                        break;
    }
}
