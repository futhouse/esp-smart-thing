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

#include "modules/core/telegram.hpp"

#include <UrlEncode.h>
#include "net/client.hpp"

Telegram::Telegram(const std::shared_ptr<ILogger>& log,
                    const std::shared_ptr<IFlash>& flash):
                    _log(move(log)),
                    _flash(move(flash))
{
}

#ifdef TELEGRAM_NOTIFY_MOD

void Telegram::setToken(const String &token)
{
    _token = token;
}

String& Telegram::getToken()
{
    return _token;
}

void Telegram::addUser(const TelegramUser &usr)
{
    _users.push_back(usr);
}

std::vector<TelegramUser>& Telegram::getUsers()
{
    return _users;
}

bool Telegram::sendNotify(const String &msg)
{
    NetClient client(NET_CLIENT_HTTPS, "api.telegram.org");

    for (uint8_t i = 0; i < _users.size(); i++) {
        if (_users[i].Enabled && _users[i].Notify) {
            _log->info("TELEGRAM", "Sending notify: " + msg);
            if (!client.getRequest("/bot"+ _token +
                                "/sendMessage?chat_id=" + String(_users[i].ChatID) +
                                "&text=" + urlEncode(msg)))
            {
                return false;
            }
        }
    }

    return true;
}

bool Telegram::saveStates()
{
    auto cfg = _flash->getConfigs();

    _log->info("TELEGRAM", "Saving configs");

    strncpy(cfg->TelegramCfg.Token, _token.c_str(), 47);
    cfg->TelegramCfg.Token[46] = '\0';

    for (uint8_t i = 0; i < _users.size(); i++) {
        cfg->TelegramCfg.Users[i].ChatID = _users[i].ChatID;
        cfg->TelegramCfg.Users[i].Notify = _users[i].Notify;
        cfg->TelegramCfg.Users[i].Bot = _users[i].Bot;
        cfg->TelegramCfg.Users[i].Enabled = _users[i].Enabled;
    }

    return _flash->saveData();
}

void Telegram::loadStates()
{
    auto& tgCfg = _flash->getConfigs()->TelegramCfg;

    _log->info("TELEGRAM", "Loading configs");
    _users.clear();

    setToken(tgCfg.Token);
    _log->info("TELEGRAM", "Set Token: " + _token);

    for (uint8_t i = 0; i < TELEGRAM_USERS_COUNT; i++) {
        const auto user = tgCfg.Users[i];

        addUser({ 
            ChatID: user.ChatID,
            Notify: user.Notify,
            Bot: user.Bot,
            Enabled: user.Enabled
        });

        _log->info("TELEGRAM", "Add user ChatID: " + String(user.ChatID));
    }
}

#endif /* TELEGRAM_NOTIFY_MOD */
