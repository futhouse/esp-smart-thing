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

#ifndef __TELEGRAM_HPP__
#define __TELEGRAM_HPP__

#include "custom/modules.hpp"

#include <Arduino.h>

class ITelegram
{
#ifdef TELEGRAM_NOTIFY_MOD

public:
    virtual void setCreds(const String &bot, const String &chat) = 0;
    virtual bool sendMsg(const String &msg) = 0;

#endif /* TELEGRAM_NOTIFY_MOD */
};

class Telegram : public ITelegram
{
#ifdef TELEGRAM_NOTIFY_MOD

public:
    void setCreds(const String &bot, const String &chat);
    bool sendMsg(const String &msg);

private:
    String _chatID;
    String _botToken;

#endif /* TELEGRAM_NOTIFY_MOD */
};

#endif /* __TELEGRAM_HPP__ */
