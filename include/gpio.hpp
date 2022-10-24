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

#define GPIO_EXTENDERS_COUNT      8

#define GPIO_PINS_PER_REGULAR_EXTENDER  16
#define GPIO_PINS_PER_PCF8574_EXTENDER  8

#ifdef ESP32
#define GPIO_INTERNAL_COUNT 25
#elif defined(ESP8266)
#define GPIO_INTERNAL_COUNT 11
#endif

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
#ifdef ESP32
    GPIO_1_TX0 = 1,
    GPIO_2_D2_A2_2,
    GPIO_3_RX0,
    GPIO_4_D4_A2_0,
    GPIO_5_D5_CS0,
    GPIO_12_D12_A2_5 = 12,
    GPIO_13_D13_A2_4,
    GPIO_14_D14_A2_6,
    GPIO_15_D15_A2_3,
    GPIO_16_RX2,
    GPIO_17_TX2,
    GPIO_18_D18_CLK,
    GPIO_19_D19_MISO,
    GPIO_21_D21_SDA = 21,
    GPIO_22_D22_SCL,
    GPIO_23_D23_MOSI,
    GPIO_25_D25_A2_8 = 25,
    GPIO_26_D26_A2_9,
    GPIO_27_D27_A2_7,
    GPIO_32_D32_A1_4 = 32,
    GPIO_33_D33_A1_5,
    GPIO_34_D34_A1_6,
    GPIO_35_D35_A1_7,
    GPIO_36_VP_A1_0,
    GPIO_39_VN_A1_3 = 39
#elif defined(ESP8266)
    GPIO_0_D3_FLASH,    // Pulled VCC
    GPIO_2_D4 = 2,      // Pulled VCC
    GPIO_3_RX,
    GPIO_4_D2_SDA,
    GPIO_5_D1_SCL,
    GPIO_12_D6_MISO = 12,
    GPIO_13_D7_MOSI,
    GPIO_14_D5_SCLK,
    GPIO_15_D8_CS,      // Pulled GND
    GPIO_16_D0_WAKE,
    GPIO_ADC0_A0_ADC0   // Analog PIN
} GpioIntPin;
#endif

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

typedef enum _GpioExtAddr
{
    GPIO_ADDR_EXT_20,
    GPIO_ADDR_EXT_21,
    GPIO_ADDR_EXT_22,
    GPIO_ADDR_EXT_23,
    GPIO_ADDR_EXT_24,
    GPIO_ADDR_EXT_25,
    GPIO_ADDR_EXT_26,
    GPIO_ADDR_EXT_27
} GpioExtAddr;

typedef enum _GpioState
{
    GPIO_LOW,
    GPIO_HIGH,
    GPIO_NONE
} GpioState;

typedef struct _GpioPin
{
    GpioType    Type;
    uint8_t     Addr;
    uint8_t     Pin;
} GpioPin;

typedef struct _GpioExtender
{
    GpioType    Type;
    uint8_t     Addr;
    bool        IsAlive;
} GpioExtender;

typedef struct _GpioPinName
{
    String      Name;
    GpioIntPin  Pin;
} GpioPinName;

class IGpio
{
public:
    virtual void setup();
    virtual void setPinMode(const GpioPin &pin, const GpioMode mode);
    virtual void setPinState(const GpioPin &pin, const GpioState state);
    virtual GpioState readState(const GpioPin &pin);
    virtual const GpioExtender *getExtenders() const = 0;
    virtual GpioPin strToPin(const String &str) = 0;
    virtual String pinToStr(const GpioPin &pin) = 0;
    virtual void getGpioNames(std::vector<String> &names) = 0;
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
     * @brief Set the Pin Mode of internal or external GPIO
     * 
     * @param pin GPIO configuration
     * @param mode Input/Output mode
     */
    void setPinMode(const GpioPin &pin, const GpioMode mode);

    /**
     * @brief Set the Pin State of internal or external GPIO
     * 
     * @param pin GPIO configuration
     * @param state High/Low state
     */
    void setPinState(const GpioPin &pin, const GpioState state);

    /**
     * @brief Read pin state from internal or external GPIO
     * 
     * @param pin GPIO configuration
     * 
     * @return The state of the pin
     */
    GpioState readState(const GpioPin &pin);

    /**
     * @brief Get GPIO Extenders
     * 
     * @return const GpioExtender* 
     */
    const GpioExtender *getExtenders() const;

    /**
     * @brief Convert String name of GPIO to GpioPin
     * 
     * @param str Name of GPIO
     */
    GpioPin strToPin(const String &str);

    /**
     * @brief Convert Pin struct to string
     * 
     * @param pin GPIO number
     * @return String GPIO name 
     */
    String pinToStr(const GpioPin &pin);

    /**
     * @brief Get the Gpio Names object
     * 
     * @param names Out pins
     */
    void getGpioNames(std::vector<String> &names);

private:
    const std::shared_ptr<ILogger> _log;

    bool _extFound = false;
    GpioExtender _ext[GPIO_EXTENDERS_COUNT];
    pcf8574 _ext8574[GPIO_EXTENDERS_COUNT];
    pca9555 _ext9555[GPIO_EXTENDERS_COUNT];
    mcp23017 _ext23017[GPIO_EXTENDERS_COUNT];
};

#endif /* __GPIO_HPP__ */
