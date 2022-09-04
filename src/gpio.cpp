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

static uint8_t GpioAddrs[] = {
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27
};

Gpio::Gpio(const std::shared_ptr<ILogger> &log):
    _log(move(log))
{
}

void Gpio::setup()
{
    bool extFound = false;

    _log->info("GPIO", "Init GPIO module");

    for (uint8_t ext = 0; ext < GPIO_MAX_EXTENDERS; ext++)
    {
        _ext8574[ext].postSetup(GpioAddrs[ext]);
        if (_ext8574[ext].begin())
        {
            extFound = true;
            _states[ext].IsAlive = true;
            _states[ext].Type = GPIO_PCF_8574;
            _log->info("GPIO", "Extender PCF8574 found at address: " + String(GpioAddrs[ext]));
        }

        _ext9555[ext].postSetup(GpioAddrs[ext]);
        if (_ext9555[ext].begin())
        {
            extFound = true;
            _states[ext].IsAlive = true;
            _states[ext].Type = GPIO_PCA_9555;
            _log->info("GPIO", "Extender PCA9555 found at address: " + String(GpioAddrs[ext]));
        }

        _ext23017[ext].postSetup(GpioAddrs[ext]);
        if (_ext23017[ext].begin())
        {
            extFound = true;
            _states[ext].IsAlive = true;
            _states[ext].Type = GPIO_MCP_23017;
            _log->info("GPIO", "Extender MCP23017 found at address: " + String(GpioAddrs[ext]));
        }
    }

    _states[GPIO_MAX_EXTENDERS].Type = GPIO_INTERNAL;

    if (!extFound)
        _log->info("GPIO", "No GPIO extenders found");
}

void Gpio::loadStates(const GpioConfigs *cfg, const size_t count)
{
    _log->info("GPIO", "Loading GPIO states from configs");
    /**
     * @brief Loading extenders states
     * 
     */
    for (size_t ext = 0; ext < count; ext++)
    {
        if (!_states[ext].IsAlive)
            continue;

        _states[ext].Modes = cfg[ext].Modes;
        _states[ext].States = cfg[ext].States;

        if (_states[ext].Type == GPIO_PCF_8574)
        {
            for (uint8_t pin = 0; pin < GPIO_PINS_PER_PCF8574_EXTENDER; pin++)
            {
                /**
                 * @brief Loading PCF8574 external GPIO modes
                 * 
                 */
                if (GPIO_IS_BIT(_states[ext].Modes, pin))
                {
                    setPinMode(_states[ext].Type, GpioAddrs[ext], GPIO_OUTPUT, pin);
                    _log->info("GPIO", "Type: PCF8574 Addr: " + String(GpioAddrs[ext]) + " Pin: " + String(pin) + " Mode: OUTPUT");
                }
                else
                {
                    setPinMode(_states[ext].Type, GpioAddrs[ext], GPIO_INPUT, pin);
                    _log->info("GPIO", "Type: PCF8574 Addr: " + String(GpioAddrs[ext]) + " Pin: " + String(pin) + " Mode: INPUT");
                }

                /**
                 * @brief Loading PCF8574 external GPIO states
                 * 
                 */
                if (GPIO_IS_BIT(_states[ext].States, pin))
                {
                    setPinState(_states[ext].Type, GpioAddrs[ext], GPIO_HIGH, pin);
                    _log->info("GPIO", "Type: PCF8574 Addr: " + String(GpioAddrs[ext]) + " Pin: " + String(pin) + " State: HIGH");
                }
                else
                {
                    setPinState(_states[ext].Type, GpioAddrs[ext], GPIO_LOW, pin);
                    _log->info("GPIO", "Type: PCF8574 Addr: " + String(GpioAddrs[ext]) + " Pin: " + String(pin) + " State: LOW");
                }
            }
        }
        else
        {
            for (uint8_t pin = 0; pin < GPIO_PINS_PER_REGULAR_EXTENDER; pin++)
            {
                /**
                 * @brief Loading regular external GPIO modes
                 * 
                 */
                if (GPIO_IS_BIT(_states[ext].Modes, pin))
                {
                    setPinMode(_states[ext].Type, GpioAddrs[ext], GPIO_OUTPUT, pin);
                    _log->info("GPIO", "Type: REGULAR_EXT Addr: " + String(GpioAddrs[ext]) + " Pin: " + String(pin) + " Mode: OUTPUT");
                }
                else
                {
                    setPinMode(_states[ext].Type, GpioAddrs[ext], GPIO_INPUT, pin);
                    _log->info("GPIO", "Type: REGULAR_EXT Addr: " + String(GpioAddrs[ext]) + " Pin: " + String(pin) + " Mode: INPUT");
                }

                /**
                 * @brief Loading regular external GPIO states
                 * 
                 */
                if (GPIO_IS_BIT(_states[ext].States, pin))
                {
                    setPinState(_states[ext].Type, GpioAddrs[ext], GPIO_HIGH, pin);
                    _log->info("GPIO", "Type: REGULAR_EXT Addr: " + String(GpioAddrs[ext]) + " Pin: " + String(pin) + " State: HIGH");
                }
                else
                {
                    setPinState(_states[ext].Type, GpioAddrs[ext], GPIO_LOW, pin);
                    _log->info("GPIO", "Type: REGULAR_EXT Addr: " + String(GpioAddrs[ext]) + " Pin: " + String(pin) + " State: LOW");
                }
            }
        }
    }

    /**
     * @brief Loading onboard board GPIO states
     * 
     */
    for (uint8_t pin = 0; pin < GPIO_PINS_PER_REGULAR_EXTENDER; pin++)
    {
        /**
         * @brief Loading internal GPIO modes
         * 
         */
        if (GPIO_IS_BIT(_states[GPIO_MAX_EXTENDERS].Modes, pin))
        {
            setPinMode(_states[GPIO_MAX_EXTENDERS].Type, 0, GPIO_OUTPUT, pin);
            _log->info("GPIO", "Type: INTERNAL Addr: " + String(0) + " Pin: " + String(pin) + " Mode: OUTPUT");
        }
        else
        {
            setPinMode(_states[GPIO_MAX_EXTENDERS].Type, 0, GPIO_INPUT, pin);
            _log->info("GPIO", "Type: INTERNAL Addr: " + String(0) + " Pin: " + String(pin) + " Mode: INPUT");
        }

        /**
         * @brief Loading internal GPIO states
         * 
         */
        if (GPIO_IS_BIT(_states[GPIO_MAX_EXTENDERS].States, pin))
        {
            setPinState(_states[GPIO_MAX_EXTENDERS].Type, 0, GPIO_HIGH, pin);
            _log->info("GPIO", "Type: INTERNAL Addr: " + String(0) + " Pin: " + String(pin) + " State: LOW");
        }
        else
        {
            setPinState(_states[GPIO_MAX_EXTENDERS].Type, 0, GPIO_LOW, pin);
            _log->info("GPIO", "Type: INTERNAL Addr: " + String(0) + " Pin: " + String(pin) + " State: LOW");
        }
    }
}

void Gpio::getPinStates(GpioConfigs *cfg, const size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        cfg[i].Modes = _states[i].Modes;
        cfg[i].States = _states[i].States;
    }
}

void Gpio::setPinMode(const GpioType gType, const uint8_t addr, const GpioMode mode, const uint8_t pin)
{
    if (gType == GPIO_INTERNAL)
    {
        if (mode == GPIO_INPUT)
            pinMode(pin, INPUT);
        else
            pinMode(pin, OUTPUT);
    }
    else if (gType == GPIO_MCP_23017)
    {
        if (mode == GPIO_INPUT)
            _ext23017[addr].gpioPinMode(pin, INPUT);
        else
            _ext23017[addr].gpioPinMode(pin, OUTPUT);
    }
    else if (gType == GPIO_PCF_8574)
    {
        if (mode == GPIO_INPUT)
            _ext8574[addr].gpioPinMode(pin, INPUT);
        else
            _ext8574[addr].gpioPinMode(pin, OUTPUT);
    }
    else if (gType == GPIO_PCA_9555)
    {
        if (mode == GPIO_INPUT)
            _ext9555[addr].gpioPinMode(pin, INPUT);
        else
            _ext9555[addr].gpioPinMode(pin, OUTPUT);
    }

    if (mode == GPIO_INPUT)
        GPIO_RESET_BIT(_states[addr].Modes, pin);
    else
        GPIO_SET_BIT(_states[addr].Modes, pin);
}

void Gpio::setPinState(const GpioType gType, const uint8_t addr, const GpioState state, const uint8_t pin)
{
    if (gType == GPIO_INTERNAL)
    {
        if (state == GPIO_LOW)
            digitalWrite(pin, LOW);
        else
            digitalWrite(pin, HIGH);
    }
    else if (gType == GPIO_MCP_23017)
    {
        if (state == GPIO_LOW)
            _ext23017[addr].gpioDigitalWrite(pin, LOW);
        else
            _ext23017[addr].gpioDigitalWrite(pin, HIGH);
    }
    else if (gType == GPIO_PCF_8574)
    {
        if (state == GPIO_LOW)
            _ext8574[addr].gpioDigitalWrite(pin, LOW);
        else
            _ext8574[addr].gpioDigitalWrite(pin, HIGH);
    }
    else if (gType == GPIO_PCA_9555)
    {
        if (state == GPIO_LOW)
            _ext9555[addr].gpioDigitalWrite(pin, LOW);
        else
            _ext9555[addr].gpioDigitalWrite(pin, HIGH);
    }

    if (state == GPIO_LOW)
        GPIO_RESET_BIT(_states[addr].States, pin);
    else
        GPIO_SET_BIT(_states[addr].States, pin);
}

GpioState Gpio::readPin(const GpioType gType, const uint8_t addr, const uint8_t pin)
{
    if (gType == GPIO_INTERNAL)
    {
        if (digitalRead(pin) == LOW)
            return GPIO_LOW;
        else
            return GPIO_HIGH;
    }
    else if (gType == GPIO_MCP_23017)
    {
        if (_ext23017[addr].gpioDigitalRead(pin) == LOW)
            return GPIO_LOW;
        else
            return GPIO_HIGH;
    }
    else if (gType == GPIO_PCF_8574)
    {
        if (_ext8574[addr].gpioDigitalRead(pin) == LOW)
            return GPIO_LOW;
        else
            return GPIO_HIGH;
    }
    else if (gType == GPIO_PCA_9555)
    {
        if (_ext9555[addr].gpioDigitalRead(pin) == LOW)
            return GPIO_LOW;
        else
            return GPIO_HIGH;
    }

    return GPIO_LOW;
}
