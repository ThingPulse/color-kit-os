/****************************************************************************
 *   Aug 11 17:13:51 2020
 *   Copyright  2020  Dirk Brosswick
 *   Email: dirk.brosswick@googlemail.com
 ****************************************************************************/
 
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include "stock_ticker_config.h"

stock_ticker_config_t::stock_ticker_config_t() : BaseJsonConfig( STOCK_TICKER_JSON_CONFIG_FILE) {
}

bool stock_ticker_config_t::onSave(JsonDocument& doc) {
  doc["apikey"] = apikey;

  JsonArray symbols = doc.createNestedArray("symbols");
  for (int i = 0; i < MAX_STOCK_SYMBOLS; i++) {
      if (stock_symbols[i][0] != '\0') { // only add non-empty symbols
          symbols.add(stock_symbols[i]); // <-- this was the mistake
      }
  }

  return true;
}

bool stock_ticker_config_t::onLoad(JsonDocument& doc) {

    strncpy( apikey, doc["apikey"], sizeof( apikey ) );
    JsonArray symbols = doc["symbols"];
    int i = 0;
    for (const char* symbol : symbols) {
        if (i < MAX_STOCK_SYMBOLS) {
          // Copy safely into the 2D array
          strncpy(stock_symbols[i], symbol, MAX_SYMBOL_LENGTH - 1);
          stock_symbols[i][MAX_SYMBOL_LENGTH - 1] = '\0'; // Null-terminate
          i++;
        } else {
          break; // Prevent overflow if more symbols are in the JSON
        }
      }


    return true;
}

bool stock_ticker_config_t::onDefault( void ) {
    return true;
}