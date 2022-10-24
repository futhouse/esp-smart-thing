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

#ifndef __TELEGRAM_BOT_HPP__
#define __TELEGRAM_BOT_HPP__

#include "custom/modules.hpp"
#include "flash.hpp"
#include "logger.hpp"
#include "modules/module.hpp"
#include "modules/secure.hpp"
#include "modules/core/telegram.hpp"

#include <Arduino.h>
#include <Ticker.h>
#include <memory>
#include <map>
#include <vector>

#ifdef TELEGRAM_BOT_MOD

#define TGBOT_READ_MSG_DELAY_MS 10000

typedef enum
{
    TGBOT_MENU_MAIN,
#ifdef SECURE_MOD
    TGBOT_MENU_SECURE,
#endif
} TgbotMenuLevel;

typedef struct _TgbotUser
{
    unsigned int ChatID;
    TgbotMenuLevel Menu;
} TgbotUser;

typedef struct _TgbotMessage
{
    unsigned    UpdateID;
    unsigned    ChatID;
    String      Name;
    String      Surname;
    String      Text;
} TgbotMessage;
#endif /* TELEGRAM_BOT_MOD */

class ITgBot : public Module
{
#ifdef TELEGRAM_BOT_MOD
public:
    virtual void setStatus(bool status) = 0;
    virtual const bool getStatus() const = 0;
    virtual void setup() = 0;
    virtual void loop() = 0;
#endif /* TELEGRAM_BOT_MOD */
};

class TgBot : public ITgBot
{
public:
    TgBot(const std::shared_ptr<ILogger>& log,
          const std::shared_ptr<IFlash>& flash,
          const std::shared_ptr<ITelegram>& tg,
          const std::shared_ptr<ISecure>& sec);

    /**
     * @brief Save states to EEPROM
     * 
     * @return true 
     * @return false 
     */
    bool saveStates();

    /**
     * @brief Loading states from EEPROM
     * 
     */
    void loadStates();

#ifdef TELEGRAM_BOT_MOD
    
    /**
     * @brief Set the Status of bot
     * 
     * @param status 
     */
    void setStatus(bool status);

    /**
     * @brief Get the Status object
     * 
     * @return true 
     * @return false 
     */
    const bool getStatus() const;

    /**
     * @brief Startup configuration
     * 
     */
    void setup();

    /**
     * @brief Main loop cycle
     * 
     */
    void loop();

#endif /* TELEGRAM_BOT_MOD */
private:
    const std::shared_ptr<ILogger> _log;
    const std::shared_ptr<IFlash> _flash;
    const std::shared_ptr<ITelegram> _tg;
    const std::shared_ptr<ISecure> _sec;

#ifdef TELEGRAM_BOT_MOD
    Ticker _timer;
    bool _status;
    TgbotUser _users[CONFIG_TG_USERS_COUNT];
    String _lastMsg;

    void handleMain();
    const bool getLastMsg(TgbotMessage &msg) const;
    const bool checkUser(const unsigned id) const;
    TgbotUser& getUserMenu(unsigned chatId);
    bool sendMessage(unsigned chatId, const String &msg, const std::vector<String> &keybd);
#endif /* TELEGRAM_BOT_MOD */
};

#endif /* __TELEGRAM_BOT_HPP__ */
