#pragma once

#define LGFX_USE_V1


#include <LovyanGFX.hpp>
#include <driver/i2c.h>


class ColorKitGrande : public lgfx::LGFX_Device {

  lgfx::Panel_ILI9488 _panel_instance;
  lgfx::Bus_SPI _bus_instance;
  //lgfx::Light_PWM _light_instance;
  //lgfx::Touch_FT5x06 _touch_instance;

public:
  ColorKitGrande(void) {
    {
      auto cfg = _bus_instance.config();

      cfg.freq_write = 40000000;
      cfg.freq_read = 16000000;
      cfg.dma_channel = 1;
      cfg.pin_sclk = TFT_SCLK;
      cfg.pin_mosi = TFT_MOSI;
      cfg.pin_miso = TFT_MISO;
      cfg.pin_dc = TFT_DC;

      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    {
      auto cfg = _panel_instance.config();

      cfg.pin_cs = TFT_CS;
      cfg.pin_rst = TFT_RST;

      cfg.memory_width = 320;
      cfg.memory_height = 480;
      cfg.panel_width = 320;
      cfg.panel_height = 480;
      cfg.offset_rotation = 2;

      _panel_instance.config(cfg);
    }


    setPanel(&_panel_instance);
  }
};
