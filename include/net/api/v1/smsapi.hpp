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

#ifndef __SMS_API_HPP__
#define __SMS_API_HPP__

#include <Arduino.h>

#include "custom/modules.hpp"
#include "modules/core/sms.hpp"
#include "flash.hpp"
#include "net/espsrv.hpp"
#include "../api.hpp"

class SmsApi: public IApi
{
public:
    SmsApi(const std::shared_ptr<IFlash>& flash,
           const std::shared_ptr<ISms>& sms);

    void registerHandlers(const std::shared_ptr<EspServer> &server);

private:
    const std::shared_ptr<IFlash> _flash;
    const std::shared_ptr<ISms> _sms;
    std::shared_ptr<EspServer> _server;
#ifdef SMS_NOTIFY_MOD
    void smsInfoHandler();
    void smsConfHandler();
    void smsTestHandler();
    void smsHtmlHandler();
#endif
};

#endif /* __SMS_API_HPP__ */
