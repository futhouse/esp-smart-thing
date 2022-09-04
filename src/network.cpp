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


#include <ESP8266WiFi.h>

#include "network.hpp"

void Network::setStatusLed(uint8_t gpio, bool inverted)
{
    _statusLed = gpio;
    _inverted = inverted;

    pinMode(_statusLed, OUTPUT);

    if (_inverted)
        digitalWrite(_statusLed, HIGH);
    else
        digitalWrite(_statusLed, LOW);
}

void Network::startAP(const String& ssid, const String& passwd)
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, passwd);
}

void Network::connectToAP(const String& ssid, const String& passwd)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, passwd);
}

void Network::loop()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        if (_inverted)
            digitalWrite(_statusLed, LOW);
        else
            digitalWrite(_statusLed, HIGH);
    }
    else
    {
        if (_inverted)
            digitalWrite(_statusLed, HIGH);
        else
            digitalWrite(_statusLed, LOW);
    }
}

String Network::getIP()
{
    return WiFi.localIP().toString();
}

String Network::getMAC()
{
    return WiFi.macAddress();
}
