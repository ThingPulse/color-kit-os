

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

osm_location_t *map_location = NULL;             /** @brief osm location obj */

// A global buffer to hold the canvas pixels.
// It must have SCREEN_WIDTH * SCREEN_HEIGHT elements.
lv_color_t g_canvas_buf[SCREEN_WIDTH * SCREEN_HEIGHT];

void canvas_event_cb(lv_obj_t * canvas, lv_event_t event);

// A pointer to our canvas object so we can draw onto it later.
lv_obj_t *g_canvas = NULL;

typedef struct {
    lv_point_t img_pos;      // current image top-left coordinate on the canvas
    bool dragging;
    lv_point_t drag_start;   // pointer position when drag started
    lv_point_t img_pos_start;// image position when drag started
} drag_data_t;

// We'll keep this data around
drag_data_t g_drag_data;


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

    // -> Use your HTTP library to download the image into memory:
    // *data = pointer_to_downloaded_bytes;
    // *data_size = number_of_downloaded_bytes;
    //
    // Return true if successful, false otherwise.
    return true; // replace with real implementation
}

// Decodes a raw image buffer (PNG/JPG) into an LVGL image descriptor
// This snippet uses the built-in PNG decoder from LVGL (must be enabled)
// or a custom decoder. Implementation details vary with your build config.
lv_img_dsc_t decode_tile_image(const uint8_t *data, size_t data_size)
{
    lv_img_dsc_t img_dsc;
    memset(&img_dsc, 0, sizeof(img_dsc));
    img_dsc.data = data;
    img_dsc.data_size = data_size;
    img_dsc.header.always_zero = 0;
    img_dsc.header.cf = LV_IMG_CF_RAW_ALPHA; 
    img_dsc.header.w = 256;
    img_dsc.header.h = 256;
    // Typically, you set header.w, header.h, etc., but the LVGL PNG decoder
    // can figure those out automatically if configured properly.
    return img_dsc;
}


void draw_map_tiles(double centerLat, double centerLon, uint8_t zoom)
{
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
    int tilesH = 2; // covers 320 px horizontally
    int tilesV = 3; // covers 480 px vertically (but you might use 3 to be safe)


    uint8_t* tile_data[6];
    size_t tile_data_size[6] = {0};
    lv_img_dsc_t tile_img[6];
    uint8_t tile_index = 0;


    for(int ty = 0; ty < tilesV; ty++)
    {
        for(int tx = 0; tx < tilesH; tx++)
        {
            int tileX = centerTileX + (tx - tilesH/2);
            int tileY = centerTileY + (ty - tilesV/2);

            // Where on the canvas do we draw this tile?
            int drawX = (tx - tilesH/2)*256 + centerOffsetX + 160; // +160 to center it horizontally
            int drawY = (ty - tilesV/2)*256 + centerOffsetY + 240; // +240 to center it vertically


            if(!download_tile_image(tileX, tileY, zoom, &tile_data[tile_index], &tile_data_size[tile_index])) {
                log_e("Download failed");
                // If download fails, skip or show a placeholder
                continue;
            }

            //lv_img_dsc_t tile_img = decode_tile_image(tile_data, tile_data_size);

            memset(&tile_img[tile_index], 0, sizeof(tile_img));
            tile_img[tile_index].data = tile_data[tile_index];
            tile_img[tile_index].data_size = tile_data_size[tile_index];
            tile_img[tile_index].header.always_zero = 0;
            tile_img[tile_index].header.cf = LV_IMG_CF_RAW_ALPHA; // or something appropriate
            tile_img[tile_index].header.w = 256;
            tile_img[tile_index].header.h = 256;

            lv_draw_img_dsc_t draw_dsc;
            lv_draw_img_dsc_init(&draw_dsc);

            // Now draw it on the canvas (in LVGL v7).
            // (x, y) are the coords on the canvas where the top-left of the tile is drawn.
            lv_canvas_draw_img(g_canvas, drawX, drawY, &tile_img[tile_index], &draw_dsc);
            log_i("Draw map to %d, %d", drawX, drawY);

            // Freed after drawing if the decoder doesn’t keep a reference.
            //free(tile_data);
            tile_index++;
        }
    }
    for (int i = 0; i < 9; i++) {
        free(tile_data[i]);
    }

    // Finally refresh the canvas object
    lv_obj_invalidate(g_canvas);
}


void plainspotter_app_main_setup( AppPage* screen) {

    // Create a canvas on the active screen
    g_canvas = lv_canvas_create(screen->handle(), NULL);
    if(!g_canvas) {
        LV_LOG_WARN("Failed to create canvas object");
        return;
    }

    // Assign the local buffer to the canvas
    lv_canvas_set_buffer(
        g_canvas,
        g_canvas_buf,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        LV_IMG_CF_TRUE_COLOR
    );

    // Position the canvas at top-left of the screen (0,0)
    lv_obj_set_pos(g_canvas, 0, 0);
    //lv_obj_set_event_cb(screen->handle(), canvas_event_cb);

    // Optional: set a background fill (just as an example)
    // Fill the canvas buffer with a light gray color
    lv_color_t bg_color = LV_COLOR_SILVER;
    lv_canvas_fill_bg(g_canvas, bg_color, LV_OPA_COVER);

    //refresh_map_tile(5, 9, 12, 50, 50);

    draw_map_tiles(47.4, 8.5, 10 );
    //draw_map_tiles(10, 10, 10 );
}


