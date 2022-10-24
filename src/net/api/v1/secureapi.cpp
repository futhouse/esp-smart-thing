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
#include "net/api/v1/secureapi.hpp"
#include "net/html/modules/secure.hpp"
#include "utils.hpp"

SecureApi::SecureApi(const std::shared_ptr<IGpio>& gpio,
                     const std::shared_ptr<ISecure>& secure,
                     const std::shared_ptr<IFlash>& flash
                    ):
    _gpio(move(gpio)),
    _secure(move(secure)),
    _flash(move(flash))
{
}

void SecureApi::registerHandlers(const std::shared_ptr<EspServer> &server)
{
    _server = server;
#ifdef SECURE_MOD
    _server->on(API_SECURE_CONF, HTTP_POST, std::bind(&SecureApi::secConfHandler, this, std::placeholders::_1));

    _server->on(API_SECURE_INFO, HTTP_GET, std::bind(&SecureApi::secInfoHandler, this, std::placeholders::_1));
    _server->on(API_SECURE_TYPES, HTTP_GET, std::bind(&SecureApi::secTypesHandler, this, std::placeholders::_1));
    _server->on(API_SECURE_KEY_VERIFY, HTTP_GET, std::bind(&SecureApi::secVerifyHandler, this, std::placeholders::_1));
    _server->on(API_SECURE_KEY_CLEAR, HTTP_GET, std::bind(&SecureApi::secClearKeysHandler, this, std::placeholders::_1));
    _server->on(API_SECURE_ARM, HTTP_GET, std::bind(&SecureApi::secArmHandler, this, std::placeholders::_1));
    _server->on(API_SECURE_ALARM, HTTP_GET, std::bind(&SecureApi::secAlarmHandler, this, std::placeholders::_1));

    _server->on("/secure.html", HTTP_GET, std::bind(&SecureApi::secHtmlHandler, this, std::placeholders::_1));
#endif
}

#ifdef SECURE_MOD

void SecureApi::secInfoHandler(AsyncWebServerRequest *req) const
{
    NetResponse resp(req, 3096);

    resp.setArg("armed", _secure->getArmed());
    resp.setArg("alarm", _secure->getAlarm());
    resp.setArg("last_key", _secure->getLastKey());
    resp.setArg("inverted", _secure->getInvertAlarm());

    resp.setArg("pin_alarm", _gpio->pinToStr(_secure->getPin(SECURE_ALARM_PIN)));
    resp.setArg("pin_key", _gpio->pinToStr(_secure->getPin(SECURE_KEY_PIN)));
    resp.setArg("pin_led", _gpio->pinToStr(_secure->getPin(SECURE_LED_PIN)));

    for (uint8_t i = 0; i < CONFIG_SECURE_KEYS_COUNT; i++) {
        resp.setArg("keys", i, _secure->getKeys()[i]);
    }

    const auto sensors = _secure->getSensors();
    for (uint8_t i = 0; i < CONFIG_SECURE_SENSORS_COUNT; i++) {
        resp.setArg("sensors", i, "name", sensors[i].Name);
        resp.setArg("sensors", i, "enabled", sensors[i].Enabled);
        resp.setArg("sensors", i, "sms", sensors[i].Sms);
        resp.setArg("sensors", i, "telegram", sensors[i].Telegram);
        resp.setArg("sensors", i, "alarm", sensors[i].Alarm);
        resp.setArg("sensors", i, "type", _secure->typeToStr(sensors[i].Type));
        resp.setArg("sensors", i, "pin", _gpio->pinToStr(sensors[i].Pin));
    }

    const auto remote = _secure->getRemoteDevices();
    for (uint8_t i = 0; i < CONFIG_SECURE_REMOTE_COUNT; i++) {
        resp.setArg("remote", i, "ip", remote[i].IP);
        resp.setArg("remote", i, "enabled", remote[i].Enabled);
    }

    const auto light = _secure->getLightDevices();
    for (uint8_t i = 0; i < CONFIG_SECURE_REMOTE_COUNT; i++) {
        resp.setArg("light", i, "ip", light[i].IP);
        resp.setArg("light", i, "enabled", light[i].Enabled);
    }

    resp.setArg("master", _secure->getMaster());
    resp.sendJson();
}

void SecureApi::secTypesHandler(AsyncWebServerRequest *req) const
{
    NetResponse resp(req);
    std::vector<String> types;

    _secure->getTypes(types);
    for (uint8_t i = 0; i < types.size(); i++) {
        resp.setArg("types", i, types[i]);
    }

    resp.sendJson();
}

void SecureApi::secConfHandler(AsyncWebServerRequest *req)
{
    NetResponse resp(req);
    DynamicJsonDocument json(2048);

    _secure->setInvertAlarm(static_cast<bool>(req->arg("inverted")));
    _secure->setPin(SECURE_ALARM_PIN, _gpio->strToPin(req->arg("pin_alarm")));
    _secure->setPin(SECURE_KEY_PIN, _gpio->strToPin(req->arg("pin_key")));
    _secure->setPin(SECURE_LED_PIN, _gpio->strToPin(req->arg("pin_led")));

    json.clear();
    deserializeJson(json, req->arg("sensors"));
    for (size_t i = 0; i < CONFIG_SECURE_SENSORS_COUNT; i++) {
        _secure->setSensor(i, {
            Name: static_cast<String>(json[i]["name"]),
            Enabled: static_cast<bool>(json[i]["enabled"]),
            Sms: static_cast<bool>(json[i]["sms"]),
            Telegram: static_cast<bool>(json[i]["telegram"]),
            Alarm: static_cast<bool>(json[i]["alarm"]),
            Type: _secure->strToType(json[i]["type"]),
            Pin: _gpio->strToPin(json[i]["pin"])
        });
    }

    json.clear();
    deserializeJson(json, req->arg("remote"));
    for (uint8_t i = 0; i < CONFIG_SECURE_REMOTE_COUNT; i++) {
        _secure->setRemote(i, {
            IP: static_cast<String>(json[i]["ip"]),
            Enabled: static_cast<bool>(json[i]["enabled"])
        });
    }

    json.clear();
    deserializeJson(json, req->arg("light"));
    for (uint8_t i = 0; i < CONFIG_SECURE_REMOTE_COUNT; i++) {
        _secure->setLight(i, {
            IP: static_cast<String>(json[i]["ip"]),
            Enabled: static_cast<bool>(json[i]["enabled"])
        });
    }

    json.clear();
    deserializeJson(json, req->arg("keys"));
    for (uint8_t i = 0; i < CONFIG_SECURE_KEYS_COUNT; i++) {
        _secure->setKey(i, static_cast<String>(json[i]));
    }

    _secure->setMaster(StrToBool(req->arg("master")));

    resp.setArg("result", _secure->saveStates());
    _secure->loadStates();
    
    resp.sendJson();
}

void SecureApi::secVerifyHandler(AsyncWebServerRequest *req) const
{
    NetResponse resp(req);

    resp.setArg("result", _secure->verifyKey(req->arg("key")));

    resp.sendJson();
}

void SecureApi::secArmHandler(AsyncWebServerRequest *req)
{
    NetResponse resp(req);

    _secure->setArmed(StrToBool(req->arg("status")), true);
    resp.setArg("result", true);

    resp.sendJson();
}

void SecureApi::secAlarmHandler(AsyncWebServerRequest *req)
{
    NetResponse resp(req);

    _secure->setAlarm(StrToBool(req->arg("status")));
    
    resp.setArg("result", true);
    resp.sendJson();
}

void SecureApi::secHtmlHandler(AsyncWebServerRequest *req) const
{
    NetResponse resp(req);
    resp.sendHtml(secHtml);
}

void SecureApi::secClearKeysHandler(AsyncWebServerRequest *req)
{
    NetResponse resp(req);

    _secure->clearKeys();

    resp.setArg("result", true);
    resp.sendJson();
}

#endif /* SECURE_MOD */
