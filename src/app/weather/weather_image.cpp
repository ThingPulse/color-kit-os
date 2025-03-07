#include "config.h"
#include "weather.h"
#include "weather_fetch.h"
#include "images/resolve_owm_icon.h"
#include "gui/gui.h"
#include "gui/mainbar/mainbar.h"
#include "gui/mainbar/main_tile/main_tile.h"
#include "gui/statusbar.h"
#include "gui/keyboard.h"
#include "gui/widget_factory.h"
#include "gui/widget_styles.h"
#include "gui/widget.h"
#include "hardware/powermgm.h"
#include "hardware/wifictl.h"
#include "utils/alloc.h"
#include "ui/screens.h"
#include "ui/images.h"
#include "weather_image.h"

lv_obj_t *weather_image_tile = NULL;
uint32_t weather_image_tile_num;

void weather_image_tile_setup( uint32_t tile_num ) {
    weather_image_tile_num = tile_num;
    weather_image_tile = mainbar_get_tile_obj( weather_image_tile_num );
}



