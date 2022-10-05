/*****************************************************************************
 *
 * Future House Technologies
 *
 * Copyright (C) 2022 - Denisov Foundation Limited
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This application is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence 3
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 *****************************************************************************/

#include "gpio.hpp"

const GpioPinName IntPinsNames[] = {
#ifdef ESP32
    { "GPIO_1_TX0", GPIO_1_TX0 },
    { "GPIO_2_D2_A2_2", GPIO_2_D2_A2_2 },
    { "GPIO_3_RX0", GPIO_3_RX0 },
    { "GPIO_4_D4_A2_0", GPIO_4_D4_A2_0 },
    { "GPIO_5_D5_CS0", GPIO_5_D5_CS0 },
    { "GPIO_12_D12_A2_5", GPIO_12_D12_A2_5 },
    { "GPIO_13_D13_A2_4", GPIO_13_D13_A2_4 },
    { "GPIO_14_D14_A2_6", GPIO_14_D14_A2_6 },
    { "GPIO_15_D15_A2_3", GPIO_15_D15_A2_3 },
    { "GPIO_16_RX2", GPIO_16_RX2 },
    { "GPIO_17_TX2", GPIO_17_TX2 },
    { "GPIO_18_D18_CLK", GPIO_18_D18_CLK },
    { "GPIO_19_D19_MISO", GPIO_19_D19_MISO },
    { "GPIO_21_D21_SDA", GPIO_21_D21_SDA },
    { "GPIO_22_D22_SCL", GPIO_22_D22_SCL },
    { "GPIO_23_D23_MOSI", GPIO_23_D23_MOSI },
    { "GPIO_25_D25_A2_8", GPIO_25_D25_A2_8 },
    { "GPIO_26_D26_A2_9", GPIO_26_D26_A2_9 },
    { "GPIO_27_D27_A2_7", GPIO_27_D27_A2_7 },
    { "GPIO_32_D32_A1_4", GPIO_32_D32_A1_4 },
    { "GPIO_33_D33_A1_5", GPIO_33_D33_A1_5 },
    { "GPIO_34_D34_A1_6", GPIO_34_D34_A1_6 },
    { "GPIO_35_D35_A1_7", GPIO_35_D35_A1_7 },
    { "GPIO_36_VP_A1_0", GPIO_36_VP_A1_0 },
    { "GPIO_39_VN_A1_3", GPIO_39_VN_A1_3 },
#elif defined(ESP8266)
    { "GPIO_0_D3_FLASH", GPIO_0_D3_FLASH },
    { "GPIO_2_D4", GPIO_2_D4 },
    { "GPIO_3_RX", GPIO_3_RX },
    { "GPIO_4_D2_SDA", GPIO_4_D2_SDA },
    { "GPIO_5_D1_SCL", GPIO_5_D1_SCL },
    { "GPIO_12_D6_MISO", GPIO_12_D6_MISO },
    { "GPIO_13_D7_MOSI", GPIO_13_D7_MOSI }, 
    { "GPIO_14_D5_SCLK", GPIO_14_D5_SCLK },
    { "GPIO_15_D8_CS", GPIO_15_D8_CS },
    { "GPIO_16_D0_WAKE", GPIO_16_D0_WAKE },
    { "GPIO_ADC0_A0_ADC0", GPIO_ADC0_A0_ADC0 },
#endif
};

Gpio::Gpio(const std::shared_ptr<ILogger> &log):
    _log(move(log))
{
    _ext[GPIO_ADDR_EXT_20].Addr = 0x20;
    _ext[GPIO_ADDR_EXT_21].Addr = 0x21;
    _ext[GPIO_ADDR_EXT_22].Addr = 0x22;
    _ext[GPIO_ADDR_EXT_23].Addr = 0x23;
    _ext[GPIO_ADDR_EXT_24].Addr = 0x24;
    _ext[GPIO_ADDR_EXT_25].Addr = 0x25;
    _ext[GPIO_ADDR_EXT_26].Addr = 0x26;
    _ext[GPIO_ADDR_EXT_27].Addr = 0x27;
}

void Gpio::setup()
{
    _log->info("GPIO", "Init GPIO module");

    for (uint8_t i = 0; i < GPIO_EXTENDERS_COUNT; i++)
    {
        _ext8574[i].postSetup(_ext[i].Addr);
        if (_ext8574[i].begin())
        {
            _extFound = true;
            _ext[i].IsAlive = true;
            _ext[i].Type = GPIO_PCF_8574;
            _log->info("GPIO", "Extender PCF8574 found at address: " + String(_ext[i].Addr));
        }

        _ext9555[i].postSetup(_ext[i].Addr);
        if (_ext9555[i].begin())
        {
            _extFound = true;
            _ext[i].IsAlive = true;
            _ext[i].Type = GPIO_PCA_9555;
            _log->info("GPIO", "Extender PCA9555 found at address: " + String(_ext[i].Addr));
        }

        _ext23017[i].postSetup(_ext[i].Addr);
        if (_ext23017[i].begin())
        {
            _extFound = true;
            _ext[i].IsAlive = true;
            _ext[i].Type = GPIO_MCP_23017;
            _log->info("GPIO", "Extender MCP23017 found at address: " + String(_ext[i].Addr));
        }
    }

    if (!_extFound)
    {
        _extFound = false;
        _log->info("GPIO", "No GPIO extenders found");
    }
}

void Gpio::setPinMode(const GpioPin &pin, const GpioMode mode)
{
    if (pin.Type == GPIO_INTERNAL)
    {
        if (mode == GPIO_INPUT)
            pinMode(pin.Pin, INPUT);
        else
            pinMode(pin.Pin, OUTPUT);
    }
    else if (pin.Type == GPIO_MCP_23017)
    {
        if (mode == GPIO_INPUT)
            _ext23017[pin.Addr].gpioPinMode(pin.Pin, INPUT);
        else
            _ext23017[pin.Addr].gpioPinMode(pin.Pin, OUTPUT);
    }
    else if (pin.Type == GPIO_PCF_8574)
    {
        if (mode == GPIO_INPUT)
            _ext8574[pin.Addr].gpioPinMode(pin.Pin, INPUT);
        else
            _ext8574[pin.Addr].gpioPinMode(pin.Pin, OUTPUT);
    }
    else if (pin.Type == GPIO_PCA_9555)
    {
        if (mode == GPIO_INPUT)
            _ext9555[pin.Addr].gpioPinMode(pin.Pin, INPUT);
        else
            _ext9555[pin.Addr].gpioPinMode(pin.Pin, OUTPUT);
    }
}

void Gpio::setPinState(const GpioPin &pin, const GpioState state)
{
    if (pin.Type == GPIO_INTERNAL)
    {
        if (state == GPIO_LOW)
            digitalWrite(pin.Pin, LOW);
        else
            digitalWrite(pin.Pin, HIGH);
    }
    else if (pin.Type == GPIO_MCP_23017)
    {
        if (state == GPIO_LOW)
            _ext23017[pin.Addr].gpioDigitalWrite(pin.Pin, LOW);
        else
            _ext23017[pin.Addr].gpioDigitalWrite(pin.Pin, HIGH);
    }
    else if (pin.Type == GPIO_PCF_8574)
    {
        if (state == GPIO_LOW)
            _ext8574[pin.Addr].gpioDigitalWrite(pin.Pin, LOW);
        else
            _ext8574[pin.Addr].gpioDigitalWrite(pin.Pin, HIGH);
    }
    else if (pin.Type == GPIO_PCA_9555)
    {
        if (state == GPIO_LOW)
            _ext9555[pin.Addr].gpioDigitalWrite(pin.Pin, LOW);
        else
            _ext9555[pin.Addr].gpioDigitalWrite(pin.Pin, HIGH);
    }
}

GpioState Gpio::readState(const GpioPin &pin)
{
    if (pin.Type == GPIO_INTERNAL)
    {
        if (digitalRead(pin.Pin) == LOW)
            return GPIO_LOW;
        else
            return GPIO_HIGH;
    }
    else if (pin.Type == GPIO_MCP_23017)
    {
        if (_ext23017[pin.Addr].gpioDigitalRead(pin.Pin) == LOW)
            return GPIO_LOW;
        else
            return GPIO_HIGH;
    }
    else if (pin.Type == GPIO_PCF_8574)
    {
        if (_ext8574[pin.Addr].gpioDigitalRead(pin.Pin) == LOW)
            return GPIO_LOW;
        else
            return GPIO_HIGH;
    }
    else if (pin.Type == GPIO_PCA_9555)
    {
        if (_ext9555[pin.Addr].gpioDigitalRead(pin.Pin) == LOW)
            return GPIO_LOW;
        else
            return GPIO_HIGH;
    }

    return GPIO_LOW;
}

const GpioExtender *Gpio::getExtenders()
{
    if (_extFound)
        return _ext;
    else
        return nullptr;
}

String Gpio::pinToStr(const GpioPin &pin)
{
    if (pin.Type == GPIO_INTERNAL)
    {
        for (uint8_t i = 0; i < GPIO_INTERNAL_COUNT; i++)
        {
            if (IntPinsNames[i].Pin == pin.Pin)
                return IntPinsNames[i].Name;
        }
    } else {
        for (uint8_t i = 0; i < GPIO_EXTENDERS_COUNT; i++)
        {
            if (pin.Type == GPIO_PCA_9555)
            {
                for (uint8_t j = 0; j < GPIO_PINS_PER_REGULAR_EXTENDER; j++)
                {
                    if ((pin.Addr == i) && (pin.Pin == j))
                        return ("GPIO_PCA9555_" + String(_ext[i].Addr) + "_" + String(j));
                }
            } else if (pin.Type == GPIO_PCF_8574)
            {
                for (uint8_t j = 0; j < GPIO_PINS_PER_PCF8574_EXTENDER; j++)
                {
                    if ((pin.Addr == i) && (pin.Pin == j))
                        return ("GPIO_PCF8574_" + String(_ext[i].Addr) + "_" + String(j));
                }
            } else if (pin.Type == GPIO_MCP_23017)
            {
                for (uint8_t j = 0; j < GPIO_PINS_PER_REGULAR_EXTENDER; j++)
                {
                    if ((pin.Addr == i) && (pin.Pin == j))
                        return ("GPIO_MCP23017_" + String(_ext[i].Addr) + "_" + String(j));
                }
            }
        }
    }
    return "Not Found";
}

GpioPin Gpio::strToPin(const String &str)
{
    GpioPin pin;

    for (uint8_t i = 0; i < GPIO_INTERNAL_COUNT; i++)
    {
        if (IntPinsNames[i].Name == str)
        {
            pin.Type = GPIO_INTERNAL;
            pin.Pin = IntPinsNames[i].Pin;
            return pin;
        }
    }

    for (uint8_t i = 0; i < GPIO_EXTENDERS_COUNT; i++)
    {
        if (_ext[i].Type == GPIO_PCF_8574)
        {
            for (uint8_t j = 0; j < GPIO_PINS_PER_PCF8574_EXTENDER; j++)
            {
                if (str == ("GPIO_PCF8574_" + String(_ext[i].Addr) + "_" + String(j)))
                {
                    pin.Type = GPIO_PCF_8574;
                    pin.Addr = i;
                    pin.Pin = j;
                    return pin;
                }
            }
        }
        else if (_ext[i].Type == GPIO_MCP_23017)
        {
            for (uint8_t j = 0; j < GPIO_PINS_PER_REGULAR_EXTENDER; j++)
            {
                if (str == ("GPIO_MCP23017_" + String(_ext[i].Addr) + "_" + String(j)))
                {
                    pin.Type = GPIO_MCP_23017;
                    pin.Addr = i;
                    pin.Pin = j;
                    return pin;
                }
            }
        }
        else if (_ext[i].Type == GPIO_PCA_9555)
        {
            for (uint8_t j = 0; j < GPIO_PINS_PER_REGULAR_EXTENDER; j++)
            {
                if (str == ("GPIO_PCA9555_" + String(_ext[i].Addr) + "_" + String(j)))
                {
                    pin.Type = GPIO_PCA_9555;
                    pin.Addr = i;
                    pin.Pin = j;
                    return pin;
                }
            }
        }
    }

    return pin;
}

void Gpio::getGpioNames(String &names)
{
    names = "";

    for (uint8_t i = 0; i < GPIO_INTERNAL_COUNT; i++)
    {
        names += "\"" + IntPinsNames[i].Name + "\"";
        if (i < GPIO_INTERNAL_COUNT - 1)
            names += ",";
    }

    for (uint8_t i = 0; i < GPIO_EXTENDERS_COUNT; i++)
    {
        if (_ext[i].Type == GPIO_PCF_8574)
        {
            names += ",";
            for (uint8_t j = 0; j < GPIO_PINS_PER_PCF8574_EXTENDER; j++)
            {
                names += "\"GPIO_PCF8574_" + String(_ext[i].Addr) + "_" + String(j);
                if (j != GPIO_PINS_PER_PCF8574_EXTENDER - 1)
                    names += "\",";
            }
        }
        else if (_ext[i].Type == GPIO_MCP_23017)
        {
            names += ",";
            for (uint8_t j = 0; j < GPIO_PINS_PER_REGULAR_EXTENDER; j++)
            {
                names += "\"GPIO_MCP23017_" + String(_ext[i].Addr) + "_" + String(j);
                if (j != GPIO_PINS_PER_REGULAR_EXTENDER - 1)
                    names += "\",";
            }
        }
        else if (_ext[i].Type == GPIO_PCA_9555)
        {
            names += ",";
            for (uint8_t j = 0; j < GPIO_PINS_PER_REGULAR_EXTENDER; j++)
            {
                names += "\"GPIO_PCA9555_" + String(_ext[i].Addr) + "_" + String(j);
                if (j != GPIO_PINS_PER_REGULAR_EXTENDER - 1)
                    names += "\",";
            }
        }
    }
}
