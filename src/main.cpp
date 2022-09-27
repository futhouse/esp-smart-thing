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

#include "logger.hpp"
#include "flash.hpp"
#include "network.hpp"
#include "gpio.hpp"
#include "httpsrv.hpp"
#include "est.hpp"

#include "modules/core/sms.hpp"
#include "modules/core/telegram.hpp"

const auto logger = std::make_shared<Logger>();
const auto flash = std::make_shared<Flash>();
const auto sms = std::make_shared<Sms>();
const auto tg = std::make_shared<Telegram>();
const auto dns = std::make_shared<DNSServer>();
const auto gpio = std::make_shared<Gpio>(logger);
const auto network = std::make_shared<Network>(dns, gpio);
const auto asyncSrv = std::make_shared<ESP8266WebServer>(CONFIG_WEB_SERVER_PORT);
const auto server = std::make_shared<HttpSrv>(asyncSrv, network, logger, gpio, flash, sms, tg);
const auto est = std::make_shared<EspSmartThing>(logger, flash, network, server, gpio, sms, tg);

void setup()
{
    est->startApp();
}

void loop()
{
    est->loop();
}
