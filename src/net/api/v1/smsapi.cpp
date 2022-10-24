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
#include "net/api/v1/smsapi.hpp"
#include "net/html/modules/core/sms.hpp"

SmsApi::SmsApi(const std::shared_ptr<ISms>& sms):
    _sms(move(sms))
{
}

void SmsApi::registerHandlers(const std::shared_ptr<EspServer> &server)
{
    _server = server;
#ifdef SMS_NOTIFY_MOD
    _server->on(API_SMS_CONF, HTTP_POST, std::bind(&SmsApi::smsConfHandler, this, std::placeholders::_1));
    _server->on(API_SMS_INFO, HTTP_GET, std::bind(&SmsApi::smsInfoHandler, this, std::placeholders::_1));
    _server->on(API_SMS_TEST, HTTP_GET, std::bind(&SmsApi::smsTestHandler, this, std::placeholders::_1));
    _server->on("/sms.html", std::bind(&SmsApi::smsHtmlHandler, this, std::placeholders::_1));
#endif
}

#ifdef SMS_NOTIFY_MOD

void SmsApi::smsInfoHandler(AsyncWebServerRequest *req)
{
    NetResponse resp(req);

    resp.setArg("token", _sms->getToken());
    resp.setArg("phone", _sms->getPhone());
    
    resp.sendJson();
}

void SmsApi::smsConfHandler(AsyncWebServerRequest *req)
{
    NetResponse resp(req);

    _sms->setCreds(req->arg("token"), req->arg("phone"));
    
    resp.setArg("result", _sms->saveStates());
    _sms->loadStates();

    resp.sendJson();
}

void SmsApi::smsTestHandler(AsyncWebServerRequest *req)
{
    NetResponse resp(req);

    resp.setArg("result", _sms->sendMsg("Test notify!"));
    
    resp.sendJson();
}

void SmsApi::smsHtmlHandler(AsyncWebServerRequest *req)
{
    NetResponse resp(req);

    resp.setArg("result", _sms->sendMsg("Test notify!"));
    
    resp.sendHtml(smsHtml);
}

#endif /* SMS_NOTIFY_MOD */
