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

#include "net/api/v1/devapi.hpp"
#include "net/html/info.hpp"
#include "net/api/api.hpp"
#include "net/response.hpp"

DeviceApi::DeviceApi(const std::shared_ptr<IFlash>& flash,
                     const std::shared_ptr<INetwork>& net):
    _flash(move(flash)),
    _net(move(net))
{
}

void DeviceApi::registerHandlers(const std::shared_ptr<EspServer> &server)
{
    _server = server;
    _server->on(API_DEV_CONF, HTTP_POST, std::bind(&DeviceApi::devConfHandler, this, std::placeholders::_1));
    _server->on(API_DEV_INFO, HTTP_GET, std::bind(&DeviceApi::devInfoHandler, this, std::placeholders::_1));
    _server->on("/", HTTP_GET, std::bind(&DeviceApi::devHtmlHandler, this, std::placeholders::_1));
}

void DeviceApi::devHtmlHandler(AsyncWebServerRequest *req) const
{
    NetResponse resp(req);
    resp.sendHtml(infoHtml);
}

void DeviceApi::devInfoHandler(AsyncWebServerRequest *req) const
{
    NetResponse resp(req);
    auto cfg = _flash->getConfigs();

    resp.setArg("name", String(cfg->DevName));
    resp.setArg("ip", _net->getIP());
    resp.setArg("mac", _net->getMAC());

    resp.sendJson();
}

void DeviceApi::devConfHandler(AsyncWebServerRequest *req)
{
    NetResponse resp(req);
    auto cfg = _flash->getConfigs();

    strncpy(cfg->DevName, req->arg("name").c_str(), CONFIG_STR_LEN);
    cfg->DevName[CONFIG_STR_LEN - 1] = '\0';

    resp.setArg("result", _flash->saveData());
    resp.sendJson();
}
