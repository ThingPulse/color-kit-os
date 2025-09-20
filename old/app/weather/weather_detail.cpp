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
#include "weather_detail.h"
#include "i18n/weather_i18n.h"

lv_obj_t *weather_image_tile = NULL;
uint32_t weather_image_tile_num;

lv_obj_t * detail_exit_btn = NULL;

lv_obj_t * hour_forecast_container[MAX_FORECAST_HOURS];
lv_obj_t * hour_forecast_icon[MAX_FORECAST_HOURS];
lv_obj_t * hour_forecast_description[MAX_FORECAST_HOURS];
lv_obj_t * hour_forecast_pop[MAX_FORECAST_HOURS];
lv_obj_t * hour_forecast_time[MAX_FORECAST_HOURS];
lv_obj_t * hour_forecast_temp[MAX_FORECAST_HOURS];


static void exit_detail_weather_widget_event_cb( lv_obj_t * obj, lv_event_t event );


void weather_image_tile_setup( uint32_t tile_num ) {
    weather_image_tile_num = tile_num;
    weather_image_tile = mainbar_get_tile_obj( weather_image_tile_num );
    create_screen_detail(weather_image_tile);

    detail_exit_btn = wf_add_exit_button( weather_image_tile, exit_detail_weather_widget_event_cb );
    lv_obj_align(detail_exit_btn, weather_image_tile, LV_ALIGN_IN_BOTTOM_LEFT, THEME_PADDING, -THEME_PADDING );
    lv_obj_t *src, *clone;

    hour_forecast_container[0] = objects.detail_container_template;
    hour_forecast_icon[0] = objects.detail_image_template;
    hour_forecast_description[0] = objects.detail_description_label_template;
    hour_forecast_pop[0] = objects.detail_label_pop_template;
    hour_forecast_time[0] = objects.detail_time_label_template;
    hour_forecast_temp[0] = objects.detail_temp_label_template;

    for (int i = 1; i < MAX_FORECAST_HOURS; i++) {
        src = objects.detail_container_template;
        hour_forecast_container[i] = lv_cont_create(weather_image_tile, src);
        clone = hour_forecast_container[i];
        lv_obj_set_size(clone, lv_obj_get_width(src), lv_obj_get_height(src));
        lv_obj_set_pos(clone, lv_obj_get_x(src), lv_obj_get_y(src) + i * lv_obj_get_height(src));

        src = objects.detail_image_template;
        hour_forecast_icon[i] = lv_img_create(hour_forecast_container[i], src);
        clone = hour_forecast_icon[i];
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

        src = objects.detail_separator_template;
        clone = lv_line_create(hour_forecast_container[i], src);

    }
}

void update_weather_detail(weather_config_t *weather_config, weather_forcast_t * hourly_forecast) {
    lv_label_set_text(objects.detail_title_label, get_string(STR_FORECAST_DETAIL));
    char buf[32] = "";
    const char* weather_units_symbol = weather_config->imperial ? "F" : "C";
    for (int i = 0; i < MAX_FORECAST_HOURS; i++) {
        time_t forecastTimeUtc = hourly_forecast[i].timestamp;
        struct tm *forecastLocalTime = localtime(&forecastTimeUtc);
        uint8_t hour = forecastLocalTime->tm_hour;
        snprintf( buf, sizeof( buf ),"%02d:00", hour );
        lv_label_set_text( hour_forecast_time[i], buf );

        lv_img_set_src( hour_forecast_icon[i],  (lv_obj_t*)resolve_owm_icon( hourly_forecast[i].icon, true ));
        lv_img_set_pivot(hour_forecast_icon[i], 0, 0);
        lv_img_set_zoom(hour_forecast_icon[i],  lv_img_get_zoom(objects.detail_image_template));

        snprintf( buf, sizeof( buf ),"%0.0f%%",  hourly_forecast[i].pop);
        lv_label_set_text( hour_forecast_pop[i], buf );

        lv_label_set_text(hour_forecast_description[i], hourly_forecast[i].description);

        snprintf( buf, sizeof( buf ),"%0.1f°%s",  hourly_forecast[i].temp, weather_units_symbol);
        lv_label_set_text(hour_forecast_temp[i], buf);

    }
    
}

static void exit_detail_weather_widget_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       weather_jump_to_forecast();
                                        break;
    }
}



