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

#include "modules/core/tgbot.hpp"
#include "net/client.hpp"

#include <ArduinoJson.h>

TgBot::TgBot(const std::shared_ptr<ILogger>& log,
            const std::shared_ptr<IFlash>& flash,
            const std::shared_ptr<ITelegram>& tg,
            const std::shared_ptr<ISecure>& sec
            ):
    _log(move(log)),
    _flash(move(flash)),
    _tg(move(tg)),
    _sec(move(sec))
{
}

#ifdef TELEGRAM_BOT_MOD

void TgBot::setStatus(bool status)
{
    _status = status;
    if (status) {
        _timer.start();
        _log->info("TGBOT", "Telegram BOT was started");
    } else {
        _timer.stop();
        _log->info("TGBOT", "Telegram BOT was stoped");
    }
}

const bool TgBot::getStatus() const
{
    return _status;
}

void TgBot::setup()
{
    _timer.begin(std::bind(&TgBot::handleMain, this), TGBOT_READ_MSG_DELAY_MS, 0, MILLIS);
    for (uint8_t i = 0; i < CONFIG_TG_USERS_COUNT; i++) {
        const auto user = _tg->getUsers()[i];
        _users->ChatID = user.ChatID;
        _users->Menu = TGBOT_MENU_MAIN;
    }
}

void TgBot::loop()
{
    _timer.update();
}

void TgBot::handleMain()
{
    TgbotMessage msg;
    String out;
    std::vector<String> keybd;

    if (getLastMsg(msg)) {
        Serial.println(msg.Text);
        if (msg.Text != _lastMsg) {
            _lastMsg = msg.Text;

            if (!checkUser(msg.ChatID))
                return;

            auto& user = getUserMenu(msg.ChatID);

            switch (user.Menu)
            {
                case TGBOT_MENU_MAIN:
                    if (msg.Text == "Security") {
                        user.Menu = TGBOT_MENU_SECURE;
                    }
                    break;

                case TGBOT_MENU_SECURE:
                    if (msg.Text == "Back") {
                        user.Menu = TGBOT_MENU_MAIN;
                    }
                    break;
            }

            switch (user.Menu)
            {
                case TGBOT_MENU_MAIN:
                    keybd.push_back("Security");
                    out = "Main Menu";
                    break;

                case TGBOT_MENU_SECURE:
                    keybd.push_back("Back");
                    out = "Security Menu";
                    break;
            }

            sendMessage(msg.ChatID, out, keybd);
        }
    }
}

TgbotUser& TgBot::getUserMenu(unsigned chatId)
{
    for (size_t i = 0; i < CONFIG_TG_USERS_COUNT; i++) {
        if (_users[i].ChatID == chatId)
            return _users[i];
    }
    return _users[0];
}

const bool TgBot::checkUser(const unsigned id) const
{
    for (size_t i = 0; i < CONFIG_TG_USERS_COUNT; i++) {
        if (_users[i].ChatID == id) {
            return true;
        }
    }
    return false;
}

bool TgBot::sendMessage(unsigned chatId, const String &msg, const std::vector<String> &keybd)
{
    DynamicJsonDocument doc(1024);
    String out;
    NetClient client(NET_CLIENT_HTTP, _tg->getServer());

    NetRequest req("/telegram");
    req.setArg("token", _tg->getToken());
    req.setArg("method", "sendMessage");
    req.setArg("chat_id", chatId);
    if (keybd.size() > 0) {
        for (size_t i = 0; i < keybd.size(); i++) {
            doc["keyboard"][0][i]["text"] = keybd[i];
        }
        serializeJson(doc, out);
        req.setArg("reply_markup", out);
    }
    req.setArgE("text", msg);

    return client.getRequest(req);
}

const bool TgBot::getLastMsg(TgbotMessage &msg) const
{
    DynamicJsonDocument doc(1024);
    NetClient client(NET_CLIENT_HTTP, _tg->getServer());

    NetRequest req("/telegram");
    req.setArg("token", _tg->getToken());
    req.setArg("method", "getUpdates");
    req.setArg("offset", -1);

    if (!client.getRequest(req))
        return false;

    deserializeJson(doc, client.getPayload());
    if (!static_cast<bool>(doc["ok"]))
        return false;

    JsonArray msgs = static_cast<JsonArray>(doc["result"]);
    auto newMsg = msgs[msgs.size() - 1];
    msg.UpdateID = static_cast<unsigned>(newMsg["update_id"]);
    msg.ChatID = static_cast<unsigned>(newMsg["message"]["chat"]["id"]);
    msg.Name = static_cast<String>(newMsg["message"]["from"]["first_name"]);
    msg.Surname = static_cast<String>(newMsg["message"]["from"]["last_name"]);
    msg.Text = static_cast<String>(newMsg["message"]["text"]);

    return true;
}

#endif /* TELEGRAM_BOT_MOD */

bool TgBot::saveStates()
{
#ifdef TELEGRAM_BOT_MOD
    _log->info("TELEGRAM", "Saving Telegram BOT configs");

    auto cfg = _flash->getConfigs();
    cfg->TgbotCfg.Status = _status;

    return _flash->saveData();
#else
    return false;
#endif
}

void TgBot::loadStates()
{
#ifdef TELEGRAM_BOT_MOD
    _log->info("TELEGRAM", "Loading Telegram BOT configs");

    auto& tgbCfg = _flash->getConfigs()->TgbotCfg;

    setStatus(tgbCfg.Status);
#endif
}
