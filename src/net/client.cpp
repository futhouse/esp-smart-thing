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

#ifdef ESP32
#include <HTTPClient.h>
#include <WiFi.h>
#elif defined (ESP8266)
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecureBearSSL.h>

#include "net/client.hpp"

NetClient::NetClient(NetClientType typ, const String& addr)
{
    switch( typ )
    {
        case NET_CLIENT_HTTP:
            _addr = "http://";
            break;

        case NET_CLIENT_HTTPS:
            _addr = "https://";
            break;
    }

    _addr += addr;
}

bool NetClient::getRequest(const String& req)
{
    return request(req);
}

bool NetClient::getRequest(NetRequest& req)
{
    return request(req.getResult());
}

String& NetClient::getPayload()
{
    return _payload;
}

bool NetClient::request(const String& req)
{
    HTTPClient https;
    BearSSL::WiFiClientSecure client;

    client.setInsecure();

    if (https.begin(client, _addr + req)) {
        int code = https.GET();
        if ((code == HTTP_CODE_OK) || (code == HTTPC_ERROR_READ_TIMEOUT)) {
            _payload = client.readString();
            https.end();
            return true;
        }
        https.end();
    }

    return false;
}
