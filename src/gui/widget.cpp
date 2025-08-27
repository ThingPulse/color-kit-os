/****************************************************************************
 *   Sep 2 08:21:51 2020
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
#include "gui/mainbar/mainbar.h"
#include "gui/mainbar/main_tile/main_tile.h"
#include "widget.h"

#ifdef NATIVE_64BIT
    #include "utils/logging.h"
#else
    #include <Arduino.h>
#endif

icon_t *widget_register( const char* widgetname, const lv_img_dsc_t *icon, lv_event_cb_t event_cb ) {

    icon_t *widget = main_tile_get_free_widget_icon();

    if ( widget == NULL ) {
        log_e("no free widget icon");
        return( NULL );
    }

    widget->active = true;
    // setup label and ext_label
    lv_label_set_text( widget->label, widgetname );
    lv_obj_align( widget->label, LV_ALIGN_BOTTOM_MID, 0, 0 );
    lv_label_set_text( widget->ext_label, "" );
    lv_obj_align_to( widget->ext_label, widget->label, LV_ALIGN_OUT_TOP_MID, 0, 0 );
    // setup icon and set event callback
    lv_obj_t *img = lv_img_create(widget->icon_img);
    lv_img_set_src(img, icon);
    lv_obj_center(img);
    lv_obj_remove_style_all( widget->icon_img );
    lv_obj_align( widget->icon_img, LV_ALIGN_TOP_MID, 0, 0 );
    lv_obj_add_event_cb( widget->icon_img, event_cb, LV_EVENT_ALL, NULL );
    // setup icon indicator
    lv_img_set_src( widget->icon_indicator, &info_ok_16px );
    lv_obj_align( widget->icon_indicator, LV_ALIGN_TOP_RIGHT, 0, 0 );

    lv_obj_clear_flag( widget->icon_cont, LV_OBJ_FLAG_HIDDEN );
    lv_obj_clear_flag( widget->label, LV_OBJ_FLAG_HIDDEN );
    lv_obj_clear_flag( widget->ext_label, LV_OBJ_FLAG_HIDDEN );
    lv_obj_clear_flag( widget->icon_img, LV_OBJ_FLAG_HIDDEN );
    lv_obj_clear_flag( widget->icon_indicator, LV_OBJ_FLAG_HIDDEN );

    mainbar_add_slide_element( widget->icon_cont );
    mainbar_add_slide_element( widget->icon_img );
    main_tile_align_widgets();
    lv_obj_invalidate( lv_scr_act() );

    return( widget );
}

icon_t *widget_remove( icon_t *widget ) {

    if ( widget == NULL ) {
        log_e("no widget icon selected");
        return( NULL );
    }

    widget->active = false;
    lv_obj_add_flag( widget->icon_cont, LV_OBJ_FLAG_HIDDEN );
    lv_obj_add_flag( widget->icon_img, LV_OBJ_FLAG_HIDDEN );
    lv_obj_add_flag( widget->icon_indicator, LV_OBJ_FLAG_HIDDEN );
    lv_obj_add_flag( widget->label, LV_OBJ_FLAG_HIDDEN );
    lv_obj_add_flag( widget->ext_label, LV_OBJ_FLAG_HIDDEN );
    main_tile_align_widgets();
    lv_obj_invalidate( lv_scr_act() );
    return( NULL );
}

void widget_set_indicator( icon_t *widget, icon_indicator_t indicator ) {
    if ( widget == NULL ) {
        return;
    }

    switch( indicator ) {
        case ICON_INDICATOR_OK:      lv_img_set_src( widget->icon_indicator, &info_ok_16px );
                                     break;
        case ICON_INDICATOR_FAIL:    lv_img_set_src( widget->icon_indicator, &info_fail_16px );
                                     break;
        case ICON_INDICATOR_UPDATE:  lv_img_set_src( widget->icon_indicator, &info_update_16px );
                                     break;
        case ICON_INDICATOR_1:       lv_img_set_src( widget->icon_indicator, &info_1_16px );
                                     break;
        case ICON_INDICATOR_2:       lv_img_set_src( widget->icon_indicator, &info_2_16px );
                                     break;
        case ICON_INDICATOR_3:       lv_img_set_src( widget->icon_indicator, &info_3_16px );
                                     break;
        case ICON_INDICATOR_N:       lv_img_set_src( widget->icon_indicator, &info_n_16px );
                                     break;
    }
    lv_obj_align( widget->icon_indicator, LV_ALIGN_TOP_RIGHT, 0, 0 );
    lv_obj_clear_flag( widget->icon_indicator, LV_OBJ_FLAG_HIDDEN );
    lv_obj_invalidate( lv_scr_act() );
}

void widget_hide_indicator( icon_t *widget ) {
    if ( widget == NULL ) {
        return;
    }

    if ( widget->active == false ) {
        return;
    }

    lv_obj_add_flag( widget->icon_indicator, LV_OBJ_FLAG_HIDDEN );
    lv_obj_invalidate( lv_scr_act() );
}

void widget_set_icon( icon_t *widget, const lv_img_dsc_t *icon ) {
    if ( widget == NULL ) {
        return;
    }

    if ( widget->active == false ) {
        return;
    }

    lv_obj_t* img = lv_obj_get_child(widget->icon_img, 0);
    lv_img_set_src(img, icon);
    lv_obj_remove_style_all( widget->icon_img );
    lv_obj_align( widget->icon_img, LV_ALIGN_TOP_LEFT, 0, 0 );
    lv_obj_invalidate( lv_scr_act() );
}

void widget_set_label( icon_t *widget, const char* text ) {
    if ( widget == NULL ) {
        return;
    }

    if ( widget->active == false ) {
        return;
    }

    lv_label_set_text( widget->label, text );
    lv_obj_align( widget->label, LV_ALIGN_BOTTOM_MID, 0, 0 );
    lv_obj_set_style_text_align( widget->label, LV_TEXT_ALIGN_CENTER, 0 );
    lv_obj_invalidate( lv_scr_act() );
}

void widget_set_extended_label( icon_t *widget, const char* text ) {
    if ( widget == NULL ) {
        return;
    }

    if ( widget->active == false ) {
        return;
    }

    lv_label_set_text( widget->ext_label, text );
    lv_obj_align_to( widget->ext_label, widget->label, LV_ALIGN_OUT_TOP_MID, 0, 0 );
    lv_obj_set_style_text_align( widget->ext_label, LV_TEXT_ALIGN_CENTER, 0 );
    lv_obj_invalidate( lv_scr_act() );
}