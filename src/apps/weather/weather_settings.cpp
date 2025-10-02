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
#include "i18n/weather_i18n.h"

#ifdef NATIVE_64BIT
    #include "utils/logging.h"
    #include "utils/millis.h"
#else
    #include <Arduino.h>
#endif


void action_get_gps_coordinates(lv_event_t * e) {

}