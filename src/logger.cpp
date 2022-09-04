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

#include "logger.hpp"

/**
 * @brief Init Log module
 * 
 */
void Logger::setup()
{
    Serial.begin(LOG_SERIAL_SPEED);
}

/**
 * @brief Information log message
 * 
 * @param module App module
 * @param msg Log message
 */
void Logger::info(const String &module, const String &msg)
{
    Serial.println("[INFO][" + module + "] " + msg);
}

/**
 * @brief Error log message
 * 
 * @param module App module
 * @param msg Log message
 */
void Logger::error(const String &module, const String &msg)
{
    Serial.println("[ERROR][" + module + "] " + msg);
}
