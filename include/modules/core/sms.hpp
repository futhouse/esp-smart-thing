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

#include <Arduino.h>

class ISms
{
#ifdef SMS_NOTIFY_MOD
public:
    virtual void setCreds(const String &token, const String &phone) = 0;
    virtual const String &getPhone() = 0;
    virtual const String &getToken() = 0;
    virtual bool sendMsg(const String &msg) = 0;
#endif /* SMS_NOTIFY_MOD */
};

class Sms : public ISms
{
#ifdef SMS_NOTIFY_MOD
public:
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
    const String &getPhone();

    /**
     * @brief Get the Token string
     * 
     * @return const String& 
     */
    const String &getToken();

    /**
     * @brief Send SMS message
     * 
     * @param msg Text
     * @return true 
     * @return false 
     */
    bool sendMsg(const String &msg);

private:
    String _phone;
    String _token;
#endif /* SMS_NOTIFY_MOD */
};

#endif /* __SMS_HPP__ */
