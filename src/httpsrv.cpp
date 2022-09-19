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

#include <ArduinoJson.h>
#include <functional>

#include "httpsrv.hpp"
#include "html/404.hpp"
#include "html/misc.hpp"
#include "html/wifi.hpp"
#include "html/info.hpp"

HttpSrv::HttpSrv(const std::shared_ptr<AsyncWebServer>& asyncSrv,
                        const std::shared_ptr<INetwork>& net,
                        const std::shared_ptr<ILogger>& log,
                        const std::shared_ptr<IGpio>& gpio,
                        const std::shared_ptr<IFlash>& flash):
    _asyncSrv(move(asyncSrv)),
    _net(move(net)),
    _log(move(log)),
    _gpio(move(gpio)),
    _flash(move(flash))
{
}

void HttpSrv::setup()
{
    _log->info("HTTP", "Starting WEB server");
    _asyncSrv->begin();

    _asyncSrv->on("/info/index", std::bind(&HttpSrv::infoConfHandler, this, std::placeholders::_1));
    _asyncSrv->on("/info/gpio_names", std::bind(&HttpSrv::gpioNamesHandler, this, std::placeholders::_1));
    _asyncSrv->on("/info/wifi", std::bind(&HttpSrv::wifiInfoHandler, this, std::placeholders::_1));

    _asyncSrv->on("/conf/edname", std::bind(&HttpSrv::edNameConfHandler, this, std::placeholders::_1));
    _asyncSrv->on("/conf/wifi", std::bind(&HttpSrv::wifiConfHandler, this, std::placeholders::_1));

    _asyncSrv->onNotFound(std::bind(&HttpSrv::notFoundHandler, this, std::placeholders::_1));
    _asyncSrv->on("/", std::bind(&HttpSrv::infoHandler, this, std::placeholders::_1));
    _asyncSrv->on("/wifi.html", std::bind(&HttpSrv::wifiHandler, this, std::placeholders::_1));
}

/**
 * @brief JSON managment handlers
 * 
 */

void HttpSrv::gpioNamesHandler(AsyncWebServerRequest *request)
{
    String names;

    _gpio->getGpioNames(names);

    request->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, "[" + names + "]"); 
}

void HttpSrv::infoConfHandler(AsyncWebServerRequest *request)
{
    String out = "";
    DynamicJsonDocument doc(1024);
    auto cfg = _flash->getConfigs();

    doc["name"] = String(cfg->DevName);
    doc["ip"] = _net->getIP();
    doc["mac"] = _net->getMAC();
    serializeJson(doc, out);

    request->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void HttpSrv::wifiInfoHandler(AsyncWebServerRequest *request)
{
    String out = "";
    DynamicJsonDocument doc(1024);
    auto cfg = _flash->getConfigs();

    doc["ssid"] = String(cfg->NetCfg.SSID);
    doc["passwd"] = String(cfg->NetCfg.Password);
    doc["ap"] = (!cfg->NetCfg.IsConnectAP) ? true : false;
    doc["inverted"] = cfg->NetCfg.IsInverted;
    doc["enabled"] = cfg->NetCfg.IsLedEnabled;

    auto led = cfg->NetCfg.StatusLED;
    auto ledPin = GpioPin
    {
        Type: static_cast<GpioType>(led.Type),
        Addr: led.Addr,
        Pin: led.Pin
    };

    doc["gpio"] = _gpio->PinToStr(ledPin);
    serializeJson(doc, out);

    request->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void HttpSrv::wifiConfHandler(AsyncWebServerRequest *request)
{
    String out = "";
    DynamicJsonDocument doc(1024);
    GpioPin ledPin;
    auto cfg = _flash->getConfigs();

    strncpy(cfg->NetCfg.SSID, request->arg("ssid").c_str(), 19);
    strncpy(cfg->NetCfg.Password, request->arg("passwd").c_str(), 19);
    if (request->arg("ap") == "true")
        cfg->NetCfg.IsConnectAP = false;
    else
        cfg->NetCfg.IsConnectAP = true;
    if (request->arg("inverted") == "true")
        cfg->NetCfg.IsInverted = true;
    else
        cfg->NetCfg.IsInverted = false;
    if (request->arg("enabled") == "true")
        cfg->NetCfg.IsLedEnabled = true;
    else
        cfg->NetCfg.IsLedEnabled = false;

    _gpio->StrToPin(request->arg("gpio"), ledPin);
    cfg->NetCfg.StatusLED.Addr = ledPin.Addr;
    cfg->NetCfg.StatusLED.Type = ledPin.Type;
    cfg->NetCfg.StatusLED.Pin = ledPin.Pin;

    _flash->saveData();

    doc["result"] = true;
    serializeJson(doc, out);
    request->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void HttpSrv::edNameConfHandler(AsyncWebServerRequest *request)
{
    auto cfg = _flash->getConfigs();
    
    strncpy(cfg->DevName, request->arg("name").c_str(), 19);
    _flash->saveData();

    request->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, ""); 
}

/**
 * @brief HTML handlers
 * 
 */

void HttpSrv::notFoundHandler(AsyncWebServerRequest *request)
{
    request->send(HTTP_CODE_OK, HTTP_CONTENT_HTML, notFoundHtml); 
}

void HttpSrv::infoHandler(AsyncWebServerRequest *request)
{
    request->send(HTTP_CODE_OK, HTTP_CONTENT_HTML,
        String(headerHtml) + String(infoHtml) + String(footerHtml));
}

void HttpSrv::wifiHandler(AsyncWebServerRequest *request)
{
    request->send(HTTP_CODE_OK, HTTP_CONTENT_HTML,
        String(headerHtml) + String(wifiHtml) + String(footerHtml));
}
