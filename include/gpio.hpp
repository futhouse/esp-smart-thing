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

#ifndef __GPIO_HPP__
#define __GPIO_HPP__

#include <Arduino.h>
#include <pca9555.h>
#include <pcf8574.h>
#include <mcp23017.h>
#include <memory>

#include "custom/configs.hpp"
#include "logger.hpp"

#define GPIO_MAX_EXTENDERS      8

#define GPIO_PINS_PER_REGULAR_EXTENDER  16
#define GPIO_PINS_PER_PCF8574_EXTENDER  8

#define GPIO_SET_BIT(v, b) \
    do { \
        v |= (1 << (b)); \
    } while (0)

#define GPIO_RESET_BIT(v, b) \
    do { \
        v &= ~(1 << (b)); \
    } while (0)

#define GPIO_IS_BIT(v, b) \
    (v & (1 << (b))) ? true : false

typedef enum _GpioType
{
    GPIO_INTERNAL,
    GPIO_PCA_9555,
    GPIO_PCF_8574,
    GPIO_MCP_23017
} GpioType;

typedef enum _GpioMode
{
    GPIO_INPUT,
    GPIO_INPUT_PULLUP,
    GPIO_OUTPUT
} GpioMode;

typedef enum _GpioIntPin
{
    /* Available for all boards */
    GPIO_0_D3_FLASH,    // Pulled VCC
    GPIO_1_TX,
    GPIO_2_D4,          // Pulled VCC
    GPIO_3_RX,
    GPIO_4_D2_SDA,
    GPIO_5_D1_SCL,
    /* N/A for WeMos */
    GPIO_6_SK_SDCLK,
    GPIO_7_SD_SDD0,
    GPIO_8_S1_SDD1,
    GPIO_9_S2_SDD2,
    GPIO_10_S3_SDD3,
    GPIO_11_SC_SDCMD,
    /* Available for all boards */
    GPIO_12_D6_MISO,
    GPIO_13_D7_MOSI,
    GPIO_14_D5_SCLK,
    GPIO_15_D8_CS,      // Pulled GND
    GPIO_16_D0_WAKE,
    GPIO_ADC0_A0_ADC0   // Analog PIN
} GpioIntPin;

typedef enum _GpioExtPin
{
    GPIO_EXT_0,
    GPIO_EXT_1,
    GPIO_EXT_2,
    GPIO_EXT_3,
    GPIO_EXT_4,
    GPIO_EXT_5,
    GPIO_EXT_6,
    GPIO_EXT_7,
    GPIO_EXT_8,
    GPIO_EXT_9,
    GPIO_EXT_10,
    GPIO_EXT_11,
    GPIO_EXT_12,
    GPIO_EXT_13,
    GPIO_EXT_14,
    GPIO_EXT_15
} GpioExtPin;

typedef enum _GpioAddr
{
    GPIO_ADDR_EXT_20,
    GPIO_ADDR_EXT_21,
    GPIO_ADDR_EXT_22,
    GPIO_ADDR_EXT_23,
    GPIO_ADDR_EXT_24,
    GPIO_ADDR_EXT_25,
    GPIO_ADDR_EXT_26,
    GPIO_ADDR_EXT_27,
    GPIO_ADDR_INTERNAL
} GpioAddr;

typedef enum _GpioState
{
    GPIO_LOW,
    GPIO_HIGH
} GpioState;

typedef struct _GpioStates
{
    bool        IsAlive;
    GpioType    Type;
    uint16_t    Modes;
    uint16_t    States;
} GpioStates;

class IGpio
{
public:
    virtual void setup();
    virtual void loadStates(const GpioConfigs *cfg, const size_t count);
    virtual void getPinStates(GpioConfigs *cfg, const size_t count);
    virtual void setPinMode(const GpioType gType, const uint8_t addr, const GpioMode mode, const uint8_t pin);
    virtual void setPinState(const GpioType gType, const uint8_t addr, const GpioState state, const uint8_t pin);
    virtual GpioState readPin(const GpioType gType, const uint8_t addr, const uint8_t pin);
};

class Gpio: public IGpio
{
public:
    Gpio(const std::shared_ptr<ILogger> &log);

    /**
     * @brief Init all extenders and internal GPIO
     * 
     */
    void setup();

    /**
     * @brief Loading GPIO modes and states from configs
     * 
     * @param cfg Configs pointer
     * @param count Count of extenders
     */
    void loadStates(const GpioConfigs *cfg, const size_t count);

    /**
     * @brief Get the Pin States for saving to configs
     * 
     * @param cfg Configs pointer
     * @param count Count of extenders
     */
    void getPinStates(GpioConfigs *cfg, const size_t count);

    /**
     * @brief Set the Pin Mode of internal or external GPIO
     * 
     * @param gType Internal or External GPIO
     * @param addr Address of extender if GPIO not internal
     * @param mode Input/Output mode
     * @param pin Number of managment pin
     */
    void setPinMode(const GpioType gType, const uint8_t addr, const GpioMode mode, const uint8_t pin);

    /**
     * @brief Set the Pin State of internal or external GPIO
     * 
     * @param gType Internal or External GPIO
     * @param addr Address of extender if GPIO not internal
     * @param mode Input/Output mode
     * @param pin Number of managment pin
     */
    void setPinState(const GpioType gType, const uint8_t addr, const GpioState state, const uint8_t pin);

    /**
     * @brief Read pin state from internal or external GPIO
     * 
     * @param gType Internal or External GPIO
     * @param addr Address of extender if GPIO not internal
     * @param pin Number of managment pin
     * 
     * @return The state of the pin
     */
    GpioState readPin(const GpioType gType, const uint8_t addr, const uint8_t pin);

private:
    const std::shared_ptr<ILogger> _log;

    GpioStates _states[GPIO_MAX_EXTENDERS + 1];
    pca9555 _ext9555[GPIO_MAX_EXTENDERS];
    mcp23017 _ext23017[GPIO_MAX_EXTENDERS];
    pcf8574 _ext8574[GPIO_MAX_EXTENDERS];
};

#endif /* __GPIO_HPP__ */
