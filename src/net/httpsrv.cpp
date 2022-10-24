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

#ifdef ESP32
#include <HttpClient.h>
#elif defined (ESP8266)
#include <ESP8266HttpClient.h>
#endif

#include "net/httpsrv.hpp"
#include "net/html/404.hpp"
#include "net/html/wifi.hpp"
#include "net/html/info.hpp"

#include "net/html/modules/core/sms.hpp"
#include "net/html/modules/core/telegram.hpp"

HttpSrv::HttpSrv(const std::shared_ptr<EspServer>& syncSrv,
                 const std::shared_ptr<ILogger>& logger,
                 const std::shared_ptr<IApi>& apiGpio,
                 const std::shared_ptr<IApi>& apiDev,
                 const std::shared_ptr<IApi>& apiWifi,
                 const std::shared_ptr<IApi>& apiSms,
                 const std::shared_ptr<IApi>& apiTg,
                 const std::shared_ptr<IApi>& apiSec
                ):
    _syncSrv(move(syncSrv)),
    _log(logger),
    _apiGpio(move(apiGpio)),
    _apiDev(move(apiDev)),
    _apiWifi(move(apiWifi)),
    _apiSms(move(apiSms)),
    _apiTg(move(apiTg)),
    _apiSec(move(apiSec))
{
}

void HttpSrv::setup()
{
    _log->info("HTTP", "Starting WEB server");
    
    _syncSrv->onNotFound(std::bind(&HttpSrv::notFoundHandler, this, std::placeholders::_1));
    
    _apiDev->registerHandlers(_syncSrv);
    _apiGpio->registerHandlers(_syncSrv);
    _apiWifi->registerHandlers(_syncSrv);
    _apiSms->registerHandlers(_syncSrv);
    _apiTg->registerHandlers(_syncSrv);
    _apiSec->registerHandlers(_syncSrv);

    _syncSrv->begin();
}

void HttpSrv::loop()
{
}

void HttpSrv::notFoundHandler(AsyncWebServerRequest *req)
{
    req->send(HTTP_CODE_OK, HTTP_CONTENT_HTML, notFoundHtml); 
}

