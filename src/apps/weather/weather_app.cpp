
#include "weather_app.h"
#include "system/ui/screens.h"
#include "lvgl.h"
#include <time.h>
#include "weather_fetch.h"
#include "weather_icons.h"

#ifdef NATIVE_64BIT
    #include "utils/logging.h"
    #include "utils/millis.h"
#else
    #include <Arduino.h>
#endif

static weather_forecast_t weather_today;

/**
 * @brief LVGL timer callback to update time and date labels.
 * 
 * This function is executed by an LVGL timer to periodically refresh the
 * time and date displayed on the weather screen.
 * 
 * @param timer Pointer to the LVGL timer that triggered the callback.
 */
static void update_datetime_task(lv_timer_t * timer) {
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

/**
 * @brief LVGL timer callback to fetch and update weather data.
 * 
 * This function is executed by an LVGL timer to periodically fetch weather
 * data from the OpenWeatherMap API and update the UI.
 * 
 * @param timer Pointer to the LVGL timer that triggered the callback.
 */
static void update_weather_task(lv_timer_t * timer) {
    //if (wifictl_get_event(WIFICTL_CONNECT)) {
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

            //lv_label_set_text(objects.weather_label_city, weather_today.name);
            lv_label_set_text(objects.weather_label_current_temp, temp_buffer);
            lv_label_set_text(objects.weather_label_current_description, weather_today.description);
            lv_img_set_src(objects.weather_image_current_weather, resolve_owm_icon(weather_today.icon));
            lv_label_set_text(objects.weather_label_wind_speed, wind_buffer);
            // Angle is in 1/10 degree, OWM gives it in degrees
            lv_img_set_angle(objects.weather_image_wind_direction, (weather_today.wind_deg + 180) * 10);
        } else {
            log_e("Weather fetch failed.");
        }
    /*} else {
        log_w("Cannot fetch weather, no WiFi connection.");
    }*/
}

void setup_weather_app() {
    // Create a timer to update the time and date every 500ms (2 times per second)
    lv_timer_create(update_datetime_task, 500, NULL);
    // Create a timer to fetch weather data every 10 minutes
    lv_timer_t * weather_timer = lv_timer_create(update_weather_task, 10 * 60, NULL);
    lv_timer_ready(weather_timer); // Run it once immediately
}