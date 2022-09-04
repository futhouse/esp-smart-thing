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

class INetwork
{
public:
    virtual void setStatusLed(uint8_t gpio, bool inverted) = 0;
    virtual void startAP(const String& ssid, const String& passwd) = 0;
    virtual void connectToAP(const String& ssid, const String& passwd) = 0;
    virtual void loop() = 0;
    virtual String getIP() = 0;
    virtual String getMAC() = 0;
};

class Network: public INetwork
{
public:
    /**
     * @brief Set Wi-Fi Status Led GPIO number
     * 
     * @param gpio GPIO number
     * @param inverted Inverted LED
     */
    void setStatusLed(uint8_t gpio, bool inverted);

    /**
     * @brief Start new AP
     * 
     * @param ssid SSID of AP
     * @param passwd Password of AP
     */
    void startAP(const String& ssid, const String& passwd);

    /**
     * @brief Connecting to AP
     * 
     * @param ssid SSID of AP
     * @param passwd Password of AP
     */
    void connectToAP(const String& ssid, const String& passwd);

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
    uint8_t _statusLed;
    bool _inverted;
};

#endif /* __NETWORK_HPP__ */
