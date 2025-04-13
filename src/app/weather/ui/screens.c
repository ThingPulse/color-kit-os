#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

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
    {
        lv_obj_t *parent_obj = obj;
        {
            // time_container
            lv_obj_t *obj = lv_cont_create(parent_obj, NULL);
            objects.time_container = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 320, 122);
            lv_obj_set_style_local_pad_left(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_pad_top(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_pad_right(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_pad_bottom(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_border_width(obj, 0, LV_CONT_PART_MAIN, LV_STATE_DEFAULT);
            lv_obj_set_style_local_radius(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_bg_opa(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_bg_color(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xffffffff));
            lv_obj_set_style_local_text_color(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xffffffff & 0xFFFFFF));
            {
                lv_obj_t *parent_obj = obj;
                {
                    // labelTime
                    lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
                    objects.label_time = obj;
                    lv_obj_set_pos(obj, 15, 34);
                    lv_obj_set_size(obj, 290, 48);
                    lv_label_set_static_text(obj, "08:23");
                    lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans48);
                    lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
                    lv_obj_set_style_local_bg_color(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xffffffff));
                }
                {
                    // labelDate
                    lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
                    objects.label_date = obj;
                    lv_obj_set_pos(obj, 69, 14);
                    lv_obj_set_size(obj, 182, 17);
                    lv_label_set_static_text(obj, "Friday, 11.10.2023");
                    lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans16);
                    lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
                    lv_obj_set_style_local_bg_color(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xffffffff));
                }
            }
        }
        {
            // today_container
            lv_obj_t *obj = lv_cont_create(parent_obj, NULL);
            objects.today_container = obj;
            lv_obj_set_pos(obj, 0, 90);
            lv_obj_set_size(obj, 320, 134);
            lv_obj_set_style_local_pad_left(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_pad_top(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_pad_right(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_pad_bottom(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_border_width(obj, 0, LV_CONT_PART_MAIN, LV_STATE_DEFAULT);
            lv_obj_set_style_local_radius(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_bg_opa(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_bg_color(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xffffffff));
            lv_obj_set_style_local_text_color(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xffffffff & 0xFFFFFF));
            {
                lv_obj_t *parent_obj = obj;
                {
                    // labelWindSpeed
                    lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
                    objects.label_wind_speed = obj;
                    lv_obj_set_pos(obj, 227, 106);
                    lv_obj_set_size(obj, 90, 17);
                    lv_label_set_static_text(obj, "1 m/s");
                    lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans16);
                    lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
                }
                {
                    // labelPressure
                    lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
                    objects.label_pressure = obj;
                    lv_obj_set_pos(obj, 70, 106);
                    lv_obj_set_size(obj, 182, 17);
                    lv_label_set_static_text(obj, "955hPa");
                    lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans16);
                    lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
                }
                {
                    // labelHumidity
                    lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
                    objects.label_humidity = obj;
                    lv_obj_set_pos(obj, 69, 86);
                    lv_obj_set_size(obj, 182, 17);
                    lv_label_set_static_text(obj, "76%");
                    lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans16);
                    lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
                }
                {
                    // labelCurrentDescription
                    lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
                    objects.label_current_description = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 320, 32);
                    lv_label_set_static_text(obj, "clear sky");
                    lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
                    lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans24);
                }
                {
                    // imageWind
                    lv_obj_t *obj = lv_img_create(parent_obj, NULL);
                    objects.image_wind = obj;
                    lv_obj_set_pos(obj, 234, 25);
                    lv_obj_set_size(obj, 75, 75);
                    lv_img_set_src(obj, &img_wind_n);
                }
                {
                    // imageCurrentWeather
                    lv_obj_t *obj = lv_img_create(parent_obj, NULL);
                    objects.image_current_weather = obj;
                    lv_obj_set_pos(obj, -4, 21);
                    lv_obj_set_size(obj, 100, 100);
                    lv_img_set_src(obj, &img_weather_clear_day);
                }
                {
                    // labelCurrentTemperature
                    lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
                    objects.label_current_temperature = obj;
                    lv_obj_set_pos(obj, 0, 36);
                    lv_obj_set_size(obj, 320, 48);
                    lv_label_set_static_text(obj, "14.3°");
                    lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans48);
                    lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
                }
            }
        }
        {
            // forecast_container
            lv_obj_t *obj = lv_cont_create(parent_obj, NULL);
            objects.forecast_container = obj;
            lv_obj_set_pos(obj, 0, 240);
            lv_obj_set_size(obj, 314, 116);
            lv_obj_set_style_local_pad_left(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_pad_top(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_pad_right(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_pad_bottom(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_border_width(obj, 0, LV_CONT_PART_MAIN, LV_STATE_DEFAULT);
            lv_obj_set_style_local_radius(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_bg_opa(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_bg_color(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xffffffff));
            lv_obj_set_style_local_text_color(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xffffffff & 0xFFFFFF));
            {
                lv_obj_t *parent_obj = obj;
                {
                    // labelTempDay0
                    lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
                    objects.label_temp_day0 = obj;
                    lv_obj_set_pos(obj, 5, 24);
                    lv_obj_set_size(obj, 69, 18);
                    lv_label_set_static_text(obj, "5-8°");
                    lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans18);
                    lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
                }
                {
                    // labelDay1
                    lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
                    objects.label_day1 = obj;
                    lv_obj_set_pos(obj, 85, 0);
                    lv_obj_set_size(obj, 69, 24);
                    lv_label_set_static_text(obj, "FRI");
                    lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
                    lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans24);
                }
                {
                    // labelDay2
                    lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
                    objects.label_day2 = obj;
                    lv_obj_set_pos(obj, 165, 0);
                    lv_obj_set_size(obj, 69, 24);
                    lv_label_set_static_text(obj, "SAT");
                    lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
                    lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans24);
                }
                {
                    // labelDay3
                    lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
                    objects.label_day3 = obj;
                    lv_obj_set_pos(obj, 245, 0);
                    lv_obj_set_size(obj, 69, 24);
                    lv_label_set_static_text(obj, "SUN");
                    lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
                    lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans24);
                }
                {
                    // labelDay0
                    lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
                    objects.label_day0 = obj;
                    lv_obj_set_pos(obj, 5, 0);
                    lv_obj_set_size(obj, 69, 24);
                    lv_label_set_static_text(obj, "THU");
                    lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
                    lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans24);
                }
                {
                    // labelTempDay1
                    lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
                    objects.label_temp_day1 = obj;
                    lv_obj_set_pos(obj, 85, 24);
                    lv_obj_set_size(obj, 69, 18);
                    lv_label_set_static_text(obj, "6-9°");
                    lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans18);
                    lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
                }
                {
                    // labelTempDay2
                    lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
                    objects.label_temp_day2 = obj;
                    lv_obj_set_pos(obj, 165, 24);
                    lv_obj_set_size(obj, 69, 18);
                    lv_label_set_static_text(obj, "13-20°");
                    lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans18);
                    lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
                }
                {
                    // labelTempDay3
                    lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
                    objects.label_temp_day3 = obj;
                    lv_obj_set_pos(obj, 245, 24);
                    lv_obj_set_size(obj, 69, 18);
                    lv_label_set_static_text(obj, "15-22°");
                    lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans18);
                    lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
                }
                {
                    // imageDay0
                    lv_obj_t *obj = lv_img_create(parent_obj, NULL);
                    objects.image_day0 = obj;
                    lv_obj_set_pos(obj, 5, 39);
                    lv_obj_set_size(obj, 64, 64);
                    lv_img_set_src(obj, &img_weather_fog_64px);
                }
                {
                    // imageDay1
                    lv_obj_t *obj = lv_img_create(parent_obj, NULL);
                    objects.image_day1 = obj;
                    lv_obj_set_pos(obj, 87, 39);
                    lv_obj_set_size(obj, 64, 64);
                    lv_img_set_src(obj, &img_weather_fog_64px);
                }
                {
                    // imageDay2
                    lv_obj_t *obj = lv_img_create(parent_obj, NULL);
                    objects.image_day2 = obj;
                    lv_obj_set_pos(obj, 169, 39);
                    lv_obj_set_size(obj, 64, 64);
                    lv_img_set_src(obj, &img_weather_clear_day_64px);
                }
                {
                    // imageDay3
                    lv_obj_t *obj = lv_img_create(parent_obj, NULL);
                    objects.image_day3 = obj;
                    lv_obj_set_pos(obj, 246, 39);
                    lv_obj_set_size(obj, 64, 64);
                    lv_img_set_src(obj, &img_weather_sleet_64px);
                }
            }
        }
        {
            // message_container
            lv_obj_t *obj = lv_cont_create(parent_obj, NULL);
            objects.message_container = obj;
            lv_obj_set_pos(obj, 0, 90);
            lv_obj_set_size(obj, 320, 200);
            lv_obj_set_style_local_pad_left(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_pad_top(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_pad_right(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_pad_bottom(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_bg_opa(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_border_width(obj, 0, LV_CONT_PART_MAIN, LV_STATE_DEFAULT);
            lv_obj_set_style_local_radius(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_set_style_local_text_color(obj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xffffffff & 0xFFFFFF));
            {
                lv_obj_t *parent_obj = obj;
                {
                    // labelErrorMessage
                    lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
                    objects.label_error_message = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 320, 100);
                    lv_label_set_static_text(obj, "Error loading weather data.\n Please check coordinates\nand API key.");
                    lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans24);
                    lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
                }
            }
        }
        {
            // imageMoon
            lv_obj_t *obj = lv_img_create(parent_obj, NULL);
            objects.image_moon = obj;
            lv_obj_set_pos(obj, 115, 369);
            lv_obj_set_size(obj, 75, 75);
            lv_img_set_src(obj, &img_moon_phase_15);
        }
        {
            // labelSun
            lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
            objects.label_sun = obj;
            lv_obj_set_pos(obj, 18, 373);
            lv_obj_set_size(obj, 69, 24);
            lv_label_set_static_text(obj, "Sun");
            lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
            lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans24);
        }
        {
            // labelMoon
            lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
            objects.label_moon = obj;
            lv_obj_set_pos(obj, 228, 375);
            lv_obj_set_size(obj, 69, 24);
            lv_label_set_static_text(obj, "Moon");
            lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
            lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans24);
        }
        {
            // labelSunrise
            lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
            objects.label_sunrise = obj;
            lv_obj_set_pos(obj, 18, 398);
            lv_obj_set_size(obj, 69, 18);
            lv_label_set_static_text(obj, "06:44");
            lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans18);
            lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
        }
        {
            // labelSunset
            lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
            objects.label_sunset = obj;
            lv_obj_set_pos(obj, 18, 417);
            lv_obj_set_size(obj, 69, 18);
            lv_label_set_static_text(obj, "19:05");
            lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans18);
            lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
        }
        {
            // labelMoonrise
            lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
            objects.label_moonrise = obj;
            lv_obj_set_pos(obj, 228, 400);
            lv_obj_set_size(obj, 69, 18);
            lv_label_set_static_text(obj, "00:21");
            lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans18);
            lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
        }
        {
            // labelMoonset
            lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
            objects.label_moonset = obj;
            lv_obj_set_pos(obj, 228, 418);
            lv_obj_set_size(obj, 69, 18);
            lv_label_set_static_text(obj, "07:05");
            lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans18);
            lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
        }
        {
            // labelMoonPhase
            lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
            objects.label_moon_phase = obj;
            lv_obj_set_pos(obj, 15, 454);
            lv_obj_set_size(obj, 282, 17);
            lv_label_set_static_text(obj, "Waning Gibbous");
            lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans16);
            lv_label_set_align(obj, LV_LABEL_ALIGN_CENTER);
        }
        {
            // lastUpdate
            lv_obj_t *obj = lv_label_create(parent_obj, NULL);
			lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
            objects.last_update = obj;
            lv_obj_set_pos(obj, 5, 3);
            lv_obj_set_size(obj, 80, 20);
            lv_label_set_static_text(obj, "Text");
            lv_obj_set_style_local_text_font(obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &ui_font_open_sans16);
        }
    }
}

void tick_screen_main() {
}


void create_screens() {
// 
//     lv_disp_t *dispp = lv_disp_get_default();
//     lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
//     lv_disp_set_theme(dispp, theme);
//     
//     create_screen_main(lv_obj_t *tile);
}

typedef void (*tick_screen_func_t)();

tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
};

void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
