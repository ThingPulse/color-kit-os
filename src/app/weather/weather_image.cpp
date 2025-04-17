#include "config.h"
#include "weather.h"
#include "weather_fetch.h"
#include "images/resolve_owm_icon.h"
#include "gui/gui.h"
#include "gui/mainbar/mainbar.h"
#include "gui/mainbar/main_tile/main_tile.h"
#include "gui/statusbar.h"
#include "gui/keyboard.h"
#include "gui/widget_factory.h"
#include "gui/widget_styles.h"
#include "gui/widget.h"
#include "hardware/powermgm.h"
#include "hardware/wifictl.h"
#include "utils/alloc.h"
#include "ui/screens.h"
#include "ui/images.h"
#include "weather_image.h"

#define FORECAST_CONTAINER_NUM 5

lv_obj_t *weather_image_tile = NULL;
uint32_t weather_image_tile_num;

lv_obj_t * detail_exit_btn = NULL;

lv_obj_t * hour_forecast_container[FORECAST_CONTAINER_NUM];
lv_obj_t * hour_forecast_img[FORECAST_CONTAINER_NUM];
lv_obj_t * hour_forecast_description[FORECAST_CONTAINER_NUM];
lv_obj_t * hour_forecast_pop[FORECAST_CONTAINER_NUM];
lv_obj_t * hour_forecast_time[FORECAST_CONTAINER_NUM];
lv_obj_t * hour_forecast_temp[FORECAST_CONTAINER_NUM];


static void exit_detail_weather_widget_event_cb( lv_obj_t * obj, lv_event_t event );


void weather_image_tile_setup( uint32_t tile_num ) {
    weather_image_tile_num = tile_num;
    weather_image_tile = mainbar_get_tile_obj( weather_image_tile_num );
    create_screen_detail(weather_image_tile);

    detail_exit_btn = wf_add_exit_button( weather_image_tile, exit_detail_weather_widget_event_cb );
    lv_obj_align(detail_exit_btn, weather_image_tile, LV_ALIGN_IN_BOTTOM_LEFT, THEME_PADDING, -THEME_PADDING );
    lv_obj_t *src, *clone;

    hour_forecast_container[0] = objects.detail_container_template;
    hour_forecast_img[0] = objects.detail_image_template;
    hour_forecast_description[0] = objects.detail_description_label_template;
    hour_forecast_pop[0] = objects.detail_label_pop_template;
    hour_forecast_time[0] = objects.detail_time_label_template;
    hour_forecast_temp[0] = objects.detail_temp_label_template;

    for (int i = 1; i < FORECAST_CONTAINER_NUM; i++) {
        src = objects.detail_container_template;
        hour_forecast_container[i] = lv_cont_create(weather_image_tile, src);
        clone = hour_forecast_container[i];
        lv_obj_set_size(clone, lv_obj_get_width(src), lv_obj_get_height(src));
        lv_obj_set_pos(clone, lv_obj_get_x(src), lv_obj_get_y(src) + i * lv_obj_get_height(src));

        src = objects.detail_image_template;
        hour_forecast_img[i] = lv_img_create(hour_forecast_container[i], src);
        clone = hour_forecast_img[i];
        lv_obj_set_size(clone, lv_obj_get_width(src), lv_obj_get_height(src));
        lv_obj_set_pos(clone, lv_obj_get_x(src), lv_obj_get_y(src));
        lv_img_set_pivot(clone, 0, 0);
        lv_img_set_zoom(clone, lv_img_get_zoom(clone));

        src = objects.detail_description_label_template;
        clone = lv_label_create(hour_forecast_container[i], src);
        hour_forecast_description[i] = clone;

        src = objects.detail_label_pop_template;
        clone = lv_label_create(hour_forecast_container[i], src);
        hour_forecast_pop[i] = clone;

        src = objects.detail_time_label_template;
        clone = lv_label_create(hour_forecast_container[i], src);
        hour_forecast_time[i] = clone;

        src = objects.detail_temp_label_template;
        clone = lv_label_create(hour_forecast_container[i], src);
        hour_forecast_temp[i] = clone;

    }
}

static void exit_detail_weather_widget_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       weather_jump_to_forecast();
                                        break;
    }
}



