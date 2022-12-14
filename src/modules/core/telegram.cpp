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

void Telegram::setUser(size_t id, const TelegramUser &usr)
{
    _users[id] = usr;
}

TelegramUser* Telegram::getUsers()
{
    return _users;
}

bool Telegram::sendNotify(const String &msg)
{
    NetClient client(NET_CLIENT_HTTPS, "api.telegram.org");

    NetRequest req("/bot"+ _token + "/sendMessage");
    req.setArgE("text", msg);

    for (size_t i = 0; i < CONFIG_TG_USERS_COUNT; i++) {
        if (_users[i].Enabled && _users[i].Notify) {
            _log->info("TELEGRAM", "Sending notify: \"" + msg + "\"");
            req.setArg("chat_id", _users[i].ChatID);
            if (!client.getRequest(req))
                return false;
        }
    }

    return true;
}

#endif /* TELEGRAM_NOTIFY_MOD */

bool Telegram::saveStates()
{
#ifdef TELEGRAM_NOTIFY_MOD
    _log->info("TELEGRAM", "Saving Telegram configs");

    auto cfg = _flash->getConfigs();

    strncpy(cfg->TelegramCfg.Token, _token.c_str(), CONFIG_TG_TOKEN_LEN);
    cfg->TelegramCfg.Token[CONFIG_TG_TOKEN_LEN - 1] = '\0';

    for (size_t i = 0; i < CONFIG_TG_USERS_COUNT; i++) {
        setUser(i, {
            ChatID: _users[i].ChatID,
            Notify: _users[i].Notify,
            Bot: _users[i].Bot,
            Enabled: _users[i].Enabled,
        });
    }

    return _flash->saveData();
#else
    return false;
#endif
}

void Telegram::loadStates()
{
#ifdef TELEGRAM_NOTIFY_MOD
    _log->info("TELEGRAM", "Loading Telegram configs");

    auto& tgCfg = _flash->getConfigs()->TelegramCfg;

    setToken(tgCfg.Token);
    _log->info("TELEGRAM", "Set Token: \"" + _token + "\"");

    for (size_t i = 0; i < CONFIG_TG_USERS_COUNT; i++) {
        const auto user = tgCfg.Users[i];

        setUser(i, { 
            ChatID: user.ChatID,
            Notify: user.Notify,
            Bot: user.Bot,
            Enabled: user.Enabled
        });

        if (user.ChatID != 0) {
            _log->info("TELEGRAM", "Add new user ChatID: \"" + String(user.ChatID) + "\"");
        }
    }
#endif
}
