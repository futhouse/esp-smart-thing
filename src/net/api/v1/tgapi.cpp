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

#include <functional>

#include "net/response.hpp"
#include "net/api/v1/tgapi.hpp"
#include "net/html/modules/core/telegram.hpp"

TelegramApi::TelegramApi(const std::shared_ptr<ITelegram>& tg
                        ):
    _tg(move(tg))
{
}

void TelegramApi::registerHandlers(const std::shared_ptr<EspServer> &server)
{
#ifdef TELEGRAM_NOTIFY_MOD
    _server = server;
    _server->on(API_TELEGRAM_INFO, std::bind(&TelegramApi::tgInfoHandler, this, std::placeholders::_1));
    _server->on(API_TELEGRAM_CONF, std::bind(&TelegramApi::tgConfHandler, this, std::placeholders::_1));
    _server->on(API_TELEGRAM_TEST, std::bind(&TelegramApi::tgTestHandler, this, std::placeholders::_1));
    _server->on("/telegram.html", std::bind(&TelegramApi::tgHtmlHandler, this, std::placeholders::_1));
#endif
}

#ifdef TELEGRAM_NOTIFY_MOD

void TelegramApi::tgInfoHandler(AsyncWebServerRequest *req) const
{
    NetResponse resp(req);
    
    resp.setArg("server", _tg->getServer());
    resp.setArg("token", _tg->getToken());

    auto users = _tg->getUsers();
    for (uint8_t i = 0; i < CONFIG_TG_USERS_COUNT; i++) {
        resp.setArg("users", i, "chatid", users[i].ChatID);
        resp.setArg("users", i, "notify", users[i].Notify);
        resp.setArg("users", i, "bot", users[i].Bot);
        resp.setArg("users", i, "enabled", users[i].Enabled);
    }
    
    resp.sendJson();
}

void TelegramApi::tgConfHandler(AsyncWebServerRequest *req)
{
    NetResponse resp(req);
    DynamicJsonDocument json(1024);

    _tg->setToken(req->arg("token"));
    _tg->setServer(req->arg("server"));

    deserializeJson(json, req->arg("users"));
    for (uint8_t i = 0; i < CONFIG_TG_USERS_COUNT; i++) {
        _tg->setUser(i, {
            ChatID: static_cast<unsigned>(json[i]["chatid"]),
            Notify: static_cast<bool>(json[i]["notify"]),
            Bot: static_cast<bool>(json[i]["bot"]),
            Enabled: static_cast<bool>(json[i]["enabled"])
        });
    }

    resp.setArg("result", _tg->saveStates());
    _tg->loadStates();
    
    resp.sendJson();
}

void TelegramApi::tgTestHandler(AsyncWebServerRequest *req) const
{
    NetResponse resp(req);
    resp.setArg("result", _tg->sendNotify("Test notify!"));
    resp.sendJson();
}

void TelegramApi::tgHtmlHandler(AsyncWebServerRequest *req) const
{
    NetResponse resp(req);
    resp.sendHtml(tgHtml);
}

#endif /* TELEGRAM_NOTIFY_MOD */
