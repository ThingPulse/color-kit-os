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

static void update_wifi_status_icon(void *data) {
    bool connected = (bool)data;
    if (connected) {
        lv_label_set_text(objects.title_bar_wifi, LV_SYMBOL_WIFI);
        lv_obj_set_style_text_color(objects.title_bar_wifi, lv_color_hex(0x00000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    } else {
        lv_label_set_text(objects.title_bar_wifi, LV_SYMBOL_WIFI);
        lv_obj_set_style_text_color(objects.title_bar_wifi, lv_color_hex(0x99999900), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

static bool header_bar_wifictl_event_cb( EventBits_t event, void *arg ) {   
    log_i("Wifi update event in header bar received"); 
    switch( event ) {
        case WIFICTL_CONNECT:       
            lv_async_call(update_wifi_status_icon, (void *)true);
            break;
        case WIFICTL_DISCONNECT:
        case WIFICTL_OFF: 
            lv_async_call(update_wifi_status_icon, (void *)false);
            break;
    }
    return( true );
}

void action_home_screen_load(lv_event_t * e) {
    //lv_obj_set_parent(objects.title_bar, objects.homescreen);
}

void action_attach_header_bar(lv_event_t * e) {
    /*lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *screen = (lv_obj_t*) lv_event_get_target(e);
    lv_obj_set_parent(objects.title_bar, screen);
    lv_obj_move_to_index(objects.title_bar, 0);*/
}

void init_header_bar() {
    lv_obj_set_parent(objects.title_bar, lv_layer_top());
    lv_obj_move_to_index(objects.title_bar, 0);
    header_bar_timer = lv_timer_create(header_bar_timer_cb, 500,  (void *)NULL);
    wifictl_register_cb( WIFICTL_OFF | WIFICTL_CONNECT | WIFICTL_DISCONNECT, header_bar_wifictl_event_cb, "header bar event callback" );
    wifictl_set_autoon(true);
}
