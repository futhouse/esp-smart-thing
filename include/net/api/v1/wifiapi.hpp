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

#ifndef __WIFI_API_HPP__
#define __WIFI_API_HPP__

#include <Arduino.h>

#include "custom/modules.hpp"
#include "flash.hpp"
#include "net/espsrv.hpp"
#include "../api.hpp"
#include "gpio.hpp"
#include "modules/network.hpp"

class WifiApi: public IApi
{
public:
    WifiApi(const std::shared_ptr<IGpio> &gpio,
            const std::shared_ptr<IFlash> &flash,
            const std::shared_ptr<INetwork> &net);

    void registerHandlers(const std::shared_ptr<EspServer> &server);

private:
    const std::shared_ptr<IGpio> _gpio;
    const std::shared_ptr<IFlash> _flash;
    std::shared_ptr<EspServer> _server;
    const std::shared_ptr<INetwork> _net;

    void wifiInfoHandler(AsyncWebServerRequest *req) const;
    void wifiHandler(AsyncWebServerRequest *req) const;
    void wifiConfHandler(AsyncWebServerRequest *req);
};

#endif /* __WIFI_API_HPP__ */
