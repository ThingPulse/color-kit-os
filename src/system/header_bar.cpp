#include "header_bar.h"
#include "ui/actions.h"
#include "ui/screens.h"
#include "hardware/timesync.h"
#include "utils/io.h"
#include "utils/logging.h"
#include "hardware/wifictl.h"

lv_timer_t * header_bar_timer;

void header_bar_timer_cb(lv_timer_t * timer) {
    char buf[10];
    timesync_get_current_timestring(buf, sizeof(buf));
    lv_label_set_text(objects.title_bar_time, buf);
}

void action_register_header_bar(lv_event_t * e) {

}

static bool header_bar_wifictl_event_cb( EventBits_t event, void *arg ) {   
    log_i("Wifi update event in header bar received"); 
    switch( event ) {
        case WIFICTL_CONNECT:       
            lv_label_set_text(objects.title_bar_wifi, "WiFi: on");
            break;
        case WIFICTL_DISCONNECT:
            lv_label_set_text(objects.title_bar_wifi, "WiFi: off");
            break;
        case WIFICTL_OFF: 
            lv_label_set_text(objects.title_bar_wifi, "WiFi: off");
            break;
    }
    return( true );
}

void action_home_screen_load(lv_event_t * e) {
    lv_obj_set_parent(objects.title_bar, objects.homescreen);
}

void action_attach_header_bar(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *screen = (lv_obj_t*) lv_event_get_target(e);
    lv_obj_set_parent(objects.title_bar, screen);
    lv_obj_move_to_index(objects.title_bar, 0);
}

void init_header_bar() {
    header_bar_timer = lv_timer_create(header_bar_timer_cb, 500,  (void *)NULL);
    wifictl_register_cb( WIFICTL_OFF | WIFICTL_CONNECT | WIFICTL_DISCONNECT, header_bar_wifictl_event_cb, "header bar event callback" );
    wifictl_set_autoon(true);
}
