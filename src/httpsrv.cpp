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

#include "httpsrv.hpp"
#include "html/404.hpp"
#include "html/misc.hpp"
#include "html/wifi.hpp"
#include "html/info.hpp"

HttpServer::HttpServer(const std::shared_ptr<ESP8266WebServer>& espServer,
                        const std::shared_ptr<INetwork>& net):
    _espServer(move(espServer)),
    _net(move(net))
{
}

void HttpServer::start(uint32_t port)
{
    _espServer->begin(port);
    _espServer->enableCORS(true);

    _espServer->on("/mgmt/info", std::bind(&HttpServer::infoMgmtHandler, this));

    _espServer->onNotFound(std::bind(&HttpServer::notFoundHandler, this));
    _espServer->on("/", std::bind(&HttpServer::infoHandler, this));
    _espServer->on("/wifi", std::bind(&HttpServer::wifiHandler, this));
}

void HttpServer::registerHandler(const String &path, std::function<void ()> handler)
{
    _espServer->on(path, handler);
}

void HttpServer::loop() 
{
    _espServer->handleClient();
}

/**
 * @brief JSON managment handlers
 * 
 */

void HttpServer::infoMgmtHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);

    doc["ip"] = _net->getIP();
    doc["mac"] = _net->getMAC();
    serializeJson(doc, out);

    _espServer->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

/**
 * @brief HTML handlers
 * 
 */

void HttpServer::notFoundHandler()
{
    _espServer->send(HTTP_CODE_OK, HTTP_CONTENT_HTML, notFoundHtml); 
}

void HttpServer::infoHandler()
{
    _espServer->send(HTTP_CODE_OK, HTTP_CONTENT_HTML, headerHtml + infoHtml + infoScript + footerHtml);
}

void HttpServer::wifiHandler()
{
    _espServer->send(HTTP_CODE_OK, HTTP_CONTENT_HTML, headerHtml + wifiHtml + wifiScript + footerHtml);
}
