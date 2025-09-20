

#include "config.h"

#include "plainspotter.h"
#include "plainspotter_app_main.h"

#include "gui/mainbar/app_tile/app_tile.h"
#include "gui/mainbar/main_tile/main_tile.h"
#include "gui/mainbar/mainbar.h"
#include "gui/statusbar.h"
#include "gui/widget_factory.h"
#include "gui/widget_styles.h"
#include "utils/osm_map/osm_map.h"

#ifdef NATIVE_64BIT
    #include <time.h>
    #include "utils/logging.h"
    #include "utils/millis.h"
#else
    #include <time.h>
    #include <Arduino.h>
#endif
#include <math.h>

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 320



// A global buffer to hold the canvas pixels.
// It must have SCREEN_WIDTH * SCREEN_HEIGHT elements.
lv_color_t *map_canvas_buf;
lv_color_t *plane_canvas_buf;


lv_obj_t *map_canvas = NULL;
lv_obj_t *plane_canvas = NULL;

bool is_buffer_allocated = false;

static void planespotter_update_task( lv_task_t * task );
lv_task_t * planespotter_tile_task;

// ----------------------------------------------------------------------
// Page Setup
// ----------------------------------------------------------------------
/**
 * @brief Creates a canvas and readies it for drawing. Call this once.
 */

// Convert from latitude/longitude to the OSM x/y tile number at a given zoom level.
void lat_lon_to_tile(double lat, double lon, uint8_t zoom, double *x_tile, double *y_tile)
{
    // Ensure lat/lon are in valid ranges
    if (lat > 85.05112878)  lat = 85.05112878;
    if (lat < -85.05112878) lat = -85.05112878;
    if (lon > 180)          lon = 180;
    if (lon < -180)         lon = -180;
    
    // Convert to radians
    double lat_rad = lat * M_PI / 180.0;
    
    // Number of tiles at this zoom level
    double n = (double)(1 << zoom);

    // Calculate tile numbers
    *x_tile = (lon + 180.0) / 360.0 * n;
    *y_tile = (1.0 - log(tan(lat_rad) + 1.0 / cos(lat_rad)) / M_PI) / 2.0 * n;
}

// Example function to download tile into a buffer.
// You must implement the HTTP portion or file read portion yourself.
bool download_tile_image(int x, int y, int zoom, uint8_t **data, size_t *data_size)
{
    // For demonstration, build an OSM tile URL: e.g. "https://tile.openstreetmap.org/zoom/x/y.png"
    // Make sure to respect OSM tile usage policy!

    char url[128];
    snprintf(url, sizeof(url), "http://tile.openstreetmap.org/%d/%d/%d.png", zoom, x, y);
    log_i("Downloading map: %s", url);
    uri_load_dsc_t *uri_load_dsc = uri_load_to_ram( url );

    *data = uri_load_dsc->data;
    *data_size = uri_load_dsc->size;

    log_i("Tile Size: %d", uri_load_dsc->size);
    if (uri_load_dsc->size == 0) {
        return false;
    }

    // -> Use your HTTP library to download the image into memory:
    // *data = pointer_to_downloaded_bytes;
    // *data_size = number_of_downloaded_bytes;
    //
    // Return true if successful, false otherwise.
    return true; // replace with real implementation
}

void draw_map_tiles(double centerLat, double centerLon, uint8_t zoom)
{
    if (!is_buffer_allocated) {
        #ifndef NATIVE_64BIT
            log_i("PSARM: %d", ESP.getFreePsram());
        #endif
        size_t buf_size_in_pixels = LV_CANVAS_BUF_SIZE_TRUE_COLOR(SCREEN_WIDTH, SCREEN_HEIGHT);

        map_canvas_buf = (lv_color_t*) MALLOC(sizeof(lv_color_t) * buf_size_in_pixels);
        plane_canvas_buf = (lv_color_t*) MALLOC(sizeof(lv_color_t) * buf_size_in_pixels);

        // Assign the local buffer to the canvas
        lv_canvas_set_buffer(
            map_canvas,
            map_canvas_buf,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            LV_IMG_CF_TRUE_COLOR
        );

        lv_canvas_set_buffer(
            plane_canvas,
            plane_canvas_buf,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED
        );

        lv_canvas_fill_bg(plane_canvas, LV_COLOR_TRANSP, 128);


        is_buffer_allocated = true;
    }
    double xTileD, yTileD;
    lat_lon_to_tile(centerLat, centerLon, zoom, &xTileD, &yTileD);

    // Fractional tile position
    double fractionalX = xTileD - floor(xTileD);
    double fractionalY = yTileD - floor(yTileD);

    // The integer tile index for the center tile
    int centerTileX = (int)floor(xTileD);
    int centerTileY = (int)floor(yTileD);

    // On-screen offset (in pixels) that the center tile will have due to fractional part
    // Each tile is 256 px wide.
    int centerOffsetX = (int)(-fractionalX * 256.0 + 0.5);
    int centerOffsetY = (int)(-fractionalY * 256.0 + 0.5);

    // We want to fill the 320×480 area around that center tile.
    // Let's define how many tiles horizontally and vertically we need:
    int tilesH = 3; // covers 320 px horizontally
    int tilesV = 4; // covers 480 px vertically (but you might use 3 to be safe)
    int number_of_tiles = tilesH * tilesV;

    
    
    
    uint8_t tile_index = 0;

    
    uint8_t *tile_data = NULL;
    size_t tile_data_size = 0;
    lv_draw_img_dsc_t draw_dsc[9];
    for (int i = 0; i < 9; i++) {
        lv_draw_img_dsc_init(&draw_dsc[i]);
    }

    lv_img_dsc_t tile_img[9];
    
    //lv_img_cache_set_size( 1 );
    //lv_img_cache_set_size( 1 );
    //lv_img_cache_invalidate_src(NULL);

    for(int ty = 0; ty < tilesV; ty++)
    {
        for(int tx = 0; tx < tilesH; tx++)
        {
            int tileX = centerTileX + (tx - tilesH/2);
            int tileY = centerTileY + (ty - tilesV/2);

            // Where on the canvas do we draw this tile?
            int drawX = (tx - tilesH/2)*256 + centerOffsetX + 160; // +160 to center it horizontally
            int drawY = (ty - tilesV/2)*256 + centerOffsetY + 240; // +240 to center it vertically
            int tile_right  = drawX + 256;
            int tile_bottom = drawY + 256;

            if(tile_right < 0 || tile_bottom < 0 || drawX >= SCREEN_WIDTH || drawY >= SCREEN_HEIGHT) {
                log_i("Skipping invisible tile");
                continue;
            }
            log_i("Processing tile %d", tile_index);
            #ifndef NATIVE_64BIT
                log_i("PSARM: %d", ESP.getFreePsram());
            
            #endif
            tile_data= NULL;
            tile_data_size = 0;

            if(!download_tile_image(tileX, tileY, zoom, &tile_data, &tile_data_size)) {
                log_e("Download failed");
                // If download fails, skip or show a placeholder
                continue;
            }


            memset(&tile_img[tile_index], 0, sizeof(tile_img[tile_index]));
            tile_img[tile_index].data = tile_data;
            tile_img[tile_index].data_size = tile_data_size;
            tile_img[tile_index].header.always_zero = 0;
            tile_img[tile_index].header.cf = LV_IMG_CF_RAW_ALPHA; // or something appropriate
            tile_img[tile_index].header.w = 256;
            tile_img[tile_index].header.h = 256;

            lv_draw_label_dsc_t label;
            lv_draw_label_dsc_init(&label);
            label.color = LV_COLOR_RED;
            
            char tile_text[128];
            snprintf(tile_text, sizeof(tile_text), "%d", tile_index);
            log_i("Tile text: %s", tile_text);
            // Now draw it on the canvas (in LVGL v7).
            // (x, y) are the coords on the canvas where the top-left of the tile is drawn.

            lv_canvas_draw_img(map_canvas, drawX, drawY, &tile_img[tile_index], &draw_dsc[tile_index]);
            lv_img_cache_invalidate_src(&tile_img[tile_index]);

            //lv_canvas_draw_text(plane_canvas, drawX + 10, drawY + 30, 100, &label, tile_text, LV_LABEL_ALIGN_LEFT);
            
            log_i("Draw map to %d, %d", drawX, drawY);

            // Freed after drawing if the decoder doesn’t keep a reference.
            //free(tile_data);
            //tile_img[tile_index].data = NULL;

            //free(tile_data);
            tile_index++;

            
        }
    }

    // Finally refresh the canvas object
    lv_obj_invalidate(map_canvas);


}

void refresh_map() {
    draw_map_tiles(47.4, 8.6, 13 );
}

static void planespotter_update_task( lv_task_t * task ) {
    lv_canvas_fill_bg(plane_canvas, LV_COLOR_TRANSP, 128);
    lv_draw_label_dsc_t label;
    lv_draw_label_dsc_init(&label);
    label.color = LV_COLOR_RED;
    char tile_text[128];
    snprintf(tile_text, sizeof(tile_text), "%d", millis());

    lv_canvas_draw_text(plane_canvas,160, 240, 100, &label, tile_text, LV_LABEL_ALIGN_LEFT);
}


void plainspotter_app_main_setup( AppPage* screen) {

    // Create a canvas on the active screen
    map_canvas = lv_canvas_create(screen->handle(), NULL);
    if(!map_canvas) {
        LV_LOG_WARN("Failed to create canvas object");
        return;
    }

    plane_canvas = lv_canvas_create(screen->handle(), NULL);
    if(!plane_canvas) {
        LV_LOG_WARN("Failed to create canvas object");
        return;
    }


    // Position the canvas at top-left of the screen (0,0)
    lv_obj_set_pos(map_canvas, 0, 0);
    lv_obj_set_pos(plane_canvas, 0, 0);

    planespotter_tile_task = lv_task_create( planespotter_update_task, 5000, LV_TASK_PRIO_MID, NULL );

}


