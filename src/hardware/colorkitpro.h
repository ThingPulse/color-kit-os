#pragma once

#define LGFX_USE_V1

#include <LovyanGFX.hpp>
#include <driver/i2c.h>



class ColorKitPro : public lgfx::LGFX_Device
{
  static constexpr int I2C_PORT_NUM = I2C_NUM_0;
  static constexpr int I2C_PIN_SDA = 40;
  static constexpr int I2C_PIN_SCL = 41;
  static constexpr int I2C_PIN_INT = 6;

  lgfx::Bus_Parallel16 _bus_instance;
  lgfx::Panel_ILI9488 _panel_instance;
  //lgfx::Light_PWM     _light_instance;
  //lgfx::ITouch*  _touch_instance_ptr = nullptr;

  /// Detects and configures the touch panel during initialization;
  bool init_impl(bool use_reset, bool use_clear) override
  {
    /*if (_touch_instance_ptr == nullptr)
    {
      lgfx::ITouch::config_t cfg;
      lgfx::i2c::init(I2C_PORT_NUM, I2C_PIN_SDA, I2C_PIN_SCL);
      if (lgfx::i2c::beginTransaction(I2C_PORT_NUM, 0x38, 400000, false).has_value()
       && lgfx::i2c::endTransaction(I2C_PORT_NUM).has_value())
      {
        _touch_instance_ptr = new lgfx::Touch_FT5x06();
        cfg = _touch_instance_ptr->config();
        cfg.i2c_addr = 0x38;
        cfg.x_max = 320;
        cfg.y_max = 480;
      }
      else
      if (lgfx::i2c::beginTransaction(I2C_PORT_NUM, 0x48, 400000, false).has_value()
       && lgfx::i2c::endTransaction(I2C_PORT_NUM).has_value())
      {
        _touch_instance_ptr = new lgfx::Touch_NS2009();
        cfg = _touch_instance_ptr->config();
        cfg.i2c_addr = 0x48;
        cfg.x_min = 368;
        cfg.y_min = 212;
        cfg.x_max = 3800;
        cfg.y_max = 3800;
      }
      if (_touch_instance_ptr != nullptr)
      {
        cfg.i2c_port = I2C_PORT_NUM;
        cfg.pin_sda  = I2C_PIN_SDA;
        cfg.pin_scl  = I2C_PIN_SCL;
        cfg.pin_int  = I2C_PIN_INT;
        cfg.freq = 400000;
        cfg.bus_shared = false;
        _touch_instance_ptr->config(cfg);
        _panel_instance.touch(_touch_instance_ptr);
      }
    }*/
    return lgfx::LGFX_Device::init_impl(use_reset, use_clear);
  }

public:

  ColorKitPro(void)
  {
    {
      auto cfg = _bus_instance.config();

      //cfg.freq_write = 40000000;
      //cfg.freq_read  = 20000000;
      cfg.pin_wr = PIN_TFT_WR;
      cfg.pin_rd = PIN_TFT_RD;
      cfg.pin_rs = PIN_TFT_RS;

      cfg.pin_d0 = PIN_TFT_D0;
      cfg.pin_d1 = PIN_TFT_D1;
      cfg.pin_d2 = PIN_TFT_D2;
      cfg.pin_d3 = PIN_TFT_D3;
      cfg.pin_d4 = PIN_TFT_D4;
      cfg.pin_d5 = PIN_TFT_D5;
      cfg.pin_d6 = PIN_TFT_D6;
      cfg.pin_d7 = PIN_TFT_D7;
      cfg.pin_d8 = PIN_TFT_D8;
      cfg.pin_d9 = PIN_TFT_D9;
      cfg.pin_d10 = PIN_TFT_D10;
      cfg.pin_d11 = PIN_TFT_D11;
      cfg.pin_d12 = PIN_TFT_D12;
      cfg.pin_d13 = PIN_TFT_D13;
      cfg.pin_d14 = PIN_TFT_D14;
      cfg.pin_d15 = PIN_TFT_D15;
      _bus_instance.config(cfg);
      _panel_instance.bus(&_bus_instance);
    }

    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs          =    PIN_TFT_CS;
      cfg.pin_rst         =    PIN_TFT_RST;
      cfg.pin_busy        =    -1;
      cfg.offset_rotation =     2;
      cfg.readable        =  true;
      cfg.invert          = false;
      cfg.rgb_order       = false;
      cfg.dlen_16bit      =  true;
      cfg.bus_shared      = false;
      _panel_instance.config(cfg);
    }

    setPanel(&_panel_instance);
  }
};