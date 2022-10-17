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

#ifndef __HTTP_SRV_HPP__
#define __HTTP_SRV_HPP__

#include <memory>

#include "modules/network.hpp"
#include "net/espsrv.hpp"
#include "logger.hpp"
#include "gpio.hpp"
#include "flash.hpp"
#include "net/api/api.hpp"

#include "modules/core/sms.hpp"
#include "modules/core/telegram.hpp"
#include "modules/secure.hpp"

class IHttpSrv
{
public:
    virtual void setup() = 0;
    virtual void loop() = 0;
};

class HttpSrv: public IHttpSrv
{
public:
    HttpSrv(const std::shared_ptr<EspServer>& syncSrv,
            const std::shared_ptr<ILogger>& logger,
            const std::shared_ptr<IApi>& apiGpio,
            const std::shared_ptr<IApi>& apiDev,
            const std::shared_ptr<IApi>& apiWifi,
            const std::shared_ptr<IApi>& apiSms,
            const std::shared_ptr<IApi>& apiTg,
            const std::shared_ptr<IApi>& apiSec);

    /**
     * @brief Init and start WEB server
     * 
     */
    void setup();

    /**
     * @brief Handle clients loop
     * 
     */
    void loop();

private:
    const std::shared_ptr<EspServer> _syncSrv;
    const std::shared_ptr<ILogger> _log;
    const std::shared_ptr<IApi> _apiGpio;
    const std::shared_ptr<IApi> _apiDev;
    const std::shared_ptr<IApi> _apiWifi;
    const std::shared_ptr<IApi> _apiSms;
    const std::shared_ptr<IApi> _apiTg;
    const std::shared_ptr<IApi> _apiSec;

    void notFoundHandler();
};

#endif /* __HTTP_SRV_HPP__ */
