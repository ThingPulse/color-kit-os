#include <string.h>

#include "screens.h"


namespace stock_ticker {
    #include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;

void create_screen_main(lv_obj_t *tile) {
    lv_obj_t *obj = lv_cont_create(tile, NULL);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 480);
    lv_obj_set_click(obj, false);
    //lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_local_bg_color(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xff000000));
    lv_obj_set_style_local_text_color(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xffffffff & 0xFFFFFF));
    lv_obj_set_style_local_border_width(obj, 0, LV_CONT_PART_MAIN, LV_STATE_DEFAULT);
    lv_obj_set_style_local_bg_opa(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 180);
    lv_obj_set_style_local_radius(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
            lv_obj_set_pos(obj, 114, 9);
            lv_obj_set_size(obj, 92, 16);
            lv_label_set_static_text(obj, "Stock Ticker!");
        }
        {
            // symbolContainerTemplate
            lv_obj_t *obj = lv_cont_create(parent_obj, NULL);
            objects.symbol_container_template = obj;
            lv_obj_set_pos(obj, 0, 40);
            lv_obj_set_size(obj, 320, 30);
            lv_obj_set_style_local_pad_left(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_pad_top(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_pad_right(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_pad_bottom(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_bg_opa(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_border_width(obj, 0, LV_CONT_PART_MAIN, LV_STATE_DEFAULT);
            lv_obj_set_style_local_radius(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // currentPriceLabelTemplate
                    lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
                    objects.current_price_label_template = obj;
                    lv_obj_set_pos(obj, 50, 5);
                    lv_obj_set_size(obj, 60, 16);
                    lv_label_set_static_text(obj, "932.1");
                    lv_label_set_align(obj, LV_LABEL_ALIGN_RIGHT);
                    lv_obj_set_style_local_text_color(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xffffffff & 0xFFFFFF));
                }
                {
                    // symbolLabelTemplate
                    lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
                    objects.symbol_label_template = obj;
                    lv_obj_set_pos(obj, 5, 5);
                    lv_obj_set_size(obj, 45, 16);
                    lv_label_set_static_text(obj, "BTC");
                    lv_obj_set_style_local_text_color(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xffffffff & 0xFFFFFF));
                }
                {
                    // changeLabelTemplate
                    lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
                    objects.change_label_template = obj;
                    lv_obj_set_pos(obj, 114, 5);
                    lv_obj_set_size(obj, 50, 16);
                    lv_label_set_static_text(obj, "+9.3");
                    lv_label_set_align(obj, LV_LABEL_ALIGN_RIGHT);
                    lv_obj_set_style_local_text_color(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xff00ff1b & 0xFFFFFF));
                }
                {
                    // changePercentageLabelTemplate
                    lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
                    objects.change_percentage_label_template = obj;
                    lv_obj_set_pos(obj, 181, 5);
                    lv_obj_set_size(obj, 50, 16);
                    lv_label_set_static_text(obj, "+2.3%");
                    lv_label_set_align(obj, LV_LABEL_ALIGN_RIGHT);
                    lv_obj_set_style_local_text_color(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xff00ff1b & 0xFFFFFF));
                }
            }
        }
    }
}

void tick_screen_main() {
}

void create_screen_setup(lv_obj_t *tile) {
    lv_obj_t *obj = lv_cont_create(tile, NULL);
    objects.setup = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 480);
    lv_obj_set_click(obj, false);
    //lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_local_bg_color(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xff000000));
    lv_obj_set_style_local_text_color(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xffffffff & 0xFFFFFF));
    lv_obj_set_style_local_border_width(obj, 0, LV_CONT_PART_MAIN, LV_STATE_DEFAULT);
    lv_obj_set_style_local_bg_opa(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 180);
    lv_obj_set_style_local_radius(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    {
        lv_obj_t *parent_obj = obj;
        {
            // apiKeyLabel
            lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
            objects.api_key_label = obj;
            lv_obj_set_pos(obj, 13, 73);
            lv_obj_set_size(obj, 57, 16);
            lv_label_set_static_text(obj, "Api Key:");
        }
        {
            // stockTickerApiField
            lv_obj_t *obj = lv_textarea_create(parent_obj, NULL);
            objects.stock_ticker_api_field = obj;
            lv_obj_set_pos(obj, 83, 63);
            lv_obj_set_size(obj, 223, 36);
            lv_textarea_set_max_length(obj, 128);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_pwd_mode(obj, false);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
            lv_obj_set_pos(obj, 95, 110);
            lv_obj_set_size(obj, 131, 16);
            lv_label_set_static_text(obj, "Stock Symbols");
            lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
        }
        {
            // stockTickerSymbolContainer
            lv_obj_t *obj = lv_cont_create(parent_obj, NULL);
            objects.stock_ticker_symbol_container = obj;
            lv_obj_set_pos(obj, 0, 137);
            lv_obj_set_size(obj, 320, 45);
            lv_obj_set_style_local_pad_left(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_pad_top(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_pad_right(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_pad_bottom(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_bg_opa(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_border_width(obj, 0, LV_CONT_PART_MAIN, LV_STATE_DEFAULT);
            lv_obj_set_style_local_radius(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // stockTickerSymbolField
                    lv_obj_t *obj = lv_textarea_create(parent_obj, NULL);
                    objects.stock_ticker_symbol_field = obj;
                    lv_obj_set_pos(obj, 14, 0);
                    lv_obj_set_size(obj, 293, 36);
                    lv_textarea_set_max_length(obj, 128);
                    lv_textarea_set_one_line(obj, true);
                    lv_textarea_set_pwd_mode(obj, false);
                }
            }
        }
    }
}

void tick_screen_setup() {
}


    void create_screens() {
// 
//     lv_disp_t *dispp = lv_disp_get_default();
//     lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
//     lv_disp_set_theme(dispp, theme);
//     
//     create_screen_main(lv_obj_t *tile);
//     create_screen_setup(lv_obj_t *tile);
}

typedef void (*tick_screen_func_t)();

tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
    tick_screen_setup,
};

void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}

}