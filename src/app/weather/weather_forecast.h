#ifndef _WEATHER_FORECAST_H
    #define _WEATHER_FORECAST_H

    #define WEATHER_FORECAST_SYNC_REQUEST   _BV(0)
    /**
     * @brief calculate weather icon aligning
     */
    #define WEATHER_ICON_SIZE               64                                                                      /** @brief weather icon x/y size in px */
    #define WEATHER_MAX_FORECAST_ICON       RES_X_MAX / WEATHER_ICON_SIZE                                           /** @brief max icon in a row */
    #define WEATHER_FORCAST_ICON_SPACE      ( RES_X_MAX % WEATHER_ICON_SIZE ) / ( RES_X_MAX / WEATHER_ICON_SIZE )   /** @brief space between two icons in px */
    #define WEATHER_MAX_FORECAST            WEATHER_MAX_FORECAST_ICON * 2



    void weather_forecast_tile_setup( uint32_t tile_num );
    void weather_forecast_sync( void );
    void weather_app_task( lv_task_t * task );
    void weather_add_widget( void );
    void weather_remove_widget( void );
    void weather_widget_setup();
    void enter_weather_widget_event_cb( lv_obj_t * obj, lv_event_t event );
    void update_time();

#endif // _WEATHER_FORECAST_H