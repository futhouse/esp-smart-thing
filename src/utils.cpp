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

#include "utils.hpp"

void HexArrayToStr(uint8_t* arr, size_t size, char out[])
{
    for (size_t i = 0; i < size; i++)
    {
        byte nib1 = (arr[i] >> 4) & 0x0F;
        byte nib2 = (arr[i] >> 0) & 0x0F;
        out[i * 2 + 0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
        out[i * 2 + 1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
    }
    out[size * 2] = '\0';
}

String BoolToStr(bool value)
{
    return (value == true) ? "true" : "false";
}
