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

#ifndef __GPIO_API_HPP__
#define __GPIO_API_HPP__

#include <Arduino.h>

#include "custom/modules.hpp"
#include "gpio.hpp"
#include "net/espsrv.hpp"
#include "../api.hpp"

class GpioApi: public IApi
{
public:
    GpioApi(const std::shared_ptr<IGpio> &gpio);

    void registerHandlers(const std::shared_ptr<EspServer> &server);

private:
    const std::shared_ptr<IGpio> _gpio;
    std::shared_ptr<EspServer> _server;

    void gpioInfoHandler(AsyncWebServerRequest *req) const;
};

#endif /* __GPIO_API_HPP__ */
