#include "settingswifiscreen.h"
#include "ui/screens.h"
#include "ui/actions.h"
#include "ui/styles.h"

#include "hardware/wifictl.h"

#ifdef NATIVE_64BIT
    #include "utils/logging.h"
#else
    #include <Arduino.h>
#endif
#include <cstring>

bool isCallbackRegistered = false;

static void free_user_data_on_delete(lv_event_t * e);
static char *dup_ssid(const char *s);


void add_wifi_entry(const char * ssid) {
    lv_obj_t *obj = lv_button_create(objects.ssid_button_matrix);
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, LV_PCT(100), 50);
    add_style_settings_button(obj);
    char *ssid_copy = dup_ssid(ssid);
    lv_obj_add_event_cb(obj, action_open_wifi_password_screen, LV_EVENT_CLICKED, (void *)ssid_copy);
    lv_obj_add_event_cb(obj, free_user_data_on_delete, LV_EVENT_DELETE, ssid_copy);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_settings_label(obj);
            lv_label_set_text(obj, ssid);
        }
    }
}

static void free_user_data_on_delete(lv_event_t * e) {
    if(lv_event_get_code(e) == LV_EVENT_DELETE) {
        char *p = (char *)lv_event_get_user_data(e);
        if(p) lv_free(p);
    }
}

static char *dup_ssid(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = (char *) lv_malloc(n);
    if(p) memcpy(p, s, n);
    return p;
}

bool wifi_setup_wifictl_event_cb( EventBits_t event, void *arg ) {
    switch( event ) {
        case    WIFICTL_ON:
                    //lv_switch_on( wifi_onoff, LV_ANIM_OFF );
                    break;
        case    WIFICTL_OFF:
                    //lv_switch_off( wifi_onoff, LV_ANIM_OFF );
                    //while ( lv_list_remove( wifiname_list, 0 ) );
                    break;
        case    WIFICTL_SCAN_DONE:
                    log_i("Scan is done");
                    lv_obj_clean(objects.ssid_button_matrix);
                    break;
        case    WIFICTL_SCAN_ENTRY:
                    //lv_obj_t * wifiname_list_btn = lv_list_add_btn( wifiname_list, wifictl_is_known( (const char*)arg )?&unlock_16px:&lock_16px , (const char*)arg );
                    //lv_obj_set_event_cb( wifiname_list_btn, wifi_settings_enter_pass_event_cb);
                    add_wifi_entry((const char*)arg);
                    log_i("Wifi entry received");
                    break;
    }
    return( true );
}

void action_settings_wifi_screen_loaded(lv_event_t * e) {
    lv_obj_clean(objects.ssid_button_matrix);
    add_wifi_entry("Scanning...");
    if (!isCallbackRegistered) {
        wifictl_register_cb( WIFICTL_ON | WIFICTL_OFF | WIFICTL_SCAN_DONE | WIFICTL_SCAN_ENTRY, wifi_setup_wifictl_event_cb, "wifi network scan" );
        isCallbackRegistered = true;
    }
    // Force network scan
    wifictl_scan_networks();

}

void action_open_wifi_password_screen(lv_event_t * e) {
    const char * ssid = (const char *) lv_event_get_user_data(e);

    log_i("Pressed button for SSID: %s", ssid);
    lv_textarea_set_text(objects.ssid_input, ssid);
    lv_screen_load_anim(objects.settings_wifi_password, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0, false);

}

void action_save_wifi_settings(lv_event_t * e) {
    wifictl_insert_network( lv_label_get_text( objects.ssid_input ), lv_textarea_get_text( objects.password_input ) );

    lv_screen_load_anim(objects.settings, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0, false);
    //lv_screen_load(objects.settings);
}

void action_show_wifi_settings_keyboard(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *target = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_clear_flag(objects.wifi_settings_keyboard, LV_OBJ_FLAG_HIDDEN);
    lv_keyboard_set_popovers(objects.wifi_settings_keyboard, true);
    lv_btnmatrix_set_btn_ctrl_all(objects.wifi_settings_keyboard, LV_BTNMATRIX_CTRL_NO_REPEAT);
    lv_keyboard_set_textarea(objects.wifi_settings_keyboard, target);
}
void action_keyboard_value_changed(lv_event_t * e) {
    lv_btnmatrix_set_btn_ctrl_all(objects.wifi_settings_keyboard, LV_BTNMATRIX_CTRL_NO_REPEAT);
}
void action_hide_wifi_settings_keyboard(lv_event_t * e) {
    lv_obj_add_flag(objects.wifi_settings_keyboard, LV_OBJ_FLAG_HIDDEN);
}

void action_return_wifi_settings(lv_event_t * e) {
    lv_screen_load_anim(objects.settings_wifi, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0, false);
}

