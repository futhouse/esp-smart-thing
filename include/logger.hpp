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

#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include <Arduino.h>

#define LOG_SERIAL_SPEED    115200

class ILogger
{
public:
    virtual void setup() = 0;
    virtual void info(const String &module, const String &msg) = 0;
    virtual void error(const String &module, const String &msg) = 0;
};

class Logger: public ILogger
{
public:
    /**
     * @brief Init Log module
     * 
     */
    void setup();

    /**
     * @brief Information log message
     * 
     * @param module App module
     * @param msg Log message
     */
    void info(const String &module, const String &msg);

    /**
     * @brief Error log message
     * 
     * @param module App module
     * @param msg Log message
     */
    void error(const String &module, const String &msg);
};

#endif /* __LOGGER_HPP__ */
