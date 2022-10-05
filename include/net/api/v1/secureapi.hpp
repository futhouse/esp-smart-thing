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

#ifndef __SECURE_API_HPP__
#define __SECURE_API_HPP__

#include <Arduino.h>

#include "custom/modules.hpp"
#include "modules/secure.hpp"
#include "net/espsrv.hpp"
#include "../api.hpp"
#include "flash.hpp"
#include "gpio.hpp"

class SecureApi: public IApi
{
public:
    SecureApi(const std::shared_ptr<IGpio>& gpio,
              const std::shared_ptr<ISecure>& secure,
              const std::shared_ptr<IFlash>& flash);

    void registerHandlers(const std::shared_ptr<EspServer> &server);

private:
    const std::shared_ptr<IGpio> _gpio;
    const std::shared_ptr<ISecure> _secure;
    const std::shared_ptr<IFlash> _flash;
    std::shared_ptr<EspServer> _server;
#ifdef SECURE_MOD
    void secInfoHandler();
    void secConfHandler();
    void secTypesHandler();
    void secHtmlHandler();
    void secVerifyHandler();
    void secArmHandler();
    void secClearKeysHandler();
    void secAddKeyHandler();
#endif
};

#endif /* __SECURE_API_HPP__ */
