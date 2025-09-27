#include "weather_app.h"
#include "system/ui/screens.h"
#include "lvgl.h"
#include <time.h>

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

void setup_weather_app() {
    // Create a timer to update the time and date every 500ms (2 times per second)
    lv_timer_create(update_datetime_task, 500, NULL);
}