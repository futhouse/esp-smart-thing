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
#include <ESP8266WebServer.h>
#endif

#include "network.hpp"
#include "logger.hpp"
#include "gpio.hpp"
#include "flash.hpp"

#include "modules/core/sms.hpp"
#include "modules/core/telegram.hpp"

#define HTTP_CONTENT_HTML   "text/html"
#define HTTP_CONTENT_JSON   "application/json"

class IHttpSrv
{
public:
    virtual void setup() = 0;
    virtual void loop() = 0;
};

class HttpSrv: public IHttpSrv
{
public:
    HttpSrv(const std::shared_ptr<ESP8266WebServer>& syncSrv,
            const std::shared_ptr<INetwork>& net,
            const std::shared_ptr<ILogger>& log,
            const std::shared_ptr<IGpio>& gpio,
            const std::shared_ptr<IFlash>& flash,
            const std::shared_ptr<ISms>& sms,
            const std::shared_ptr<ITelegram>& tg);

    /**
     * @brief Init and start WEB server
     * 
     */
    void setup();

    /**
     * @brief Handle clients loop
     * 
     */
    void loop();

private:
    const std::shared_ptr<ESP8266WebServer> _syncSrv;
    const std::shared_ptr<INetwork> _net;
    const std::shared_ptr<ILogger> _log;
    const std::shared_ptr<IGpio> _gpio;
    const std::shared_ptr<IFlash> _flash;
    const std::shared_ptr<ISms> _sms;
    const std::shared_ptr<ITelegram> _tg;

    void infoConfHandler();
    void gpioNamesHandler();
    void edNameConfHandler();
    void wifiInfoHandler();
    void notFoundHandler();
    void infoHandler();
    void wifiHandler();
    void wifiConfHandler();
    
#ifdef SMS_NOTIFY_MOD
    void smsInfoHandler();
    void smsConfHandler();
    void smsTestHandler();
    void smsHtmlHandler();
#endif
#ifdef TELEGRAM_NOTIFY_MOD
    void tgInfoHandler();
    void tgConfHandler();
    void tgTestHandler();
    void tgHtmlHandler();
#endif
};

#endif /* __HTTP_SRV_HPP__ */
