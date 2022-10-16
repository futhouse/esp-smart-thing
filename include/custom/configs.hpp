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

#define CONFIG_MAGIC            0xA4A1
#define CONFIG_DEFAULT_SSID     "EspSmartThing"
#define CONFIG_DNS_SERVER_PORT  53
#define CONFIG_EXT_GPIO_COUNT   8
#define CONFIG_STR_LEN          20
#define CONFIG_IP_LEN           16

/*
 * Personal configuration
 */

typedef struct _RemoteDev
{
    char IP[CONFIG_IP_LEN];
    bool Enabled;
} RemoteDev;

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

#ifdef TELEGRAM_NOTIFY_MOD
#define CONFIG_TG_TOKEN_LEN     47
#define CONFIG_TG_USERS_COUNT   5

typedef struct _TelegramUserConfig
{
    unsigned    ChatID;
    bool        Notify;
    bool        Bot;
    bool        Enabled;
} TelegramUserConfig;

typedef struct _TelegramConfigs
{
    char                Token[CONFIG_TG_TOKEN_LEN];
    TelegramUserConfig  Users[CONFIG_TG_USERS_COUNT];
} TelegramConfigs;
#endif

#ifdef SMS_NOTIFY_MOD
#define CONFIG_SMS_TOKEN_LEN    38
#define CONFIG_SMS_PHONE_LEN    13

typedef struct _SmsConfigs
{
    char Token[CONFIG_SMS_TOKEN_LEN];
    char Phone[CONFIG_SMS_PHONE_LEN];
} SmsConfigs;
#endif

#ifdef SECURE_MOD
#define CONFIG_SECURE_NAME_LEN      10
#define CONFIG_SECURE_SENSORS_COUNT 10
#define CONFIG_SECURE_KEYS_COUNT    5
#define CONFIG_SECURE_KEY_LEN       17
#define CONFIG_SECURE_REMOTE_COUNT  5

typedef struct _SecureSensorConfig
{
    char        Name[CONFIG_SECURE_NAME_LEN];
    uint8_t     Type;
    bool        Enabled;
    bool        Telegram;
    bool        Sms;
    bool        Alarm;
    PinConfig   Pin;
} SecureSensorConfig;

typedef struct _SecureConfigs
{
    bool                Armed;
    bool                Alarm;
    bool                InvertedAlarm;
    PinConfig           AlarmPin;
    PinConfig           KeyPin;
    PinConfig           LedPin;
    SecureSensorConfig  Sensors[CONFIG_SECURE_SENSORS_COUNT];
    char                Keys[CONFIG_SECURE_KEYS_COUNT][CONFIG_SECURE_KEY_LEN];
    bool                Master;
    RemoteDev           Remote[CONFIG_SECURE_REMOTE_COUNT];
    RemoteDev           Light[CONFIG_SECURE_REMOTE_COUNT];
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
#ifdef TELEGRAM_NOTIFY_MOD
    TelegramConfigs TelegramCfg;
#endif
#ifdef SMS_NOTIFY_MOD
    SmsConfigs      SmsCfg;
#endif
#ifdef SECURE_MOD
    SecureConfigs   SecureCfg;
#endif
} Configs;

#endif /* __CONFIGS_HPP__ */
