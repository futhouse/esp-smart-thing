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

#include <EEPROM.h>

#include "flash.hpp"

void Flash::setup()
{
    EEPROM.begin(FLASH_SIZE);
}

void Flash::loadData()
{
    memset(&_cfg, 0x0, sizeof(Configs));
    EEPROM.get(0, _cfg);
}

bool Flash::saveData()
{
    EEPROM.put(0, _cfg);
    return EEPROM.commit();
}

Configs* Flash::getConfigs()
{
    return &_cfg;
}
