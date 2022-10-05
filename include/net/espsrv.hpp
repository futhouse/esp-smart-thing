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

#ifndef __ESP_SRV_HPP__
#define __ESP_SRV_HPP__

#define CONFIG_WEB_SERVER_PORT  80

#define HTTP_CONTENT_HTML   "text/html"
#define HTTP_CONTENT_JSON   "application/json"

#ifdef ESP32
#include <WebServer.h>

class EspServer: public WebServer
{
public:
    EspServer() : WebServer(CONFIG_WEB_SERVER_PORT) { }
};

#elif defined(ESP8266)
#include <ESP8266WebServer.h>

class EspServer: public ESP8266WebServer
{
public:
    EspServer() : ESP8266WebServer(CONFIG_WEB_SERVER_PORT) { }
};

#endif /* ESP8266 */

#endif /* __ESP_SRV_HPP__ */
