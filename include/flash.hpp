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

#ifndef __FLASH_HPP__
#define __FLASH_HPP__

#include <Arduino.h>

#include "custom/configs.hpp"

#define FLASH_SIZE  1024

class IFlash
{
public:
    virtual void setup() = 0;
    virtual bool saveData() = 0;
    virtual void loadData() = 0;
    virtual Configs *getConfigs() = 0;
};

class Flash: public IFlash
{
public:
    /**
     * @brief Init Flash module
     * 
     */
    void setup();

    /**
     * @brief Save configs to EEPROM
     * 
     * @return true If configs saved
     * @return false If configs not saved
     */
    bool saveData();

    /**
     * @brief Loading configs from EEPROM
     * 
     */
    void loadData();

    /**
     * @brief Get the Configs object
     * 
     * @return const Configs&
     */
    Configs *getConfigs();

private:
    Configs _cfg;
};

#endif /* __FLASH_HPP__ */
