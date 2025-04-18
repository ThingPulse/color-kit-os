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
#ifndef _SETUP_TILE_H
    #define _SETUP_TILE_H

    #include "gui/icon.h"
    #include "lvgl.h"

    #if defined( M5PAPER )
        #define MAX_SETUP_ICON_HORZ         4
        #define MAX_SETUP_ICON_VERT         5
        #define SETUP_ICON_X_CLEARENCE      48
        #define SETUP_ICON_Y_CLEARENCE      72
        #define SETUP_ICON_X_OFFSET         0
        #define SETUP_ICON_Y_OFFSET         0
        #define MAX_SETUP_TILES             1
    #elif defined( LILYGO_WATCH_2020_V1 ) || defined( LILYGO_WATCH_2020_V2 ) || defined( LILYGO_WATCH_2020_V3 ) || defined( M5CORE2 )
        #define MAX_SETUP_ICON_HORZ         3
        #define MAX_SETUP_ICON_VERT         2
        #define SETUP_ICON_X_CLEARENCE      8
        #define SETUP_ICON_Y_CLEARENCE      36
        #define SETUP_ICON_X_OFFSET         0
        #define SETUP_ICON_Y_OFFSET         0
        #define MAX_SETUP_TILES             4
    #elif defined( LILYGO_WATCH_2021 )
        #define MAX_SETUP_ICON_HORZ         2
        #define MAX_SETUP_ICON_VERT         2
        #define SETUP_ICON_X_CLEARENCE      8
        #define SETUP_ICON_Y_CLEARENCE      36
        #define SETUP_ICON_X_OFFSET         0
        #define SETUP_ICON_Y_OFFSET         -16
        #define MAX_SETUP_TILES             6
    #elif defined( WT32_SC01 )
        #define MAX_SETUP_ICON_HORZ         5
        #define MAX_SETUP_ICON_VERT         2
        #define SETUP_ICON_X_CLEARENCE      16
        #define SETUP_ICON_Y_CLEARENCE      36
        #define SETUP_ICON_X_OFFSET         0
        #define SETUP_ICON_Y_OFFSET         0
        #define MAX_SETUP_TILES             2
    #elif defined ( CKGPRO )
        #define MAX_SETUP_ICON_HORZ         4
        #define MAX_SETUP_ICON_VERT         5
        #define SETUP_ICON_X_CLEARENCE      8
        #define SETUP_ICON_Y_CLEARENCE      36
        #define SETUP_ICON_X_OFFSET         0
        #define SETUP_ICON_Y_OFFSET         36
        #define MAX_SETUP_TILES             4
    #elif defined ( CKGRANDE )
        #define MAX_SETUP_ICON_HORZ         4
        #define MAX_SETUP_ICON_VERT         5
        #define SETUP_ICON_X_CLEARENCE      8
        #define SETUP_ICON_Y_CLEARENCE      36
        #define SETUP_ICON_X_OFFSET         0
        #define SETUP_ICON_Y_OFFSET         36
        #define MAX_SETUP_TILES             4
    #else
        #define MAX_SETUP_ICON_HORZ         3
        #define MAX_SETUP_ICON_VERT         2
        #define SETUP_ICON_X_CLEARENCE      8
        #define SETUP_ICON_Y_CLEARENCE      36
        #define SETUP_ICON_X_OFFSET         0
        #define SETUP_ICON_Y_OFFSET         0
        #define MAX_SETUP_TILES             4
    #endif

    #define MAX_SETUP_ICON            ( MAX_SETUP_ICON_HORZ * MAX_SETUP_ICON_VERT * MAX_SETUP_TILES )

    #define SETUP_ICON_X_SIZE         64
    #define SETUP_ICON_Y_SIZE         64
    #define SETUP_LABEL_X_SIZE        SETUP_ICON_X_SIZE + SETUP_ICON_X_CLEARENCE
    #define SETUP_LABEL_Y_SIZE        SETUP_ICON_Y_CLEARENCE / 2

    #define SETUP_FIRST_X_POS         ( lv_disp_get_hor_res( NULL ) - ( SETUP_ICON_X_SIZE * MAX_SETUP_ICON_HORZ + SETUP_ICON_X_CLEARENCE * ( MAX_SETUP_ICON_HORZ - 1 ) ) ) / 2
    #define SETUP_FIRST_Y_POS         ( lv_disp_get_ver_res( NULL ) - ( SETUP_ICON_Y_SIZE * MAX_SETUP_ICON_VERT + SETUP_ICON_Y_CLEARENCE * ( MAX_SETUP_ICON_VERT - 1 ) ) ) / 2

    /**
     * @brief setup the setup tile
     */
    void setup_tile_setup( void );
    /**
     * @brief register an setup icon an the setup tile
     * 
     * @return  pointer to lv_obj_t icon container, here you can set your own icon with imgbtn
     */
    lv_obj_t *setup_tile_register_setup( void );
    /**
     * @brief get the tile number for the setup tile
     * 
     * @return  tile number
     */
    uint32_t setup_get_tile_num( void );
    /**
     * @brief   get the an free setup icon stucture
     * 
     * @return  pointer to the free setup_icon_t structure
     */
    icon_t *setup_tile_get_free_setup_icon( void );

#endif // _SETUP_TILE_H