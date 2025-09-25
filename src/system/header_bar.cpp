#include "header_bar.h"
#include "ui/actions.h"
#include "ui/screens.h"
#include "hardware/timesync.h"
#include "utils/io.h"
#include "utils/logging.h"

bool timesync_update_cb_2(EventBits_t event, void *arg) {
    log_i("Update time in header");
    char buf[10];
    timesync_get_current_timestring(buf, sizeof(buf));
    lv_label_set_text(objects.title_bar_time, buf);
    //lv_label_set_text(objects.titleBarTime_1, buf);
    //lv_label_set_text(objects.titleBarTime_2, buf);
    //lv_label_set_text(objects.titleBarTime_3, buf);
    return true;
}

void action_register_header_bar(lv_event_t * e) {
    log_i("Registered header bar");
    timesync_register_cb(TIME_SYNC_UPDATE, timesync_update_cb_2, "header_bar");
}
