#ifndef _STOCK_TICKER_FETCH_H
    #define _STOCK_TICKER_FETCH_H



    typedef struct stock_ticker_data {
        bool valide = false;
        char name[32] = "";
        float price = 0.0f;
        float change = 0.0f;
        float change_percentage = 0.0f;
    } stock_ticker_data_t;

    int stock_ticker_fetch_symbols( stock_ticker_data_t * data);

#endif // _WEATHER_FETCH_H