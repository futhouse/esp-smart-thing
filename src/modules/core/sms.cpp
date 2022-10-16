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

#include "modules/core/sms.hpp"
#include "net/client.hpp"

Sms::Sms(const std::shared_ptr<IFlash>& flash):
    _flash(std::move(flash))
{
}

#ifdef SMS_NOTIFY_MOD

void Sms::setCreds(const String &token, const String &phone)
{
    _token = token;
    _phone = phone;
}

bool Sms::sendMsg(const String &msg)
{
    NetClient client(NET_CLIENT_HTTPS, "sms.ru");

    NetRequest req("/sms/send");
    req.setArg("api_id", _token);
    req.setArg("to", _phone);
    req.setArgE("msg", msg);
    
    return client.getRequest(req);
}

const String& Sms::getPhone()
{
    return _phone;
}

const String& Sms::getToken()
{
    return _token;
}

#endif /* SMS_NOTIFY_MOD */

bool Sms::saveStates()
{
#ifdef SMS_NOTIFY_MOD
    auto cfg = _flash->getConfigs();

    strncpy(cfg->SmsCfg.Token, _token.c_str(), CONFIG_SMS_TOKEN_LEN);
    strncpy(cfg->SmsCfg.Phone, _phone.c_str(), CONFIG_SMS_PHONE_LEN);

    return _flash->saveData();
#else
    return false
#endif
}

void Sms::loadStates()
{
#ifdef SMS_NOTIFY_MOD
    auto cfg = _flash->getConfigs();
    setCreds(cfg->SmsCfg.Token, cfg->SmsCfg.Phone);
#endif
}
