#include "settingsscreen.h"
#include "ui/screens.h"
#include "ui/actions.h"

#ifdef NATIVE_64BIT
    #include "utils/logging.h"
#else
    #include <Arduino.h>
#endif

void action_open_home_screen(lv_event_t * e) {
    lv_screen_load(objects.homescreen);
    
}

void action_open_wifi_settings(lv_event_t * e) {
    lv_screen_load_anim(objects.settings_wifi, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0, false);
    lv_obj_set_parent(objects.title_bar, objects.settings_wifi);
}

void action_settings_screen_load(lv_event_t * e) {
    lv_obj_set_parent(objects.title_bar, objects.settings);
    lv_obj_set_pos(objects.title_bar, 0, 0);
}