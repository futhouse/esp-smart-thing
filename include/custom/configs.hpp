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

#ifndef __CONFIGS_HPP__
#define __CONFIGS_HPP__

#include "modules.hpp"

/*
 * Configuration defines
 */

#define CONFIG_DEFAULT_SSID     "EspSmartThing"
#define CONFIG_DEFAULT_PASSWD   ""

#define CONFIG_WEB_SERVER_PORT  80

#define CONFIG_GPIO_COUNT   9

/*
 * Personal configuration
 */

typedef struct _NetConfig
{
    bool    IsConnectAP;
    char    SSID[20];
    char    Password[20];
    uint8_t StatusLED;
    bool    IsInverted;
} NetConfig;

typedef struct _GpioConfigs
{
    uint16_t    Modes;
    uint16_t    States;
} GpioConfigs;

#ifdef SOCKET_MODULE
typedef struct _SocketConfigs
{
} SocketConfigs;
#endif

#ifdef SECURE_MODULE
typedef struct _SecureConfigs
{
} SecureConfigs;
#endif

/*
 * General configuration
 */

typedef struct _Configs
{
    NetConfig   NetCfg;
    GpioConfigs GpioCfg[CONFIG_GPIO_COUNT];
#ifdef SOCKET_MODULE
    SocketConfigs   SocketCfg;
#endif
#ifdef SECURE_MODULE
    SecureConfigs SecureCfg;
#endif
} Configs;

#endif /* __CONFIGS_HPP__ */
