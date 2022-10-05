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

#ifndef __TG_API_HPP__
#define __TG_API_HPP__

#include <Arduino.h>

#include "custom/modules.hpp"
#include "modules/core/telegram.hpp"
#include "flash.hpp"
#include "net/espsrv.hpp"
#include "../api.hpp"

class TelegramApi: public IApi
{
public:
    TelegramApi(const std::shared_ptr<IFlash>& flash,
                const std::shared_ptr<ITelegram>& tg);

    void registerHandlers(const std::shared_ptr<EspServer> &server);

private:
    const std::shared_ptr<IFlash> _flash;
    const std::shared_ptr<ITelegram> _tg;
    std::shared_ptr<EspServer> _server;
#ifdef TELEGRAM_NOTIFY_MOD
    void tgInfoHandler();
    void tgConfHandler();
    void tgTestHandler();
    void tgHtmlHandler();
#endif
};

#endif /* __TG_API_HPP__ */
