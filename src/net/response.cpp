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

#include "net/response.hpp"
#include "net/espsrv.hpp"

#ifdef ESP32
#include <HttpClient.h>
#elif defined (ESP8266)
#include <ESP8266HttpClient.h>
#endif

NetResponse::NetResponse(AsyncWebServerRequest *req, size_t len)
{
    _json = std::make_shared<DynamicJsonDocument>(len);
    _req = req;
}

void NetResponse::setArg(const String& name, const String& value)
{
    (*_json)[name] = value;
}

void NetResponse::setArg(const String& name, const bool value)
{
    (*_json)[name] = value;
}

void NetResponse::setArg(size_t id, const String& value)
{
    (*_json)[id] = value;
}

void NetResponse::setArg(size_t id, const String& name, const bool value)
{
    (*_json)[id][name] = value;
}

void NetResponse::setArg(const String& field, size_t id, const String& name, const bool value)
{
    (*_json)[field][id][name] = value;
}

void NetResponse::setArg(const String& field, size_t id, const String& name, const String& value)
{
    (*_json)[field][id][name] = value;
}

void NetResponse::setArg(const String& field, size_t id, const String& value)
{
    (*_json)[field][id] = value;
}

void NetResponse::setArg(const String& field, size_t id, const String& name, const unsigned value)
{
    (*_json)[field][id][name] = value;
}

void NetResponse::sendJson()
{
    String out;
    serializeJson(*_json, out);
    _req->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out);
}

void NetResponse::sendHtml(PGM_P content)
{
    _req->send_P(HTTP_CODE_OK, HTTP_CONTENT_HTML, content);
}
