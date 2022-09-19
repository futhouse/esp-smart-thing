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

#define CONFIG_MAGIC    0xA4A2

#define CONFIG_DEFAULT_SSID     "EspSmartThing"

#define CONFIG_WEB_SERVER_PORT  80
#define CONFIG_DNS_SERVER_PORT  53

#define CONFIG_EXT_GPIO_COUNT   8

#define CONFIG_STR_LEN  20

/*
 * Personal configuration
 */

typedef struct _PinConfig
{
    uint8_t Type;
    uint8_t Addr;
    uint8_t Pin;
} PinConfig;

typedef struct _NetConfig
{
    bool        IsConnectAP;
    char        SSID[CONFIG_STR_LEN];
    char        Password[CONFIG_STR_LEN];
    PinConfig   StatusLED;
    bool        IsInverted;
    bool        IsLedEnabled;
} NetConfig;

typedef struct _ConfigGpioExt
{
    uint16_t    Modes;
    uint16_t    States;
} ConfigGpioExt;

typedef struct _ConfigGpioInt
{
    uint32_t    Modes;
    uint32_t    States;
} ConfigGpioInt;

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
    uint16_t        Magic;
    char            DevName[CONFIG_STR_LEN];
    NetConfig       NetCfg;
    ConfigGpioInt   GpioIntCfg;
    ConfigGpioExt   GpioExtCfg[CONFIG_EXT_GPIO_COUNT];
#ifdef SOCKET_MODULE
    SocketConfigs   SocketCfg;
#endif
#ifdef SECURE_MODULE
    SecureConfigs   SecureCfg;
#endif
} Configs;

#endif /* __CONFIGS_HPP__ */
