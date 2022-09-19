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
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include "network.hpp"
#include "custom/configs.hpp"

Network::Network(const std::shared_ptr<DNSServer>& dns,
                 const std::shared_ptr<IGpio>& gpio):
    _dns(move(dns)),
    _gpio(move(gpio))
{
}

void Network::setStatusLed(bool enabled, const GpioPin& pin, bool inverted)
{
    _statusLed = std::move(pin);
    _inverted = inverted;
    _ledEnabled = enabled;

    _gpio->setPinMode(_statusLed, GPIO_OUTPUT);

    if (_inverted)
        _gpio->setPinState(_statusLed, GPIO_HIGH);
    else
        _gpio->setPinState(_statusLed, GPIO_LOW);
}

void Network::startAP(const String& ssid)
{
    _startAP = true;
    WiFi.mode(WIFI_AP);
    if (WiFi.softAP(ssid))
    {
        if (_ledEnabled)
        {
            if (_inverted)
                _gpio->setPinState(_statusLed, GPIO_LOW);
            else
                _gpio->setPinState(_statusLed, GPIO_HIGH);
        }
    }
    else
    {
        if (_ledEnabled)
        {
            if (_inverted)
                _gpio->setPinState(_statusLed, GPIO_LOW);
            else
                _gpio->setPinState(_statusLed, GPIO_HIGH);
        }
    }
    _dns->start(CONFIG_DNS_SERVER_PORT, "*", WiFi.softAPIP());
}

void Network::connectToAP(const String& ssid, const String& passwd)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, passwd);
}

void Network::loop()
{
    _dns->processNextRequest();
    if (!_startAP && _ledEnabled)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            if (_inverted)
                _gpio->setPinState(_statusLed, GPIO_LOW);
            else
                _gpio->setPinState(_statusLed, GPIO_HIGH);
        }
        else
        {
            if (_inverted)
                _gpio->setPinState(_statusLed, GPIO_HIGH);
            else
                _gpio->setPinState(_statusLed, GPIO_LOW);
        }
    }
}

String Network::getIP()
{
    if (_startAP)
        return WiFi.softAPIP().toString();
    else
        return WiFi.localIP().toString();
}

String Network::getMAC()
{
    return WiFi.macAddress();
}
