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
#include "flash.hpp"
#include "logger.hpp"
#include "modules/module.hpp"

#include <Arduino.h>
#include <vector>
#include <memory>

typedef struct _TelegramUser
{
    unsigned    ChatID;
    bool        Notify;
    bool        Bot;
    bool        Enabled;
} TelegramUser;

class ITelegram : public Module
{
#ifdef TELEGRAM_NOTIFY_MOD
public:
    virtual void setToken(const String &token) = 0;
    virtual const String& getToken() const = 0;
    virtual void setUser(size_t id, const TelegramUser &user) = 0;
    virtual const TelegramUser* getUsers() const = 0;
    virtual bool sendNotify(const String &msg) = 0;
    virtual bool saveStates() = 0;
    virtual void loadStates() = 0;
    virtual const String& getServer() const = 0;
    virtual void setServer(const String &server) = 0;
#endif /* TELEGRAM_NOTIFY_MOD */
};

class Telegram : public ITelegram
{
public:
    Telegram(const std::shared_ptr<ILogger>& log,
            const std::shared_ptr<IFlash>& flash);

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

#ifdef TELEGRAM_NOTIFY_MOD
    /**
     * @brief Set the Token object
     * 
     * @param token 
     */
    void setToken(const String &token);

    /**
     * @brief Get the BOT Token
     * 
     * @return String& 
     */
    const String& getToken() const;

    /**
     * @brief Add new user
     * 
     * @param id 
     */
    void setUser(size_t id, const TelegramUser &usr);

    /**
     * @brief Get the users
     * 
     * @return TelegramUser*
     */
    const TelegramUser* getUsers() const;

    /**
     * @brief Send Telegram Notify
     * 
     * @param msg Notify message
     * @return true 
     * @return false 
     */
    bool sendNotify(const String &msg);

    /**
     * @brief Get Telegram Proxy Server
     * 
     * @return const String& IP
     */
    const String& getServer() const;

    /**
     * @brief Set Telegram proxy server
     * 
     * @param server IP
     */
    void setServer(const String &server);

#endif /* TELEGRAM_NOTIFY_MOD */
private:
    const std::shared_ptr<ILogger> _log;
    const std::shared_ptr<IFlash> _flash;

#ifdef TELEGRAM_NOTIFY_MOD
    TelegramUser _users[CONFIG_TG_USERS_COUNT];
    String _token;
    String _server;
#endif /* TELEGRAM_NOTIFY_MOD */
};

#endif /* __TELEGRAM_HPP__ */
