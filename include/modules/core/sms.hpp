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

#ifndef __SMS_HPP__
#define __SMS_HPP__

#include "custom/modules.hpp"
#include "modules/module.hpp"
#include "flash.hpp"

#include <Arduino.h>
#include <functional>
#include <memory>

class ISms : public Module
{
#ifdef SMS_NOTIFY_MOD
public:
    virtual void setCreds(const String &token, const String &phone) = 0;
    virtual const String &getPhone() const = 0;
    virtual const String &getToken() const = 0;
    virtual bool sendMsg(const String &msg) = 0;
    virtual void setServer(const String &server) = 0;
    virtual const String &getServer() const = 0;
#endif /* SMS_NOTIFY_MOD */
};

class Sms : public ISms
{
public:
    Sms(const std::shared_ptr<IFlash>& flash);

    /**
     * @brief Saving states to EEPROM
     * 
     * @return true 
     * @return false 
     */
    bool saveStates();

    /**
     * @brief Loading states from EEPROM
     * 
     */
    void loadStates();

#ifdef SMS_NOTIFY_MOD
    /**
     * @brief Set the Creds: phone and token
     * 
     * @param token 
     * @param phone 
     */
    void setCreds(const String &token, const String &phone);

    /**
     * @brief Get the Phone string
     * 
     * @return const String& 
     */
    const String &getPhone() const;

    /**
     * @brief Get the Token string
     * 
     * @return const String& 
     */
    const String &getToken() const;

    /**
     * @brief Send SMS message
     * 
     * @param msg Text
     * @return true 
     * @return false 
     */
    bool sendMsg(const String &msg);

    /**
     * @brief Set SMS proxy server
     * 
     * @param server IP
     */
    void setServer(const String &server);

    /**
     * @brief Get SMS proxy server
     * 
     * @return const String& 
     */
    const String &getServer() const;
#endif /* SMS_NOTIFY_MOD */

private:
    const std::shared_ptr<IFlash> _flash;

#ifdef SMS_NOTIFY_MOD
    String _server;
    String _phone;
    String _token;
#endif /* SMS_NOTIFY_MOD */
};

#endif /* __SMS_HPP__ */
