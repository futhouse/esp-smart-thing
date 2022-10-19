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

#include "est.hpp"

EspSmartThing::EspSmartThing(const std::shared_ptr<ILogger>& log,
                             const std::shared_ptr<IFlash>& flash,
                             const std::shared_ptr<INetwork>& net,
                             const std::shared_ptr<IHttpSrv>& httpSrv,
                             const std::shared_ptr<IGpio>& gpio,
                             const std::shared_ptr<ISms>& sms,
                             const std::shared_ptr<ITelegram>& tg,
                             const std::shared_ptr<ISecure>& secure,
                             const std::shared_ptr<ITgBot>& tgBot
                            ):
    _log(move(log)),
    _flash(move(flash)),
    _net(move(net)),
    _httpSrv(move(httpSrv)),
    _gpio(move(gpio)),
    _sms(move(sms)),
    _tg(move(tg)),
    _secure(move(secure)),
    _tgBot(move(tgBot))
{
}

void EspSmartThing::startApp()
{
    _log->setup();
    delay(1000);

    _log->info("EST", "Loading configs from EEPROM");
    _flash->setup();
    _flash->loadData();
    _log->info("EST", "Flash Used bytes: " + String(_flash->getConfigSize()) +
                        " of " + String(FLASH_SIZE));

    /**
     * @brief GPIO configuration
     * 
     */
    _gpio->setup();

    /**
     * @brief Network configuration
     * 
     */
    _log->info("EST", "Starting Network module");
    _net->loadStates();
    _net->setup();

#ifdef SMS_NOTIFY_MOD
    _log->info("EST", "Starting SMS module");
    _sms->loadStates();
#endif

#ifdef TELEGRAM_NOTIFY_MOD
    _log->info("EST", "Starting Telegram module");
    _tg->loadStates();
#ifdef TELEGRAM_BOT_MOD
    _log->info("EST", "Starting Telegram BOT module");
    _tgBot->loadStates();
    _tgBot->setup();
#endif
#endif

#ifdef SECURE_MOD
    _log->info("EST", "Starting Security module");
    _secure->setup();
    _secure->loadStates();
#endif

    _log->info("EST", "Starting HTTP server");
    _httpSrv->setup();
}

void EspSmartThing::loop()
{
    _net->loop();
    _httpSrv->loop();
#ifdef SECURE_MOD
    _secure->loop();
#ifdef TELEGRAM_BOT_MOD
    _tgBot->loop();
#endif
#endif
}
