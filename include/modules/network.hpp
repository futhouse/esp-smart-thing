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
#include "flash.hpp"
#include "logger.hpp"
#include "modules/module.hpp"

#define NET_CONNECT_RETRIES 15

typedef enum
{
    NETWORK_TYPE_SSID,
    NETWORK_TYPE_AP,
    NETWORK_TYPE_MAX
} NetowkSSIDType;

typedef struct _NetworkStatusLed
{
    GpioPin Pin;
    bool    Inverted;
    bool    Enabled;
} NetworkStatusLed;

typedef struct _NetworkSSID
{
    String SSID;
    String Password;
} NetworkSSID;

class INetwork : public Module
{
public:
    virtual void setStatusLed(const NetworkStatusLed& led) = 0;
    virtual void setup() = 0;
    virtual void loop() = 0;
    virtual void setSSID(NetowkSSIDType type, const NetworkSSID& ssid) = 0;
    virtual String getIP() = 0;
    virtual String getMAC() = 0;
    virtual NetworkSSID& getSSID(NetowkSSIDType type) = 0;
    virtual void setStartAP(bool status) = 0;
    virtual bool getStartAP() = 0;
    virtual NetworkStatusLed& getStatusLed() = 0;
};

class Network: public INetwork
{
public:
    Network(const std::shared_ptr<DNSServer>& dns,
            const std::shared_ptr<IGpio>& gpio,
            const std::shared_ptr<IFlash>& flash,
            const std::shared_ptr<ILogger>& log);

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

    /**
     * @brief Loading states from EEPROM
     * 
     */
    void loadStates();

    /**
     * @brief Saving states to EEPROM
     * 
     * @return true 
     * @return false 
     */
    bool saveStates();

    /**
     * @brief Start Wi-Fi
     * 
     */
    void setup();

    /**
     * @brief Get SSID name
     * 
     * @return NetworkSSID&
     */
    NetworkSSID& getSSID(NetowkSSIDType type);

    /**
     * @brief Set remote SSID name
     * 
     * @param ssid 
     */
    void setSSID(NetowkSSIDType type, const NetworkSSID& ssid);

    /**
     * @brief Set the Start AP status
     * 
     * @param status 
     */
    void setStartAP(bool status);

    /**
     * @brief Get the Start AP status
     * 
     * @return true 
     * @return false 
     */
    bool getStartAP();

    /**
     * @brief Set the Status Led object
     * 
     * @param led 
     */
    void setStatusLed(const NetworkStatusLed& led);

    NetworkStatusLed& getStatusLed();

private:
    const std::shared_ptr<DNSServer> _dns;
    const std::shared_ptr<IGpio> _gpio;
    const std::shared_ptr<IFlash> _flash;
    const std::shared_ptr<ILogger> _log;

    bool _startAP = false;
    NetworkStatusLed _statusLed;
    NetworkSSID _ssid[NETWORK_TYPE_MAX];

    /**
     * @brief Start new AP
     * 
     * @param ssid SSID of AP
     * @param passwd AP passwd
     */
    void startAP(const String& ssid, const String& passwd);

    /**
     * @brief Connecting to AP
     * 
     * @param ssid SSID of AP
     * @param passwd Password of AP
     */
    bool connectToAP(const String& ssid, const String& passwd);
};

#endif /* __NETWORK_HPP__ */
