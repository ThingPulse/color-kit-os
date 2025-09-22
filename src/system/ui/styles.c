#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "ui.h"
#include "screens.h"

//
// Style: appButton
//

void init_style_app_button_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0xfffffdfd));
    lv_style_set_bg_grad_color(style, lv_color_hex(0xffb6b6b6));
    lv_style_set_bg_grad_dir(style, LV_GRAD_DIR_HOR);
    lv_style_set_bg_opa(style, 255);
    lv_style_set_radius(style, 10);
    lv_style_set_align(style, LV_ALIGN_TOP_MID);
};

lv_style_t *get_style_app_button_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_app_button_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_app_button(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_app_button_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_app_button(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_app_button_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: appLabel
//

void init_style_app_label_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_align(style, LV_ALIGN_BOTTOM_MID);
    lv_style_set_text_color(style, lv_color_hex(0xffffffff));
    lv_style_set_text_font(style, &lv_font_montserrat_12);
};

lv_style_t *get_style_app_label_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_app_label_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_app_label(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_app_label_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_app_label(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_app_label_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: settingsButton
//

void init_style_settings_button_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_radius(style, 0);
    lv_style_set_margin_top(style, 0);
    lv_style_set_margin_bottom(style, 0);
    lv_style_set_margin_left(style, 0);
    lv_style_set_margin_right(style, 0);
    lv_style_set_align(style, LV_ALIGN_TOP_LEFT);
    lv_style_set_border_color(style, lv_color_hex(0xff929292));
    lv_style_set_border_width(style, 1);
    lv_style_set_border_side(style, LV_BORDER_SIDE_BOTTOM);
    lv_style_set_bg_color(style, lv_color_hex(0xffe6e6e6));
    lv_style_set_text_color(style, lv_color_hex(0xff000000));
};

lv_style_t *get_style_settings_button_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_settings_button_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_settings_button(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_settings_button_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_settings_button(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_settings_button_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: settingsLabel
//

void init_style_settings_label_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_align(style, LV_ALIGN_LEFT_MID);
};

lv_style_t *get_style_settings_label_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_settings_label_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_settings_label(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_settings_label_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_settings_label(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_settings_label_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: formLabel
//

void add_style_form_label(lv_obj_t *obj) {
    (void)obj;
};

void remove_style_form_label(lv_obj_t *obj) {
    (void)obj;
};

//
// Style: textInput
//

void init_style_text_input_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0xffffffff));
    lv_style_set_border_color(style, lv_color_hex(0xffb5b5b5));
    lv_style_set_text_color(style, lv_color_hex(0xff000000));
};

lv_style_t *get_style_text_input_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_text_input_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_text_input_CURSOR_FOCUSED(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0xff000000));
    lv_style_set_bg_opa(style, 255);
};

lv_style_t *get_style_text_input_CURSOR_FOCUSED() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_text_input_CURSOR_FOCUSED(style);
    }
    return style;
};

void add_style_text_input(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_text_input_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_text_input_CURSOR_FOCUSED(), LV_PART_CURSOR | LV_STATE_FOCUSED);
};

void remove_style_text_input(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_text_input_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_text_input_CURSOR_FOCUSED(), LV_PART_CURSOR | LV_STATE_FOCUSED);
};

//
// Style: keyboards
//

void init_style_keyboards_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_align(style, LV_ALIGN_DEFAULT);
};

lv_style_t *get_style_keyboards_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_keyboards_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_keyboards_ITEMS_PRESSED(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0xffffffff));
    lv_style_set_text_color(style, lv_color_hex(0xff000000));
};

lv_style_t *get_style_keyboards_ITEMS_PRESSED() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_keyboards_ITEMS_PRESSED(style);
    }
    return style;
};

void add_style_keyboards(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_keyboards_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_keyboards_ITEMS_PRESSED(), LV_PART_ITEMS | LV_STATE_PRESSED);
};

void remove_style_keyboards(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_keyboards_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_keyboards_ITEMS_PRESSED(), LV_PART_ITEMS | LV_STATE_PRESSED);
};

//
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_app_button,
        add_style_app_label,
        add_style_settings_button,
        add_style_settings_label,
        add_style_form_label,
        add_style_text_input,
        add_style_keyboards,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_app_button,
        remove_style_app_label,
        remove_style_settings_button,
        remove_style_settings_label,
        remove_style_form_label,
        remove_style_text_input,
        remove_style_keyboards,
    };
    remove_style_funcs[styleIndex](obj);
}

