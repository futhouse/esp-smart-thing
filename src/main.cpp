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
#include "net/network.hpp"
#include "gpio.hpp"
#include "net/httpsrv.hpp"
#include "net/api/v1/secureapi.hpp"
#include "net/api/v1/tgapi.hpp"
#include "net/api/v1/smsapi.hpp"
#include "net/api/v1/wifiapi.hpp"
#include "net/api/v1/gpioapi.hpp"
#include "net/api/v1/devapi.hpp"
#include "est.hpp"

#include "modules/core/sms.hpp"
#include "modules/core/telegram.hpp"
#include "modules/secure.hpp"

const auto logger = std::make_shared<Logger>();
const auto flash = std::make_shared<Flash>();
const auto sms = std::make_shared<Sms>();
const auto tg = std::make_shared<Telegram>(logger, flash);
const auto dns = std::make_shared<DNSServer>();
const auto espSrv = std::make_shared<EspServer>();
const auto gpio = std::make_shared<Gpio>(logger);
const auto net = std::make_shared<Network>(dns, gpio);
const auto sec = std::make_shared<Secure>(logger, gpio, tg, sms, flash);
const auto apiGpio = std::make_shared<GpioApi>(gpio);
const auto apiDev = std::make_shared<DeviceApi>(flash, net);
const auto apiWifi = std::make_shared<WifiApi>(gpio, flash);
const auto apiSms = std::make_shared<SmsApi>(flash, sms);
const auto apiTg = std::make_shared<TelegramApi>(flash, tg);
const auto apiSec = std::make_shared<SecureApi>(gpio, sec, flash);
const auto server = std::make_shared<HttpSrv>(espSrv, logger, apiGpio, apiDev, apiWifi, apiSms, apiTg, apiSec);
const auto est = std::make_shared<EspSmartThing>(logger, flash, net, server, gpio, sms, tg, sec);

void setup()
{
    est->startApp();
}

void loop()
{
    est->loop();
}
