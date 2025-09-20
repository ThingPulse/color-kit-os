#include "stock_ticker_fetch.h"
#include "hardware/powermgm.h"
#include "utils/json_psram_allocator.h"
#include "utils/uri_load/uri_load.h"
#include "stock_ticker_app.h"

int stock_ticker_fetch_symbols( stock_ticker_data_t * data) {
    char url[512]="";
    int httpcode = -1;

    /**
     * build uri string
     */
    stock_ticker_config_t * config = get_stock_ticker_config();
    snprintf( url, sizeof( url ), "https://finnhub.io/api/v1/quote?symbol=%s&token=%s", 
        config->stock_symbols[0], config->apikey);
    log_d("http get: %s", url );
    /**
     * load uri file into ram
     */
    uri_load_dsc_t *uri_load_dsc = uri_load_to_ram( url );
    /**
     * if was success, pharse the json
     */
    /* if ( uri_load_dsc ) {
        SpiRamJsonDocument doc( uri_load_dsc->size * 3 );
        log_i("Http code: %d", httpcode);

        DeserializationError error = deserializeJson( doc, uri_load_dsc->data );
        if (error) {
            log_e("weather today deserializeJson() failed: %s (%d bytes)", error.c_str(), uri_load_dsc->size );
            doc.clear();
            uri_load_free_all( uri_load_dsc );
            data[0]->valide = false;
            return( httpcode );
        }

        if( doc.containsKey("c") ) {
            if ( doc["cod"].as<uint32_t>() == 200 ) {
                data->valide = true;
                strcpy( weather_today->icon, doc["weather"][0]["icon"] );
                weather_today->temp = doc["main"]["temp"].as<float>();
                weather_today->humidity = doc["main"]["humidity"].as<float>();
                weather_today->pressure = doc["main"]["pressure"].as<float>();
                strcpy( weather_today->icon, doc["weather"][0]["icon"] );
                strcpy( weather_today->description, doc["weather"][0]["description"] );
                strcpy( weather_today->name, doc["name"] );

                weather_today->wind_sp33d = doc["wind"]["speed"].as<int>();
                weather_today->wind_deg =  doc["wind"]["deg"].as<int>();
                httpcode = 200;
            } else {
                weather_today->valide = false;
            }
        } else {
            weather_today->valide = false;
        }

        doc.clear();
    }
    else {
        weather_today->valide = false;
        httpcode = -1;
    }*/
    /**
     * clear uri dsc
     */
    uri_load_free_all( uri_load_dsc );

    return( httpcode );
}