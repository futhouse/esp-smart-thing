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

#include "modules/network.hpp"
#include "custom/configs.hpp"

Network::Network(const std::shared_ptr<DNSServer>& dns,
                 const std::shared_ptr<IGpio>& gpio,
                 const std::shared_ptr<IFlash>& flash,
                 const std::shared_ptr<ILogger>& log
                ):
    _dns(move(dns)),
    _gpio(move(gpio)),
    _flash(move(flash)),
    _log(move(log))
{
}

void Network::setStatusLed(const NetworkStatusLed& led)
{
    _statusLed = led;
}

void Network::startAP(const String& ssid, const String& passwd)
{
    WiFi.mode(WIFI_AP);

    if (WiFi.softAP(ssid, passwd))
    {
        if (_statusLed.Enabled)
        {
            if (_statusLed.Inverted)
                _gpio->setPinState(_statusLed.Pin, GPIO_LOW);
            else
                _gpio->setPinState(_statusLed.Pin, GPIO_HIGH);
        }
    }
    else
    {
        if (_statusLed.Enabled)
        {
            if (_statusLed.Inverted)
                _gpio->setPinState(_statusLed.Pin, GPIO_LOW);
            else
                _gpio->setPinState(_statusLed.Pin, GPIO_HIGH);
        }
    }

    _dns->start(CONFIG_DNS_SERVER_PORT, "*", WiFi.softAPIP());
}

bool Network::connectToAP(const String& ssid, const String& passwd)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, passwd);

    for (uint8_t i = 0; i < NET_CONNECT_RETRIES; i++) {
        delay(1000);
        if (WiFi.status() == WL_CONNECTED)
            return true;
    }

    return false;
}

void Network::loop()
{
    if (_startAP)
        _dns->processNextRequest();

    if (!_startAP && _statusLed.Enabled)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            if (_statusLed.Inverted)
                _gpio->setPinState(_statusLed.Pin, GPIO_LOW);
            else
                _gpio->setPinState(_statusLed.Pin, GPIO_HIGH);
        }
        else
        {
            if (_statusLed.Inverted)
                _gpio->setPinState(_statusLed.Pin, GPIO_HIGH);
            else
                _gpio->setPinState(_statusLed.Pin, GPIO_LOW);
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

void Network::loadStates()
{
    _log->info("NET", "Loading Wi-Fi configs");

    auto cfg = _flash->getConfigs();
    const auto& led = cfg->NetCfg.StatusLED;

    setStatusLed({
        Pin: {
            Type: static_cast<GpioType>(led.Type),
            Addr: led.Addr,
            Pin: led.Pin
        },
        Inverted: cfg->NetCfg.IsInverted,
        Enabled: cfg->NetCfg.IsLedEnabled
    });

    _startAP = cfg->NetCfg.IsStartAP;

    _ssid[NETWORK_TYPE_AP].SSID = String(cfg->NetCfg.ApSSID);
    _ssid[NETWORK_TYPE_AP].Password = String(cfg->NetCfg.ApPassword);
    _ssid[NETWORK_TYPE_SSID].SSID = String(cfg->NetCfg.SSID);
    _ssid[NETWORK_TYPE_SSID].Password = String(cfg->NetCfg.Password);
}

bool Network::saveStates()
{
    _log->info("NET", "Saving Wi-Fi configs");
    auto cfg = _flash->getConfigs();

    strncpy(cfg->NetCfg.SSID, _ssid[NETWORK_TYPE_SSID].SSID.c_str(), CONFIG_STR_LEN);
    strncpy(cfg->NetCfg.Password, _ssid[NETWORK_TYPE_SSID].Password.c_str(), CONFIG_STR_LEN);
    strncpy(cfg->NetCfg.ApSSID, _ssid[NETWORK_TYPE_AP].SSID.c_str(), CONFIG_STR_LEN);
    strncpy(cfg->NetCfg.ApPassword, _ssid[NETWORK_TYPE_AP].Password.c_str(), CONFIG_STR_LEN);

    cfg->NetCfg.IsStartAP = _startAP;
    cfg->NetCfg.IsInverted = _statusLed.Inverted;
    cfg->NetCfg.IsLedEnabled = _statusLed.Enabled;
    cfg->NetCfg.StatusLED.Addr = _statusLed.Pin.Addr;
    cfg->NetCfg.StatusLED.Type = _statusLed.Pin.Type;
    cfg->NetCfg.StatusLED.Pin = _statusLed.Pin.Pin;

    return _flash->saveData();
}

void Network::setup()
{
    if (_statusLed.Enabled)
    {
        _gpio->setPinMode(_statusLed.Pin, GPIO_OUTPUT);

        if (_statusLed.Inverted)
            _gpio->setPinState(_statusLed.Pin, GPIO_HIGH);
        else
            _gpio->setPinState(_statusLed.Pin, GPIO_LOW);
    }

    if (!_startAP)
    {
        _log->info("EST", "Connecting to AP: \"" + _ssid[NETWORK_TYPE_SSID].SSID + "\"");
        if (connectToAP(_ssid[NETWORK_TYPE_SSID].SSID, _ssid[NETWORK_TYPE_SSID].Password)) {
            _log->info("EST", "Connected successful. IP: " + getIP());
        } else {
            _log->error("EST", "Failed to connect to AP.");
        }
    }
    else
    {
        _log->info("EST", "Starting new AP. SSID: \"" + _ssid[NETWORK_TYPE_AP].SSID + "\"");
        startAP(_ssid[NETWORK_TYPE_AP].SSID, _ssid[NETWORK_TYPE_AP].Password);
    }
}

NetworkSSID& Network::getSSID(NetowkSSIDType type)
{
    return _ssid[type];
}

void Network::setSSID(NetowkSSIDType type, const NetworkSSID& ssid)
{
    _ssid[type] = ssid;
}

void Network::setStartAP(bool status)
{
    _startAP = status;
}

bool Network::getStartAP()
{
    return _startAP;
}

NetworkStatusLed& Network::getStatusLed()
{
    return _statusLed;
}
