#include "header_bar.h"
#include "ui/actions.h"
#include "ui/screens.h"
#include "hardware/timesync.h"
#include "utils/io.h"

bool timesync_update_cb(EventBits_t event, void *arg) {
    char buf[10];
    timesync_get_current_timestring(buf, sizeof(buf));
    lv_label_set_text(objects.title_bar_time, buf);
    //lv_label_set_text(objects.titleBarTime_1, buf);
    //lv_label_set_text(objects.titleBarTime_2, buf);
    //lv_label_set_text(objects.titleBarTime_3, buf);
    return true;
}

void action_register_header_bar(lv_event_t * e) {
    timesync_register_cb(TIME_SYNC_UPDATE, timesync_update_cb, "header_bar");
}
