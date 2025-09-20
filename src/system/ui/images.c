#include "images.h"

#include <string.h>
const ext_img_desc_t images[4] = {
    { "logo", &img_logo },
    { "weatherApp", &img_weather_app },
    { "settings", &img_settings },
    { "weather", &img_weather },
};


const void *getLvglImageByName(const char *name) {
    for (size_t imageIndex = 0; imageIndex < sizeof(images) / sizeof(ext_img_desc_t); imageIndex++) {
        if (strcmp(images[imageIndex].name, name) == 0) {
            return images[imageIndex].img_dsc;
        }
    }
    return 0;
}

const lv_img_dsc_t img_undefined = {
    .header.w = 0,
    .header.h = 0,
    .header.cf = LV_COLOR_FORMAT_RAW_ALPHA,
    .data_size = 0,
    .data = NULL,
};