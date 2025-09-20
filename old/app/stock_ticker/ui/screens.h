#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>



namespace stock_ticker {
typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *setup;
    lv_obj_t *symbol_container_template;
    lv_obj_t *current_price_label_template;
    lv_obj_t *symbol_label_template;
    lv_obj_t *change_label_template;
    lv_obj_t *change_percentage_label_template;
    lv_obj_t *api_key_label;
    lv_obj_t *stock_ticker_api_field;
    lv_obj_t *stock_ticker_symbol_container;
    lv_obj_t *stock_ticker_symbol_field;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_SETUP = 2,
};

void create_screen_main(lv_obj_t *tile);
void tick_screen_main();

void create_screen_setup(lv_obj_t *tile);
void tick_screen_setup();

void create_screens();
void tick_screen(int screen_index);

}



#endif /*EEZ_LVGL_UI_SCREENS_H*/