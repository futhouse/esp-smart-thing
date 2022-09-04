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
#include "logger.hpp"
#include "flash.hpp"
#include "network.hpp"
#include "httpsrv.hpp"
#include "gpio.hpp"

const auto logger = std::make_shared<Logger>();
const auto flash = std::make_shared<Flash>();
const auto network = std::make_shared<Network>();
const auto gpio = std::make_shared<Gpio>(logger);
const auto espServer = std::make_shared<ESP8266WebServer>(CONFIG_WEB_SERVER_PORT);
const auto httpServer = std::make_shared<HttpServer>(espServer, network);
const auto est = std::make_shared<EspSmartThing>(logger, flash, network, httpServer, gpio);

void setup()
{
    est->startApp();
}

void loop()
{
    est->loop();
}
