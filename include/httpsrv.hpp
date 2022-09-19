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

#ifndef __HTTP_SRV_HPP__
#define __HTTP_SRV_HPP__

#include <memory>

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

#include "network.hpp"
#include "logger.hpp"
#include "gpio.hpp"
#include "flash.hpp"

#define HTTP_CONTENT_HTML   "text/html"
#define HTTP_CONTENT_JSON   "application/json"

typedef enum _HttpCodes
{
    HTTP_CODE_OK = 200,
    HTTP_CODE_FORBIDDEN = 403
} HttpCodes;

class IHttpSrv
{
public:
    virtual void setup() = 0;};

class HttpSrv: public IHttpSrv
{
public:
    HttpSrv(const std::shared_ptr<AsyncWebServer>& asyncSrv,
            const std::shared_ptr<INetwork>& net,
            const std::shared_ptr<ILogger>& log,
            const std::shared_ptr<IGpio>& gpio,
            const std::shared_ptr<IFlash>& flash);

    /**
     * @brief Init and start WEB server
     * 
     */
    void setup();

private:
    const std::shared_ptr<AsyncWebServer> _asyncSrv;
    const std::shared_ptr<INetwork> _net;
    const std::shared_ptr<ILogger> _log;
    const std::shared_ptr<IGpio> _gpio;
    const std::shared_ptr<IFlash> _flash;

    void infoConfHandler(AsyncWebServerRequest *request);
    void gpioNamesHandler(AsyncWebServerRequest *request);
    void edNameConfHandler(AsyncWebServerRequest *request);
    void wifiInfoHandler(AsyncWebServerRequest *request);
    void notFoundHandler(AsyncWebServerRequest *request);
    void infoHandler(AsyncWebServerRequest *request);
    void wifiHandler(AsyncWebServerRequest *request);
    void wifiConfHandler(AsyncWebServerRequest *request);
};

#endif /* __HTTP_SRV_HPP__ */
