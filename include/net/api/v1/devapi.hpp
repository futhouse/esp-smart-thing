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

#ifndef __DEV_API_HPP__
#define __DEV_API_HPP__

#include <Arduino.h>

#include "custom/modules.hpp"
#include "flash.hpp"
#include "net/espsrv.hpp"
#include "../api.hpp"
#include "modules/network.hpp"

class DeviceApi: public IApi
{
public:
    DeviceApi(const std::shared_ptr<IFlash>& flash,
              const std::shared_ptr<INetwork>& net);

    void registerHandlers(const std::shared_ptr<EspServer> &server);

private:
    const std::shared_ptr<IFlash> _flash;
    const std::shared_ptr<INetwork> _net;
    std::shared_ptr<EspServer> _server;

    void devInfoHandler(AsyncWebServerRequest *req) const;
    void devConfHandler(AsyncWebServerRequest *req);
    void devHtmlHandler(AsyncWebServerRequest *req) const;
};

#endif /* __DEV_API_HPP__ */
