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
#include <ArduinoJson.h>
#ifdef ESP32
#include <HttpClient.h>
#elif defined(ESP8266)
#include <ESP8266HttpClient.h>
#endif /* ESP8266 */

#include "net/html/misc.hpp"
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
    _server->on("/wifi.html", std::bind(&WifiApi::wifiHandler, this));
    _server->on(API_WIFI_INFO, std::bind(&WifiApi::wifiInfoHandler, this));
    _server->on(API_WIFI_CONF, std::bind(&WifiApi::wifiConfHandler, this));
}

void WifiApi::wifiInfoHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);

    doc["ssid"] = _net->getSSID(NETWORK_TYPE_SSID).SSID;
    doc["passwd"] = _net->getSSID(NETWORK_TYPE_SSID).Password;
    doc["ap_ssid"] = _net->getSSID(NETWORK_TYPE_AP).SSID;
    doc["ap_passwd"] = _net->getSSID(NETWORK_TYPE_AP).Password;
    doc["ap"] = BoolToStr(_net->getStartAP());
    doc["inverted"] = _net->getStatusLed().Inverted;
    doc["enabled"] = _net->getStatusLed().Enabled;
    doc["gpio"] = _gpio->pinToStr(_net->getStatusLed().Pin);

    serializeJson(doc, out);
    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void WifiApi::wifiConfHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);

    _net->setSSID(NETWORK_TYPE_SSID, {
        SSID: _server->arg("ssid"),
        Password: _server->arg("passwd")
    });
    
    _net->setSSID(NETWORK_TYPE_AP, {
        SSID: _server->arg("ap_ssid"),
        Password: _server->arg("ap_passwd")
    });

    _net->setStatusLed({
        Pin: _gpio->strToPin(_server->arg("gpio")),
        Inverted: StrToBool(_server->arg("inverted")),
        Enabled: StrToBool(_server->arg("enabled"))
    });

    _net->setStartAP(StrToBool(_server->arg("ap")));

    doc["result"] = _net->saveStates();
    _net->loadStates();
    _net->setup();

    serializeJson(doc, out);
    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void WifiApi::wifiHandler()
{
    _server->sendContent_P(headerHtml);
    _server->sendContent_P(wifiHtml);
    _server->sendContent_P(footerHtml);
}
