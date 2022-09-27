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

#include <ArduinoJson.h>
#include <functional>
#ifdef ESP32
#include <HttpClient.h>
#elif defined (ESP8266)
#include <ESP8266HttpClient.h>
#endif

#include "httpsrv.hpp"
#include "html/404.hpp"
#include "html/misc.hpp"
#include "html/wifi.hpp"
#include "html/info.hpp"

#include "html/modules/core/sms.hpp"
#include "html/modules/core/telegram.hpp"

HttpSrv::HttpSrv(const std::shared_ptr<ESP8266WebServer>& syncSrv,
                 const std::shared_ptr<INetwork>& net,
                 const std::shared_ptr<ILogger>& log,
                 const std::shared_ptr<IGpio>& gpio,
                 const std::shared_ptr<IFlash>& flash,
                 const std::shared_ptr<ISms>& sms,
                 const std::shared_ptr<ITelegram>& tg
                ):
    _syncSrv(move(syncSrv)),
    _net(move(net)),
    _log(move(log)),
    _gpio(move(gpio)),
    _flash(move(flash)),
    _sms(move(sms)),
    _tg(move(tg))
{
}

void HttpSrv::setup()
{
    _log->info("HTTP", "Starting WEB server");

    _syncSrv->on("/info/index", std::bind(&HttpSrv::infoConfHandler, this));
    _syncSrv->on("/info/gpio_names", std::bind(&HttpSrv::gpioNamesHandler, this));
    _syncSrv->on("/info/wifi", std::bind(&HttpSrv::wifiInfoHandler, this));
    _syncSrv->on("/conf/wifi", std::bind(&HttpSrv::wifiConfHandler, this));
    _syncSrv->on("/conf/edname", std::bind(&HttpSrv::edNameConfHandler, this));

#ifdef SMS_NOTIFY_MOD
    _syncSrv->on("/info/sms", std::bind(&HttpSrv::smsInfoHandler, this));
    _syncSrv->on("/conf/sms", std::bind(&HttpSrv::smsConfHandler, this));
    _syncSrv->on("/test/sms", std::bind(&HttpSrv::smsTestHandler, this));
    _syncSrv->on("/sms.html", std::bind(&HttpSrv::smsHtmlHandler, this));
#endif

#ifdef TELEGRAM_NOTIFY_MOD
    _syncSrv->on("/info/telegram", std::bind(&HttpSrv::tgInfoHandler, this));
    _syncSrv->on("/conf/telegram", std::bind(&HttpSrv::tgConfHandler, this));
    _syncSrv->on("/test/telegram", std::bind(&HttpSrv::tgTestHandler, this));
    _syncSrv->on("/telegram.html", std::bind(&HttpSrv::tgHtmlHandler, this));
#endif

    _syncSrv->onNotFound(std::bind(&HttpSrv::notFoundHandler, this));
    _syncSrv->on("/", std::bind(&HttpSrv::infoHandler, this));
    _syncSrv->on("/wifi.html", std::bind(&HttpSrv::wifiHandler, this));

    _syncSrv->begin();
}

void HttpSrv::loop()
{
    _syncSrv->handleClient();
}

/**
 * @brief JSON managment handlers
 * 
 */

void HttpSrv::gpioNamesHandler()
{
    String names;

    _gpio->getGpioNames(names);

    _syncSrv->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, "[" + names + "]"); 
}

void HttpSrv::infoConfHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);
    auto cfg = _flash->getConfigs();

    doc["name"] = String(cfg->DevName);
    doc["ip"] = _net->getIP();
    doc["mac"] = _net->getMAC();
    serializeJson(doc, out);

    _syncSrv->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void HttpSrv::wifiInfoHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);
    auto cfg = _flash->getConfigs();

    doc["ssid"] = String(cfg->NetCfg.SSID);
    doc["passwd"] = String(cfg->NetCfg.Password);
    doc["ap"] = (!cfg->NetCfg.IsConnectAP) ? true : false;
    doc["inverted"] = cfg->NetCfg.IsInverted;
    doc["enabled"] = cfg->NetCfg.IsLedEnabled;

    auto led = cfg->NetCfg.StatusLED;
    auto ledPin = GpioPin
    {
        Type: static_cast<GpioType>(led.Type),
        Addr: led.Addr,
        Pin: led.Pin
    };

    doc["gpio"] = _gpio->PinToStr(ledPin);
    serializeJson(doc, out);

    _syncSrv->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void HttpSrv::wifiConfHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);
    GpioPin ledPin;
    auto cfg = _flash->getConfigs();

    strncpy(cfg->NetCfg.SSID, _syncSrv->arg("ssid").c_str(), 19);
    strncpy(cfg->NetCfg.Password, _syncSrv->arg("passwd").c_str(), 19);
    if (_syncSrv->arg("ap") == "true")
        cfg->NetCfg.IsConnectAP = false;
    else
        cfg->NetCfg.IsConnectAP = true;
    if (_syncSrv->arg("inverted") == "true")
        cfg->NetCfg.IsInverted = true;
    else
        cfg->NetCfg.IsInverted = false;
    if (_syncSrv->arg("enabled") == "true")
        cfg->NetCfg.IsLedEnabled = true;
    else
        cfg->NetCfg.IsLedEnabled = false;

    _gpio->StrToPin(_syncSrv->arg("gpio"), ledPin);
    cfg->NetCfg.StatusLED.Addr = ledPin.Addr;
    cfg->NetCfg.StatusLED.Type = ledPin.Type;
    cfg->NetCfg.StatusLED.Pin = ledPin.Pin;

    if (_flash->saveData())
        doc["result"] = true;
    else
        doc["result"] = false;

    serializeJson(doc, out);
    _syncSrv->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void HttpSrv::edNameConfHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);
    auto cfg = _flash->getConfigs();
    
    strncpy(cfg->DevName, _syncSrv->arg("name").c_str(), 19);

    if (_flash->saveData())
        doc["result"] = true;
    else
        doc["result"] = false;

    _syncSrv->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

/**
 * @brief HTML handlers
 * 
 */

void HttpSrv::notFoundHandler()
{
    _syncSrv->send(HTTP_CODE_OK, HTTP_CONTENT_HTML, notFoundHtml); 
}

void HttpSrv::infoHandler()
{
    _syncSrv->send(HTTP_CODE_OK, HTTP_CONTENT_HTML,
        String(headerHtml) + String(infoHtml) + String(footerHtml));
}

void HttpSrv::wifiHandler()
{
    _syncSrv->send(HTTP_CODE_OK, HTTP_CONTENT_HTML,
        String(headerHtml) + String(wifiHtml) + String(footerHtml));
}

/**
 * @brief Modules Handlers
 * 
 */

#ifdef SMS_NOTIFY_MOD
void HttpSrv::smsInfoHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);
    auto cfg = _flash->getConfigs();

    doc["token"] = String(cfg->SmsCfg.Token);
    doc["phone"] = String(cfg->SmsCfg.Phone);

    serializeJson(doc, out);
    _syncSrv->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void HttpSrv::smsConfHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);
    auto cfg = _flash->getConfigs();

    strncpy(cfg->SmsCfg.Token, _syncSrv->arg("token").c_str(), 38);
    strncpy(cfg->SmsCfg.Phone, _syncSrv->arg("phone").c_str(), 13);
    _sms->setCreds(cfg->SmsCfg.Token, cfg->SmsCfg.Phone);

    if (_flash->saveData())
        doc["result"] = true;
    else
        doc["result"] = false;

    serializeJson(doc, out);
    _syncSrv->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void HttpSrv::smsTestHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);

    if (_sms->sendMsg("Test notify!"))
        doc["result"] = true;
    else
        doc["result"] = false;

    serializeJson(doc, out);
    _syncSrv->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void HttpSrv::smsHtmlHandler()
{
    _syncSrv->send(HTTP_CODE_OK, HTTP_CONTENT_HTML,
        String(headerHtml) + String(smsHtml) + String(footerHtml));
}
#endif
#ifdef TELEGRAM_NOTIFY_MOD
void HttpSrv::tgInfoHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);
    auto cfg = _flash->getConfigs();

    doc["token"] = String(cfg->TelegramCfg.Token);
    doc["chat_id"] = String(cfg->TelegramCfg.ChatID);

    serializeJson(doc, out);
    _syncSrv->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void HttpSrv::tgConfHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);
    auto cfg = _flash->getConfigs();

    strncpy(cfg->TelegramCfg.Token, _syncSrv->arg("token").c_str(), 47);
    strncpy(cfg->TelegramCfg.ChatID, _syncSrv->arg("chat_id").c_str(), 10);
    _tg->setCreds(cfg->TelegramCfg.Token, cfg->TelegramCfg.ChatID);

    if (_flash->saveData())
        doc["result"] = true;
    else
        doc["result"] = false;

    serializeJson(doc, out);
    _syncSrv->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void HttpSrv::tgTestHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);

    if (_tg->sendMsg("Test notify!"))
        doc["result"] = true;
    else
        doc["result"] = false;

    serializeJson(doc, out);
    _syncSrv->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void HttpSrv::tgHtmlHandler()
{
    _syncSrv->send(HTTP_CODE_OK, HTTP_CONTENT_HTML,
        String(headerHtml) + String(tgHtml) + String(footerHtml));
}
#endif
