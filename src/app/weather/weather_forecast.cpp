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
#include "weather.h"
#include "weather_fetch.h"
#include "weather_forecast.h"
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
#include "sunmoon/SunMoonCalc.h"
#include "sunmoon/util.h"
#include "i18n/weather_i18n.h"
#include "weather_detail.h"

#ifdef NATIVE_64BIT
    #include "utils/logging.h"
    #include "utils/millis.h"
#else
    #include <Arduino.h>
    #include "esp_task_wdt.h"

    EventGroupHandle_t weather_forecast_event_handle = NULL;
    TaskHandle_t _weather_forecast_sync_Task;
#endif

lv_obj_t *weather_forecast_tile = NULL;
uint32_t weather_forecast_tile_num;
lv_obj_t * exit_btn = NULL;
lv_obj_t * setup_btn = NULL;
lv_obj_t * reload_btn = NULL;

lv_obj_t *weather_forecast_location_label = NULL;
lv_obj_t *weather_forecast_update_label = NULL;
lv_obj_t *weather_forecast_time_label[ WEATHER_MAX_FORECAST ];
lv_obj_t *weather_forecast_icon_imgbtn[ WEATHER_MAX_FORECAST ];
lv_obj_t *weather_forecast_temperature_label[ WEATHER_MAX_FORECAST ];
lv_obj_t *weather_forecast_wind_label[ WEATHER_MAX_FORECAST ];

lv_obj_t * current_weather_icon = NULL;
lv_obj_t * current_temp_label = NULL;
lv_obj_t * current_description_label = NULL;
lv_obj_t * current_humidity_label = NULL;
lv_obj_t * current_pressure_label = NULL;
lv_obj_t * spinner = NULL;

static weather_forcast_t *weather_forecast = NULL;
static weather_forcast_t *hourly_forecast = NULL;
static weather_forcast_t weather_today;

static void weather_update_task( lv_task_t * task );

lv_task_t * weather_tile_task;

icon_t * weather_widget = NULL;

typedef struct {
    uint32_t last_update_time; // Tracks the last update time
} WeatherTaskData;

LV_IMG_DECLARE(refresh_32px);
LV_IMG_DECLARE(owm01d_64px);

lv_style_t weather_current_temp_style;
LV_FONT_DECLARE(Ubuntu_48px);



static void weather_forecast_activate_cb( void );
bool weather_button_event_cb( EventBits_t event, void *arg );
void weather_forecast_sync_Task( void * pvParameters );
bool weather_forecast_wifictl_event_cb( EventBits_t event, void *arg );
static void exit_weather_widget_event_cb( lv_obj_t * obj, lv_event_t event );
static void setup_weather_widget_event_cb( lv_obj_t * obj, lv_event_t event );
static void refresh_weather_widget_event_cb( lv_obj_t * obj, lv_event_t event );


const char* windIcons[] = { 
  "wind-n", "wind-nne", "wind-ne", "wind-ene", "wind-e", "wind-ese", "wind-se", "wind-sse", 
  "wind-s", "wind-ssw", "wind-sw", "wind-wsw", "wind-w", "wind-wnw", "wind-nw", "wind-nnw"
};

string_id_t WEEKDAYS_TEXT_KEYS[] = {STR_SUNDAY, STR_MONDAY, STR_TUESDAY, STR_WEDNESDAY, STR_THURSDAY, STR_FRIDAY, STR_SATURDAY};
string_id_t MOON_PHASE_KEYS[] = {STR_NEW_MOON, STR_WAXING_CRESCENT, STR_FIRST_QUARTER, STR_WAXING_GIBBOUS,
    STR_FULL_MOON, STR_WANING_GIBBOUS, STR_LAST_QUARTER, STR_WANING_CRESCENT};

string_id_t MONTH_KEYS[] = {    STR_JANUARY, STR_FEBRUARY, STR_MARCH,STR_APRIL,STR_MAY,STR_JUNE,STR_JULY,STR_AUGUST,  STR_SEPTEMBER,STR_OCTOBER,STR_NOVEMBER,STR_DECEMBER};

lv_task_t * _weather_app_task = NULL;

lv_obj_t* forecast_icons[4];
lv_obj_t* forecast_temp_label[4];
lv_obj_t* forecast_day_label[4];

lv_img_dsc_t moon_phases[] = {
    img_moon_phase_0,
    img_moon_phase_1,
    img_moon_phase_2,
    img_moon_phase_3,
    img_moon_phase_4,
    img_moon_phase_5,
    img_moon_phase_6,
    img_moon_phase_7,
    img_moon_phase_8,
    img_moon_phase_9,
    img_moon_phase_10,
    img_moon_phase_11,
    img_moon_phase_12,
    img_moon_phase_13,
    img_moon_phase_14,
    img_moon_phase_15,
    img_moon_phase_16,
    img_moon_phase_17,
    img_moon_phase_18,
    img_moon_phase_19,
    img_moon_phase_20,
    img_moon_phase_21,
    img_moon_phase_22,
    img_moon_phase_23,
    img_moon_phase_24,
    img_moon_phase_25,
    img_moon_phase_26,
    img_moon_phase_27,
    img_moon_phase_28,
    img_moon_phase_29,
    img_moon_phase_30,
    img_moon_phase_31

};


void weather_widget_setup() {

    weather_config_t *weather_config = weather_get_config();
    // register app and widget icon
    if ( weather_config->widget ) {
        weather_add_widget();
    }

    if( weather_config->showWind ) {
        widget_set_extended_label( weather_widget, "0" );
    }
}

void weather_forecast_tile_setup( uint32_t tile_num ) {
    log_i("weather_forecast_tile_setup");

    weather_forecast = (weather_forcast_t*)CALLOC_ASSERT( sizeof( weather_forcast_t ) * WEATHER_MAX_FORECAST , 1, "weather forecast calloc faild" );
    hourly_forecast = (weather_forcast_t*)CALLOC_ASSERT( sizeof( weather_forcast_t ) * MAX_FORECAST_HOURS , 1, "hourly forecast calloc faild" );

    weather_forecast_tile_num = tile_num;
    weather_forecast_tile = mainbar_get_tile_obj( weather_forecast_tile_num );
    create_screen_main(weather_forecast_tile);

    exit_btn = wf_add_exit_button( weather_forecast_tile, exit_weather_widget_event_cb );
    lv_obj_align(exit_btn, weather_forecast_tile, LV_ALIGN_IN_BOTTOM_LEFT, THEME_PADDING, -THEME_PADDING );

    setup_btn = wf_add_setup_button( weather_forecast_tile, setup_weather_widget_event_cb );
    lv_obj_align(setup_btn, weather_forecast_tile, LV_ALIGN_IN_BOTTOM_RIGHT, -THEME_PADDING, -THEME_PADDING );

    reload_btn = wf_add_refresh_button( weather_forecast_tile, refresh_weather_widget_event_cb );
    lv_obj_align(reload_btn, weather_forecast_tile, LV_ALIGN_IN_TOP_RIGHT, -THEME_PADDING , THEME_PADDING );

    lv_style_copy( &weather_current_temp_style, APP_STYLE );
    lv_style_set_text_font( &weather_current_temp_style, LV_STATE_DEFAULT, &Ubuntu_48px);

    lv_obj_set_style_local_bg_color(weather_forecast_tile, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x00000000));

    forecast_icons[0] = objects.image_day0;
    forecast_icons[1] = objects.image_day1;
    forecast_icons[2] = objects.image_day2;
    forecast_icons[3] = objects.image_day3;

    forecast_temp_label[0] = objects.label_temp_day0;
    forecast_temp_label[1] = objects.label_temp_day1;
    forecast_temp_label[2] = objects.label_temp_day2;
    forecast_temp_label[3] = objects.label_temp_day3;

    forecast_day_label[0] = objects.label_day0;
    forecast_day_label[1] = objects.label_day1;
    forecast_day_label[2] = objects.label_day2;
    forecast_day_label[3] = objects.label_day3;

    spinner = lv_spinner_create(weather_forecast_tile, NULL);
    lv_spinner_set_spin_time(spinner, 1000);
    lv_obj_set_pos(spinner, 5, 5);
    lv_obj_set_size(spinner, 80, 80);
    lv_obj_set_hidden(spinner, true);


    mainbar_add_tile_button_cb( weather_forecast_tile_num, weather_button_event_cb );
    mainbar_add_tile_activate_cb( weather_forecast_tile_num, weather_forecast_activate_cb );

    weather_tile_task = lv_task_create( weather_update_task, 500, LV_TASK_PRIO_MID, NULL );

}

static void weather_forecast_activate_cb( void ) {
    log_i("weather_forecast_activate_cb");
    wf_image_button_fade_in( exit_btn, 500, 0 );
    wf_image_button_fade_in( setup_btn, 500, 100 );
    wf_image_button_fade_in( reload_btn, 500, 200 );
    weather_forecast_sync();
    return;
    WeatherTaskData *data = (WeatherTaskData *)malloc(sizeof(WeatherTaskData));
    data->last_update_time = 0; // Initialize last update time
    if( !_weather_app_task ) {
            _weather_app_task = lv_task_create( weather_app_task, 10000, LV_TASK_PRIO_MID, data );
    }

}

bool weather_button_event_cb( EventBits_t event, void *arg ) {
    switch( event ) {
        case BUTTON_EXIT:           
                                    if( _weather_app_task )
                                            lv_task_del(_weather_app_task);
                                    mainbar_jump_to_maintile(true);
                                    break;
        case BUTTON_SETUP:          weather_jump_to_setup();
                                    break;
        case BUTTON_REFRESH:        weather_sync_request();
                                    break;
    }
    return( true );
}

static void exit_weather_widget_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       mainbar_jump_to_maintile(true);
                                        break;
    }
}

static void setup_weather_widget_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       weather_jump_to_setup();
                                        break;
    }
}

static void refresh_weather_widget_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       weather_sync_request();
                                        break;
    }
}

void update_time() {
    time_t now;
    static time_t last = 0;
    struct tm info, last_info;
    char buf[64];

    time( &now );
    localtime_r( &now, &info );

    if ( last != 0 ) {
        localtime_r( &last, &last_info );
    }

    if ( last == 0 || info.tm_min != last_info.tm_min || info.tm_hour != last_info.tm_hour) {
        
        snprintf(buf, sizeof(buf), "%s, %d. %s %d", get_string(WEEKDAYS_TEXT_KEYS[info.tm_wday]), info.tm_mday, get_string(MONTH_KEYS[info.tm_mon]), 1900 + info.tm_year);
        lv_label_set_text( objects.label_date, buf );
        strftime( buf, sizeof(buf), "%H:%M", &info );
        lv_label_set_text( objects.label_time, buf );
        log_i("Update time: %s", buf);
        last = now;
    }
}

static void weather_update_task( lv_task_t * task ) {
    update_time();

    static unsigned long last = 0;
   
    if( (millis() - last) > 4 * 60 * 1000 ) {
        weather_sync_request();
        last = millis();
    }
}

void weather_forecast_sync( void  ) {
    log_i("weather_forecast_sync");
    weather_config_t *weather_config = weather_get_config();
    int32_t retval = -1;

    const char* weather_units_symbol = weather_config->imperial ? "F" : "C";
    const char* weather_units_char = weather_config->imperial ? "imperial" : "metric";
    const char* weather_wind_speed_symbol = weather_config->imperial ? "mph" : "m/s";

    update_time();

    
    retval = weather_fetch_today( weather_config , &weather_today );
    //lv_obj_set_hidden(objects.today_container, true);

    retval = weather_fetch_forecast( weather_get_config() , &weather_forecast[ 0 ], &hourly_forecast[0] );

    //lv_obj_set_hidden(objects.forecast_container, true);

    //retval = weather_fetch_onecall( weather_config, &weather_today, &weather_forecast[ 0 ]);
    char buf[64];

    if (weather_today.valide) {
        lv_obj_set_hidden(objects.today_container, false);
        lv_obj_set_hidden(objects.message_container, true);
        
        snprintf( buf, sizeof( buf ),"%0.1f°%s", weather_today.temp, weather_units_symbol );
        widget_set_label( weather_widget, buf );
        lv_label_set_text( objects.label_current_temperature, buf );
        snprintf( buf, sizeof( buf ),"%0.0f%%", weather_today.humidity );
        widget_set_icon( weather_widget,  (lv_obj_t*)resolve_owm_icon( weather_today.icon, true ));

        snprintf( buf, sizeof( buf ),"%d%s %d°", weather_today.wind_sp33d, weather_wind_speed_symbol, weather_today.wind_deg );
        lv_label_set_text(objects.label_wind_speed, buf);
        if ( weather_config->showWind ) {
            widget_set_extended_label( weather_widget, buf );
            
        }
        else {
            widget_set_extended_label( weather_widget, "" );
        }
        

        lv_img_set_src(objects.image_current_weather, resolve_owm_icon( weather_today.icon, false ));
        
        lv_label_set_text( objects.label_current_description, weather_today.description );
        snprintf( buf, sizeof( buf ),"%0.0f%%", weather_today.humidity );
        lv_label_set_text( objects.label_humidity, buf );
        snprintf( buf, sizeof( buf ),"%0.0fhPa", weather_today.pressure );
        lv_label_set_text( objects.label_pressure, buf );
    } else {
        lv_obj_set_hidden(objects.today_container, true);
        lv_obj_set_hidden(objects.message_container, false);
    }

    if (weather_forecast[0].valide) {
        update_weather_detail(weather_config, hourly_forecast);
        lv_obj_set_hidden(objects.forecast_container, false);
        struct tm *forecastLocalTime; 
        for ( int i = 0; i < 4; i++) {
            forecastLocalTime = localtime(&weather_forecast[i].timestamp);
            lv_label_set_text(forecast_day_label[i], get_string(WEEKDAYS_TEXT_KEYS[forecastLocalTime->tm_wday]));
            lv_img_set_src( forecast_icons[i], resolve_owm_icon(weather_forecast[ i ].icon, true) );
            snprintf( buf, sizeof( buf ),"%0.0f|%0.0f°%s", weather_forecast[i].temp_min, weather_forecast[i].temp_max, weather_units_symbol );
            lv_label_set_text(forecast_temp_label[i], buf);

        }

        

        int windAngleIndex = round(weather_today.wind_deg * 16 / 360);
        if (windAngleIndex > 15) windAngleIndex = 0;
        lv_img_set_src(objects.image_wind, getLvglImageByName(windIcons[windAngleIndex]));
    } else {
        lv_obj_set_hidden(objects.forecast_container, true);
    }



    time_t tnow = time(nullptr);
    struct tm *nowUtc = gmtime(&tnow);
    struct tm nowLocal;
    localtime_r( &tnow, &nowLocal );

    if (weather_today.valide) {
        strftime( buf, sizeof(buf), "%H:%M", &nowLocal );
        lv_label_set_text( objects.last_update, buf );
    } else {
        lv_label_set_text( objects.last_update, "ERROR" );
    }

    char *endptr; 

    SunMoonCalc smCalc(mkgmtime(nowUtc), strtod(weather_config->lat, &endptr), strtod(weather_config->lon, &endptr));
    const SunMoonCalc::Result result = smCalc.calculateSunAndMoonData();


    char timestampBuffer[26];
    // Sun
    lv_label_set_text(objects.label_sun, get_string(STR_SUN));
    strftime(timestampBuffer, 26, UI_TIME_FORMAT_NO_SECONDS, localtime(&result.sun.rise));
    lv_label_set_text(objects.label_sunrise, timestampBuffer);

    strftime(timestampBuffer, 26, UI_TIME_FORMAT_NO_SECONDS, localtime(&result.sun.set));
    lv_label_set_text(objects.label_sunset, timestampBuffer);

    // Moon
    lv_label_set_text(objects.label_moon, get_string(STR_MOON));
    strftime(timestampBuffer, 26, UI_TIME_FORMAT_NO_SECONDS, localtime(&result.moon.rise));
    lv_label_set_text(objects.label_moonrise, timestampBuffer);
    strftime(timestampBuffer, 26, UI_TIME_FORMAT_NO_SECONDS, localtime(&result.moon.set));
    lv_label_set_text(objects.label_moonset, timestampBuffer);

    // Moon icon
    int imageIndex = round(result.moon.age * NUMBER_OF_MOON_IMAGES / LUNAR_MONTH);
    if (imageIndex == NUMBER_OF_MOON_IMAGES) imageIndex = NUMBER_OF_MOON_IMAGES - 1;

    // Moon label
    lv_label_set_text(objects.label_moon_phase, get_string(MOON_PHASE_KEYS[result.moon.phase.index]));

    lv_img_set_src(objects.image_moon, &moon_phases[imageIndex]);
    //lv_obj_set_hidden(spinner, true);

    
    lv_obj_invalidate( lv_scr_act() );
    
}

void weather_add_widget( void ) {
    weather_widget = widget_register( "n/a", &img_weather_fog_64px, enter_weather_widget_event_cb );
}

void weather_remove_widget( void ) {
    weather_widget = widget_remove( weather_widget );
}


