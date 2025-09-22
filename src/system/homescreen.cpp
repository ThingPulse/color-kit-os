#include "homescreen.h"
#include "ui/screens.h"
#include "ui/actions.h"

#ifdef NATIVE_64BIT
    #include "utils/logging.h"
#else
    #include <Arduino.h>
#endif



void app_cb(lv_event_t *e) {
    log_i("touched");
}

typedef struct {
    lv_obj_t *container;
    lv_obj_t *button;
    lv_obj_t *label;
} app_button_widget_t;

void my_button_event_handler(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = (lv_obj_t*) lv_event_get_target(e);
    log_i("Clicked. Code: %d", code);
    if (code == LV_EVENT_CLICKED) {
        
        log_i("Button clicked!");
    }
}

int get_object_index(lv_obj_t *target) {
    lv_obj_t **object_array = (lv_obj_t **)&objects;
    int max_fields = sizeof(objects) / sizeof(lv_obj_t *);

    for (int i = 0; i < max_fields; i++) {
        if (object_array[i] == target) {
            return i;
        }
    }
    return -1; // Not found
}
void action_open_app(lv_event_t * e) {

}
void action_open_settings(lv_event_t * e) {
    lv_screen_load(objects.settings);
}

app_button_widget_t create_app_button_widget(lv_obj_t *parent) {

    app_button_widget_t widget;
    lv_obj_t *obj;
    {
        {
            // appButtonWidgetContainer
            lv_obj_t *obj = lv_obj_create(parent);
            widget.container = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 70, 70);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // appButton
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    widget.button = obj;
                    lv_obj_set_pos(obj, 10, 4);
                    lv_obj_set_size(obj, 50, 50);
                }
                {
                    // appLabel
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    widget.label = obj;
                    lv_obj_set_pos(obj, 20, 54);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "App");
                }
            }
        }
    }
    return widget;
}

void create_home_screen() {
    //create_screen_homescreen();
    lv_screen_load(objects.homescreen);

}