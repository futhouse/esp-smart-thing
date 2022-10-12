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

#ifndef __NETWORK_HPP__
#define __NETWORK_HPP__

#include <Arduino.h>
#include <DNSServer.h>
#include <memory>

#include "gpio.hpp"

#define NET_CONNECT_RETRIES 15

class INetwork
{
public:
    virtual void setStatusLed(bool enabled, const GpioPin& pin, bool inverted) = 0;
    virtual void startAP(const String& ssid) = 0;
    virtual bool connectToAP(const String& ssid, const String& passwd) = 0;
    virtual void loop() = 0;
    virtual String getIP() = 0;
    virtual String getMAC() = 0;
};

class Network: public INetwork
{
public:
    Network(const std::shared_ptr<DNSServer>& dns,
            const std::shared_ptr<IGpio>& gpio);

    /**
     * @brief Set Wi-Fi Status Led GPIO number
     * 
     * @param enabled Status led enabled status
     * @param pin GPIO number
     * @param inverted Inverted LED
     */
    void setStatusLed(bool enabled, const GpioPin& pin, bool inverted);

    /**
     * @brief Start new AP
     * 
     * @param ssid SSID of AP
     */
    void startAP(const String& ssid);

    /**
     * @brief Connecting to AP
     * 
     * @param ssid SSID of AP
     * @param passwd Password of AP
     */
    bool connectToAP(const String& ssid, const String& passwd);

    /**
     * @brief Wi-Fi loop
     * 
     */
    void loop();

    /**
     * @brief Get current IP address of device
     * 
     * @return IP address
     */
    String getIP();

    /**
     * @brief Device MAC address
     * 
     * @return MAC address
     */
    String getMAC();

private:
    const std::shared_ptr<DNSServer> _dns;
    const std::shared_ptr<IGpio> _gpio;

    GpioPin _statusLed;
    bool _inverted = false;
    bool _ledEnabled = false;
    bool _startAP = false;
};

#endif /* __NETWORK_HPP__ */
