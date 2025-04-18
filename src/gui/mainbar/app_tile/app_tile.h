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
#ifndef _APP_TILE_H
    #define _APP_TILE_H

    #include "gui/icon.h"
    #include "lvgl.h"

    #if defined( M5PAPER )
        #define MAX_APPS_ICON_HORZ      4
        #define MAX_APPS_ICON_VERT      5
        #define APP_ICON_X_CLEARENCE    48
        #define APP_ICON_Y_CLEARENCE    72
        #define APP_ICON_X_OFFSET       0
        #define APP_ICON_Y_OFFSET       0
        #define MAX_APPS_TILES          1
    #elif defined( LILYGO_WATCH_2020_V1 ) || defined( LILYGO_WATCH_2020_V2 ) || defined( LILYGO_WATCH_2020_V3 ) || defined( M5CORE2 )
        #define MAX_APPS_ICON_HORZ      3
        #define MAX_APPS_ICON_VERT      2
        #define APP_ICON_X_CLEARENCE    8
        #define APP_ICON_Y_CLEARENCE    36
        #define APP_ICON_X_OFFSET       0
        #define APP_ICON_Y_OFFSET       0
        #define MAX_APPS_TILES          4
    #elif defined( LILYGO_WATCH_2021 )
        #define MAX_APPS_ICON_HORZ      2
        #define MAX_APPS_ICON_VERT      2
        #define APP_ICON_X_CLEARENCE    4
        #define APP_ICON_Y_CLEARENCE    36
        #define APP_ICON_X_OFFSET       0
        #define APP_ICON_Y_OFFSET       -16
        #define MAX_APPS_TILES          7
    #elif defined( WT32_SC01 )
        #define MAX_APPS_ICON_HORZ      5
        #define MAX_APPS_ICON_VERT      2
        #define APP_ICON_X_CLEARENCE    16
        #define APP_ICON_Y_CLEARENCE    36
        #define APP_ICON_X_OFFSET       0
        #define APP_ICON_Y_OFFSET       0
        #define MAX_APPS_TILES          3
    #elif defined ( CKGPRO )
        #define MAX_APPS_ICON_HORZ      4
        #define MAX_APPS_ICON_VERT      4
        #define APP_ICON_X_CLEARENCE    8
        #define APP_ICON_Y_CLEARENCE    36
        #define APP_ICON_X_OFFSET       0
        #define APP_ICON_Y_OFFSET       0
        #define MAX_APPS_TILES          4 
    #elif defined ( CKGRANDE )
        #define MAX_APPS_ICON_HORZ      4
        #define MAX_APPS_ICON_VERT      4
        #define APP_ICON_X_CLEARENCE    8
        #define APP_ICON_Y_CLEARENCE    36
        #define APP_ICON_X_OFFSET       0
        #define APP_ICON_Y_OFFSET       0
        #define MAX_APPS_TILES          4 
    #else
        #define MAX_APPS_ICON_HORZ      3
        #define MAX_APPS_ICON_VERT      2
        #define APP_ICON_X_CLEARENCE    8
        #define APP_ICON_Y_CLEARENCE    36
        #define APP_ICON_X_OFFSET       0
        #define APP_ICON_Y_OFFSET       0
        #define MAX_APPS_TILES          4
    #endif

    #define MAX_APPS_ICON           ( MAX_APPS_ICON_HORZ * MAX_APPS_ICON_VERT * MAX_APPS_TILES )

    #define APP_ICON_X_SIZE         70
    #define APP_ICON_Y_SIZE         70
    #define APP_LABEL_X_SIZE        APP_ICON_X_SIZE + APP_ICON_X_CLEARENCE
    #define APP_LABEL_Y_SIZE        APP_ICON_Y_CLEARENCE / 2

    #define APP_FIRST_X_POS         ( lv_disp_get_hor_res( NULL ) - ( APP_ICON_X_SIZE * MAX_APPS_ICON_HORZ + APP_ICON_X_CLEARENCE * ( MAX_APPS_ICON_HORZ - 1 ) ) ) / 2
    #define APP_FIRST_Y_POS         ( lv_disp_get_ver_res( NULL ) - ( APP_ICON_Y_SIZE * MAX_APPS_ICON_VERT + APP_ICON_Y_CLEARENCE * ( MAX_APPS_ICON_VERT - 1 ) ) ) / 2

    /**
     * @brief setup the app tile
     */
    void app_tile_setup( void );
    /**
     * @brief register an app icon an the app tile
     * 
     * @param   appname     app name
     * 
     * @return  pointer to lv_obj_t icon container, here you can set your own icon with imgbtn
     */
    lv_obj_t *app_tile_register_app( const char* appname );
    /**
     * @brief   get the an free app icon stucture
     * 
     * @return  pointer to the free lv_app_icon_t structure
     */
    icon_t *app_tile_get_free_app_icon( void );
    /**
     * @brief get the tile number for the app tile
     * 
     * @return  tile number
     */
    uint32_t app_tile_get_tile_num( void );
    /**
     * @brief   get the numbers of active app
     * 
     * @return  zero or the numbers of active apps
     */
    int32_t app_tile_get_active_app_entrys( void );
    const char *app_get_appentrys_name( int32_t appentry );

#endif // _APP_TILE_H