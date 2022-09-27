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

#include "modules/core/telegram.hpp"

#ifdef TELEGRAM_NOTIFY_MOD

#include <UrlEncode.h>
#ifdef ESP32
#include <HTTPClient.h>
#include <WiFi.h>
#elif defined (ESP8266)
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecureBearSSL.h>

void Telegram::setCreds(const String &bot, const String &chat)
{
    _botToken = bot;
    _chatID = chat;
}

bool Telegram::sendMsg(const String &msg)
{
    HTTPClient https;
    BearSSL::WiFiClientSecure client;

    client.setInsecure();

    if (https.begin(client,
        "https://api.telegram.org/bot" + _botToken +
        "/sendMessage?chat_id=" + _chatID +
        "&text=" + urlEncode(msg)))
    {
        if (https.GET() == HTTP_CODE_OK) {
            https.end();
            return true;
        }

        https.end();
    }

    return false;
}

#endif /* TELEGRAM_NOTIFY_MOD */
