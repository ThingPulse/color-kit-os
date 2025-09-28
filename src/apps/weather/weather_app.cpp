
#include "weather_app.h"
#include "system/ui/screens.h"
#include "lvgl.h"
#include <time.h>
#include "weather_fetch.h"
#include "weather_icons.h"
#include "sunmoon/SunMoonCalc.h"
#include <stdlib.h>
#include "hardware/wifictl.h"
#include "system/ui/actions.h"


#ifdef NATIVE_64BIT
    #include "utils/logging.h"
    #include "utils/millis.h"
#else
    #include <Arduino.h>
#endif

static weather_forecast_t weather_today;

LV_IMG_DECLARE(img_moon_phase_0);
LV_IMG_DECLARE(img_moon_phase_1);
LV_IMG_DECLARE(img_moon_phase_2);
LV_IMG_DECLARE(img_moon_phase_3);
LV_IMG_DECLARE(img_moon_phase_4);
LV_IMG_DECLARE(img_moon_phase_5);
LV_IMG_DECLARE(img_moon_phase_6);
LV_IMG_DECLARE(img_moon_phase_7);

const void* moon_phases[] = {
    &img_moon_phase_0,
    &img_moon_phase_1,
    &img_moon_phase_2,
    &img_moon_phase_3,
    &img_moon_phase_4,
    &img_moon_phase_5,
    &img_moon_phase_6,
    &img_moon_phase_7,
};

const char* MOON_PHASE_NAMES[] = {
    "New Moon", 
    "Waxing Crescent", 
    "First Quarter", 
    "Waxing Gibbous", 
    "Full Moon", 
    "Waning Gibbous", 
    "Last Quarter", 
    "Waning Crescent"
};

lv_timer_t * weather_timer;

/**
 * @brief LVGL timer callback to update time and date labels.
 * 
 * This function is executed by an LVGL timer to periodically refresh the
 * time and date displayed on the weather screen.
 * 
 * @param timer Pointer to the LVGL timer that triggered the callback.
 */
static void update_datetime_task(lv_timer_t * timer) {
    if (is_wifi_connected()) {
        time_t now;
        struct tm timeinfo;
        char time_buffer[9]; // HH:MM:SS\0
        char date_buffer[11]; // YYYY-MM-DD\0

        time(&now);
        localtime_r(&now, &timeinfo);

        strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", &timeinfo);
        strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d", &timeinfo);

        lv_label_set_text(objects.weather_label_time, time_buffer);
        lv_label_set_text(objects.weather_label_date, date_buffer);
    }
}

/**
 * @brief LVGL timer callback to fetch and update weather data.
 * 
 * This function is executed by an LVGL timer to periodically fetch weather
 * data from the OpenWeatherMap API and update the UI.
 * 
 * @param timer Pointer to the LVGL timer that triggered the callback.
 */
static void update_weather(void *data) {
        log_i("Fetching weather data...");

        // TODO: These settings should be made configurable
        weather_config_t config = {
            .imperial = false,
            .lang = "en",
            .lat = "47.3769",
            .lon = "8.5417",
            .apikey = "f42c672cab129e0611ceed9f0b23ab4a" // IMPORTANT: Replace with your OpenWeatherMap API key
        };

        if (weather_fetch_today(&config, &weather_today) == 200 && weather_today.valide) {
            log_i("Weather fetch successful. Temp: %.1f", weather_today.temp);
            
            char temp_buffer[10];
            snprintf(temp_buffer, sizeof(temp_buffer), "%.1f°", weather_today.temp);

            char wind_buffer[12];
            snprintf(wind_buffer, sizeof(wind_buffer), "%d m/s", weather_today.wind_speed);

            char humidity_buffer[8];
            snprintf(humidity_buffer, sizeof(humidity_buffer), "%.0f %%", weather_today.humidity);

            char pressure_buffer[12];
            snprintf(pressure_buffer, sizeof(pressure_buffer), "%.0f hPa", weather_today.pressure);

            //lv_label_set_text(objects.weather_label_city, weather_today.name);
            lv_label_set_text(objects.weather_label_current_temp, temp_buffer);
            lv_label_set_text(objects.weather_label_current_description, weather_today.description);
            lv_img_set_src(objects.weather_image_current_weather, resolve_owm_icon(weather_today.icon));
            lv_label_set_text(objects.weather_label_wind_speed, wind_buffer);
            lv_label_set_text(objects.weather_label_humidity, humidity_buffer);
            lv_label_set_text(objects.weather_label_pressure, pressure_buffer);

            // Angle is in 1/10 degree, OWM gives it in degrees
            lv_img_set_angle(objects.weather_image_wind_direction, (weather_today.wind_deg + 180) * 10);

            // Sun and Moon calculations
            time_t tnow = time(nullptr);
            char *endptr;
            SunMoonCalc smCalc(tnow, strtod(config.lat, &endptr), strtod(config.lon, &endptr));
            const SunMoonCalc::Result result = smCalc.calculateSunAndMoonData();

            char timestampBuffer[6]; // HH:MM

            // Sun
            struct tm* sun_rise_tm = localtime(&result.sun.rise);
            strftime(timestampBuffer, sizeof(timestampBuffer), "%H:%M", sun_rise_tm);
            lv_label_set_text(objects.label_sunrise, timestampBuffer);

            struct tm* sun_set_tm = localtime(&result.sun.set);
            strftime(timestampBuffer, sizeof(timestampBuffer), "%H:%M", sun_set_tm);
            lv_label_set_text(objects.label_sunset, timestampBuffer);

            // Moon
            struct tm* moon_rise_tm = localtime(&result.moon.rise);
            strftime(timestampBuffer, sizeof(timestampBuffer), "%H:%M", moon_rise_tm);
            lv_label_set_text(objects.label_moonrise, timestampBuffer);

            struct tm* moon_set_tm = localtime(&result.moon.set);
            strftime(timestampBuffer, sizeof(timestampBuffer), "%H:%M", moon_set_tm);
            lv_label_set_text(objects.label_moonset, timestampBuffer);

            // Moon phase
            lv_label_set_text(objects.label_moon_phase, MOON_PHASE_NAMES[result.moon.phase.index]);
            lv_img_set_src(objects.image_moon, moon_phases[result.moon.phase.index]);
        } 
    /*} else {
        log_w("Cannot fetch weather, no WiFi connection.");
    }*/
}

void action_on_weather_screen_gesture(lv_event_t * e) {
        // Which object received the event (here: the screen)
    lv_obj_t *scr = (lv_obj_t*) lv_event_get_current_target(e);

    // Read swipe direction from the active input device
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());

    switch(dir) {
        case LV_DIR_TOP: 
            lv_screen_load(objects.homescreen);
            break;
        case LV_DIR_BOTTOM:
            lv_async_call(update_weather, (void*)NULL);
            break;
        case LV_DIR_LEFT:
            break;
        case LV_DIR_RIGHT:
            break;
        default: break;
    }
}

static void update_weather_task(lv_timer_t * timer) {
    static uint32_t last_weather_update = 0;
    if (is_wifi_connected() && millis() - last_weather_update > 1000 * 60 * 10) {
        lv_async_call(update_weather, (void *)NULL);
        last_weather_update = millis();
    }
}

bool update_weather_when_connected(EventBits_t event, void *arg) {
    lv_async_call(update_weather, (void *)NULL);
    return true;
}

void setup_weather_app() {
    // Create a timer to update the time and date every 500ms (2 times per second)
    lv_timer_create(update_datetime_task, 500, NULL);
    // Create a timer to fetch weather data every 10 minutes
    weather_timer = lv_timer_create(update_weather_task, 5000, NULL);
    lv_timer_ready(weather_timer); // Run it once immediately
    wifictl_register_cb( WIFICTL_CONNECT | WIFICTL_CONNECT_IP, update_weather_when_connected, "update weather" );
}