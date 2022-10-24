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

#include <functional>

#include "net/api/v1/gpioapi.hpp"
#include "net/html/info.hpp"
#include "net/response.hpp"

GpioApi::GpioApi(const std::shared_ptr<IGpio> &gpio):
    _gpio(move(gpio))
{
}

void GpioApi::registerHandlers(const std::shared_ptr<EspServer> &server)
{
    _server = server;
    _server->on(API_GPIO_INFO, HTTP_GET, std::bind(&GpioApi::gpioInfoHandler, this, std::placeholders::_1));
}

void GpioApi::gpioInfoHandler(AsyncWebServerRequest *req)
{
    std::vector<String> gpios;
    NetResponse resp(req);

    _gpio->getGpioNames(gpios);
    for (size_t i = 0; i < gpios.size(); i++) {
        resp.setArg(i, gpios[i]);
    }

    resp.sendJson();
}
