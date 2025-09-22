#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: appButton
lv_style_t *get_style_app_button_MAIN_DEFAULT();
void add_style_app_button(lv_obj_t *obj);
void remove_style_app_button(lv_obj_t *obj);

// Style: appLabel
lv_style_t *get_style_app_label_MAIN_DEFAULT();
void add_style_app_label(lv_obj_t *obj);
void remove_style_app_label(lv_obj_t *obj);

// Style: settingsButton
lv_style_t *get_style_settings_button_MAIN_DEFAULT();
void add_style_settings_button(lv_obj_t *obj);
void remove_style_settings_button(lv_obj_t *obj);

// Style: settingsLabel
lv_style_t *get_style_settings_label_MAIN_DEFAULT();
void add_style_settings_label(lv_obj_t *obj);
void remove_style_settings_label(lv_obj_t *obj);

// Style: formLabel
void add_style_form_label(lv_obj_t *obj);
void remove_style_form_label(lv_obj_t *obj);

// Style: textInput
lv_style_t *get_style_text_input_MAIN_DEFAULT();
lv_style_t *get_style_text_input_CURSOR_FOCUSED();
void add_style_text_input(lv_obj_t *obj);
void remove_style_text_input(lv_obj_t *obj);

// Style: keyboards
lv_style_t *get_style_keyboards_MAIN_DEFAULT();
lv_style_t *get_style_keyboards_ITEMS_PRESSED();
void add_style_keyboards(lv_obj_t *obj);
void remove_style_keyboards(lv_obj_t *obj);



#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/