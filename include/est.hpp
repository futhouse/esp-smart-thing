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

#ifndef __ESP_SMART_THING_HPP__
#define __ESP_SMART_THING_HPP__

#include <memory>

#include "logger.hpp"
#include "flash.hpp"
#include "net/network.hpp"
#include "net/httpsrv.hpp"
#include "gpio.hpp"

#include "modules/core/sms.hpp"
#include "modules/core/telegram.hpp"
#include "modules/secure.hpp"

class EspSmartThing
{
public:
    EspSmartThing(const std::shared_ptr<ILogger>& log,
                  const std::shared_ptr<IFlash>& flash,
                  const std::shared_ptr<INetwork>& net,
                  const std::shared_ptr<IHttpSrv>& httpSrv,
                  const std::shared_ptr<IGpio>& gpio,
                  const std::shared_ptr<ISms>& sms,
                  const std::shared_ptr<ITelegram>& tg,
                  const std::shared_ptr<ISecure>& sec
                );

    /**
     * @brief Start application
     * Init all app modules
     */
    void startApp();

    /**
     * @brief Main app loop
     */
    void loop();

private:
    const std::shared_ptr<ILogger> _log;
    const std::shared_ptr<IFlash> _flash;
    const std::shared_ptr<INetwork> _net;
    const std::shared_ptr<IHttpSrv> _httpSrv;
    const std::shared_ptr<IGpio> _gpio;
    const std::shared_ptr<ISms> _sms;
    const std::shared_ptr<ITelegram> _tg;
    const std::shared_ptr<ISecure> _secure;
};

#endif /* __ESP_SMART_THING_HPP__ */
