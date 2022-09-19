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

    auto cfg = _flash->getConfigs();

    /**
     * @brief GPIO configuration
     * 
     */

    _gpio->setup();

    /**
     * @brief Network configuration
     * 
     */

    auto led = cfg->NetCfg.StatusLED;

    const auto& gpioPin = GpioPin
    {
        Type: static_cast<GpioType>(led.Type),
        Addr: led.Addr,
        Pin: led.Pin
    };

    _net->setStatusLed(cfg->NetCfg.IsLedEnabled, gpioPin, cfg->NetCfg.IsInverted);
    if (cfg->NetCfg.IsConnectAP)
    {
        _log->info("EST", "Connecting to AP (SSID: " + String(cfg->NetCfg.SSID) + ")");
        _net->connectToAP(cfg->NetCfg.SSID, cfg->NetCfg.Password);
    }
    else
    {
        _log->info("EST", "Starting new AP (SSID: " + String(CONFIG_DEFAULT_SSID) + ")");
        _net->startAP(CONFIG_DEFAULT_SSID);
    }

    _httpSrv->setup();
}

void EspSmartThing::loop()
{
    _net->loop();
}
