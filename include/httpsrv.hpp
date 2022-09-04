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
#include <ESP8266WebServer.h>

#include "network.hpp"

#define HTTP_CONTENT_HTML   "text/html"
#define HTTP_CONTENT_JSON   "application/json"

typedef enum _HttpCodes
{
    HTTP_CODE_OK = 200,
    HTTP_CODE_FORBIDDEN = 403
} HttpCodes;

class IHttpServer
{
public:
    virtual void start(uint32_t port) = 0;
    virtual void loop() = 0;
    virtual void registerHandler(const String &path, std::function<void ()> handler) = 0;
};

class HttpServer: public IHttpServer
{
public:
    HttpServer(const std::shared_ptr<ESP8266WebServer>& espServer,
                const std::shared_ptr<INetwork>& net);

    /**
     * @brief Init and start WEB server
     * 
     * @param port Web server port
     */
    void start(uint32_t port);

    /**
     * @brief Main Web Server cycle
     * 
     */
    void loop();

    /**
     * @brief Add new HTTP handler to server
     * 
     * @param path Path to handler
     * @param handler Handler function
     */
    void registerHandler(const String &path, std::function<void ()> handler);

private:
    const std::shared_ptr<ESP8266WebServer> _espServer;
    const std::shared_ptr<INetwork> _net;

    void infoMgmtHandler();

    void notFoundHandler();
    void infoHandler();
    void wifiHandler();
};

#endif /* __HTTP_SRV_HPP__ */
