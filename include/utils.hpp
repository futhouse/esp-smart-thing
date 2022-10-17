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

#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <Arduino.h>

/**
 * @brief Convert Hex values to string
 * 
 * @param arr Hex array
 * @param size Size of Hex array
 * @param out Out string
 */
void HexArrayToStr(uint8_t* arr, size_t size, char out[]);

/**
 * @brief Convert boolean to string
 * 
 * @param value Boolean value
 * @return String Output string
 */
String BoolToStr(bool value);

/**
 * @brief Convert String to boolean
 * 
 * @param value String value
 * @return true 
 * @return false 
 */
bool StrToBool(const String& value);

#endif /* __UTILS_HPP__ */
