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

#include "net/response.hpp"
#include "net/html/wifi.hpp"
#include "net/api/v1/wifiapi.hpp"
#include "utils.hpp"

WifiApi::WifiApi(const std::shared_ptr<IGpio> &gpio,
                 const std::shared_ptr<IFlash> &flash,
                 const std::shared_ptr<INetwork> &net
                 ):
    _gpio(move(gpio)),
    _flash(move(flash)),
    _net(move(net))
{
}

void WifiApi::registerHandlers(const std::shared_ptr<EspServer> &server)
{
    _server = server;
    server->on(API_WIFI_CONF, HTTP_POST, std::bind(&WifiApi::wifiConfHandler, this, std::placeholders::_1));
    server->on(API_WIFI_INFO, HTTP_GET, std::bind(&WifiApi::wifiInfoHandler, this, std::placeholders::_1));
    server->on("/wifi.html", HTTP_GET, std::bind(&WifiApi::wifiHandler, this, std::placeholders::_1));
}

void WifiApi::wifiInfoHandler(AsyncWebServerRequest *req)
{
    NetResponse resp(req);

    resp.setArg("ssid", _net->getSSID(NETWORK_TYPE_SSID).SSID);
    resp.setArg("passwd", _net->getSSID(NETWORK_TYPE_SSID).Password);
    resp.setArg("ap_ssid", _net->getSSID(NETWORK_TYPE_AP).SSID);
    resp.setArg("ap_passwd", _net->getSSID(NETWORK_TYPE_AP).Password);
    resp.setArg("ap", _net->getStartAP());
    resp.setArg("inverted", _net->getStatusLed().Inverted);
    resp.setArg("enabled", _net->getStatusLed().Enabled);
    resp.setArg("gpio", _gpio->pinToStr(_net->getStatusLed().Pin));

    resp.sendJson();
}

void WifiApi::wifiConfHandler(AsyncWebServerRequest *req)
{
    NetResponse resp(req);

    _net->setSSID(NETWORK_TYPE_SSID, {
        SSID: req->arg("ssid"),
        Password: req->arg("passwd")
    });
    
    _net->setSSID(NETWORK_TYPE_AP, {
        SSID: req->arg("ap_ssid"),
        Password: req->arg("ap_passwd")
    });

    _net->setStatusLed({
        Pin: _gpio->strToPin(req->arg("gpio")),
        Inverted: StrToBool(req->arg("inverted")),
        Enabled: StrToBool(req->arg("enabled"))
    });

    _net->setStartAP(StrToBool(req->arg("ap")));

    resp.setArg("result", _net->saveStates());
    resp.sendJson();

    ESP.reset();
}

void WifiApi::wifiHandler(AsyncWebServerRequest *req)
{
    NetResponse resp(req);
    resp.sendHtml(wifiHtml);
}
