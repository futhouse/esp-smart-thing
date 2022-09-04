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
                            const std::shared_ptr<IHttpServer>& httpSrv,
                            const std::shared_ptr<IGpio>& gpio):
    _log(move(log)),
    _flash(move(flash)),
    _net(move(net)),
    _httpSrv(move(httpSrv)),
    _gpio(move(gpio))
{
}

void EspSmartThing::startApp()
{
    _log->setup();
    _flash->setup();
    _flash->loadData();

    /*
     * Network configuration
     */

    auto netCfg = _flash->getConfigs()->NetCfg;
    _net->setStatusLed(netCfg.StatusLED, netCfg.IsInverted);
    if (netCfg.IsConnectAP)
    {
        _log->info("EST", "Connecting to AP (SSID: " + String(netCfg.SSID) + ")");
        _net->connectToAP(netCfg.SSID, netCfg.Password);
    }
    else
    {
        _log->info("EST", "Starting new AP (SSID: " + String(CONFIG_DEFAULT_SSID) + ")");
        _net->startAP(CONFIG_DEFAULT_SSID, CONFIG_DEFAULT_PASSWD);
    }

    _httpSrv->start(CONFIG_WEB_SERVER_PORT);
}

void EspSmartThing::loop()
{
    _httpSrv->loop();
}
