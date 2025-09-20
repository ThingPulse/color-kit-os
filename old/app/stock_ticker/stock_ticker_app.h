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
#ifndef _STOCK_TICKER_APP_H
    #define _STOCK_TICKER_APP_H

    #include "config/stock_ticker_config.h"

    #define MAX_STOCK_TICKER_SYMBOLS 6



    /**
     * @brief setup calc app
     * 
     */
    void stock_ticker_app_setup( void );
    /**
     * @brief get to calc app tile number
     * 
     * @return uint32_t tilenumber
     */
    uint32_t stock_ticker_app_get_app_main_tile_num( void );
    /**
     * @brief call back function when enter the app
     * 
     * @param obj           object
     * @param event         event
     */
    void enter_stock_ticker_app_event_cb( lv_obj_t * obj, lv_event_t event );
    /**
     * @brief call back function when exit the app
     * 
     * @param obj           object
     * @param event         event
     */
    void exit_stock_ticker_app_main_event_cb( lv_obj_t * obj, lv_event_t event );

    void stock_ticker_jump_to_setup( void );

    void stock_ticker_save_config( void );

    void stock_ticker_load_config( void );

    stock_ticker_config_t * get_stock_ticker_config();

#endif // _STOCK_TICKER_APP_H