#include <functional>
#include <ArduinoJson.h>
#ifdef ESP32
#include <HttpClient.h>
#elif defined(ESP8266)
#include <ESP8266HttpClient.h>
#endif /* ESP8266 */

#include "net/api/v1/smsapi.hpp"
#include "net/html/modules/core/sms.hpp"
#include "net/html/misc.hpp"

SmsApi::SmsApi(const std::shared_ptr<ISms>& sms):
    _sms(move(sms))
{
}

void SmsApi::registerHandlers(const std::shared_ptr<EspServer> &server)
{
#ifdef SMS_NOTIFY_MOD
    _server = server;
    _server->on(API_SMS_INFO, std::bind(&SmsApi::smsInfoHandler, this));
    _server->on(API_SMS_CONF, std::bind(&SmsApi::smsConfHandler, this));
    _server->on(API_SMS_TEST, std::bind(&SmsApi::smsTestHandler, this));
    _server->on("/sms.html", std::bind(&SmsApi::smsHtmlHandler, this));
#endif
}

#ifdef SMS_NOTIFY_MOD

void SmsApi::smsInfoHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);

    doc["token"] = _sms->getToken();
    doc["phone"] = _sms->getPhone();

    serializeJson(doc, out);
    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void SmsApi::smsConfHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);

    _sms->setCreds(_server->arg("token"),  _server->arg("phone"));
    
    doc["result"] = _sms->saveStates();
    _sms->loadStates();

    serializeJson(doc, out);
    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void SmsApi::smsTestHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);

    doc["result"] = _sms->sendMsg("Test notify!");

    serializeJson(doc, out);
    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void SmsApi::smsHtmlHandler()
{
    _server->sendContent_P(headerHtml);
    _server->sendContent_P(smsHtml);
    _server->sendContent_P(footerHtml);
}

#endif /* SMS_NOTIFY_MOD */
