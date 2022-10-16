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

#ifndef __API_HPP__
#define __API_HPP__

#include "net/espsrv.hpp"
#include "custom/modules.hpp"

#define API_DEV_INFO    "/api/v1/device/info"
#define API_DEV_CONF    "/api/v1/device/conf"

#define API_GPIO_INFO   "/api/v1/gpio/info"

#define API_WIFI_INFO   "/api/v1/wifi/info"
#define API_WIFI_CONF   "/api/v1/wifi/conf"

#ifdef SMS_NOTIFY_MOD
#define API_SMS_INFO    "/api/v1/sms/info"
#define API_SMS_CONF    "/api/v1/sms/conf"
#define API_SMS_TEST    "/api/v1/sms/test"
#endif

#ifdef TELEGRAM_NOTIFY_MOD
#define API_TELEGRAM_INFO   "/api/v1/telegram/info"
#define API_TELEGRAM_CONF   "/api/v1/telegram/conf"
#define API_TELEGRAM_TEST   "/api/v1/telegram/test"
#endif

#ifdef SECURE_MOD
#define API_SECURE_INFO         "/api/v1/secure/info"
#define API_SECURE_TYPES        "/api/v1/secure/types"
#define API_SECURE_CONF         "/api/v1/secure/conf"
#define API_SECURE_KEY_VERIFY   "/api/v1/secure/key/verify"
#define API_SECURE_KEY_ADD      "/api/v1/secure/key/add"
#define API_SECURE_KEY_CLEAR    "/api/v1/secure/key/clear"
#define API_SECURE_ARM          "/api/v1/secure/arm"
#define API_SECURE_ALARM        "/api/v1/secure/alarm"
#endif

#if defined(SECURE_MOD) || defined(LIGHT_MOD)
#define API_LIGHT_ALL   "/api/v1/light/all"
#endif

class IApi
{
public:
    virtual void registerHandlers(const std::shared_ptr<EspServer> &server) = 0;
};

#endif /* __API_HPP__ */
