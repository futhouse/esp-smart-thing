#include <functional>
#include <ArduinoJson.h>
#ifdef ESP32
#include <HttpClient.h>
#elif defined(ESP8266)
#include <ESP8266HttpClient.h>
#endif /* ESP8266 */

#include "net/api/v1/secureapi.hpp"
#include "net/html/modules/secure.hpp"
#include "net/html/misc.hpp"

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
#ifdef SECURE_MOD
    _server = server;
    _server->on("/api/v1/secure/info", std::bind(&SecureApi::secInfoHandler, this));
    _server->on("/api/v1/secure/types", std::bind(&SecureApi::secTypesHandler, this));
    _server->on("/api/v1/secure/conf", std::bind(&SecureApi::secConfHandler, this));
    _server->on("/api/v1/secure/key/verify", std::bind(&SecureApi::secVerifyHandler, this));
    _server->on("/api/v1/secure/key/add", std::bind(&SecureApi::secAddKeyHandler, this));
    _server->on("/api/v1/secure/key/clear", std::bind(&SecureApi::secClearKeysHandler, this));
    _server->on("/api/v1/secure/arm", std::bind(&SecureApi::secArmHandler, this));
    _server->on("/secure.html", std::bind(&SecureApi::secHtmlHandler, this));
#endif
}

#ifdef SECURE_MOD

void SecureApi::secInfoHandler()
{
    String out = "";
    DynamicJsonDocument doc(2024);

    doc["armed"] = _secure->getArmed();
    doc["alarm"] = _secure->getAlarm();
    doc["last_key"] = _secure->getLastKey();
    doc["inverted"] = _secure->getInvertAlarm();
    doc["pins"]["alarm"] = _gpio->pinToStr(_secure->getPin(SECURE_ALARM_PIN));
    doc["pins"]["key"] = _gpio->pinToStr(_secure->getPin(SECURE_KEY_PIN));
    doc["pins"]["led"] = _gpio->pinToStr(_secure->getPin(SECURE_LED_PIN));

    const auto keys = _secure->getKeys();
    JsonArray jaKeys = doc["keys"].to<JsonArray>();
    for (uint8_t i = 0; i < keys.size(); i++) {
        jaKeys.add(keys[i]);
    }

    const auto sensors = _secure->getSensors();
    for (uint8_t i = 0; i < sensors.size(); i++) {
        doc["sensors"][i]["name"] = sensors[i].Name;
        doc["sensors"][i]["enabled"] = sensors[i].Enabled;
        doc["sensors"][i]["sms"] = sensors[i].Sms;
        doc["sensors"][i]["telegram"] = sensors[i].Telegram;
        doc["sensors"][i]["alarm"] = sensors[i].Alarm;
        doc["sensors"][i]["type"] = _secure->typeToStr(sensors[i].Type);
        doc["sensors"][i]["pin"] = _gpio->pinToStr(sensors[i].Pin);
    }

    serializeJson(doc, out);
    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void SecureApi::secTypesHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);
    std::vector<String> types;

    _secure->getTypes(types);
    for (uint8_t i = 0; i < types.size(); i++) {
        doc["types"][i] = types[i];
    }

    serializeJson(doc, out);
    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void SecureApi::secConfHandler()
{
    String out = "";
    DynamicJsonDocument doc(2024);
    DynamicJsonDocument doco(1024);

    deserializeJson(doc, _server->arg(0));

    _secure->setInvertAlarm(static_cast<bool>(doc["inverted"]));
    _secure->setPin(SECURE_ALARM_PIN, _gpio->strToPin(doc["pins"]["alarm"]));
    _secure->setPin(SECURE_KEY_PIN, _gpio->strToPin(doc["pins"]["key"]));
    _secure->setPin(SECURE_LED_PIN, _gpio->strToPin(doc["pins"]["led"]));

    JsonArray sensors = static_cast<JsonArray>(doc["sensors"]);
    for (uint8_t i = 0; i < sensors.size(); i++) {
        const auto item = sensors.getElement(i);
        _secure->getSensors()[i].Name = static_cast<String>(item["name"]);
        _secure->getSensors()[i].Enabled = static_cast<bool>(item["enabled"]);
        _secure->getSensors()[i].Sms = static_cast<bool>(item["sms"]);
        _secure->getSensors()[i].Telegram = static_cast<bool>(item["telegram"]);
        _secure->getSensors()[i].Alarm = static_cast<bool>(item["alarm"]);
        _secure->getSensors()[i].Type = _secure->strToType(item["type"]);
        _secure->getSensors()[i].Pin = _gpio->strToPin(item["pin"]);
    }

    doco["result"] = _secure->saveStates();
    _secure->loadStates();

    serializeJson(doco, out);
    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void SecureApi::secVerifyHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);

    doc["result"] = _secure->verifyKey(_server->arg("key"));

    serializeJson(doc, out);
    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void SecureApi::secArmHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);

    _secure->setArmed((_server->arg("status") == "true") ? true : false, true);
    doc["result"] = true;

    serializeJson(doc, out);
    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void SecureApi::secHtmlHandler()
{
    _server->sendContent_P(headerHtml);
    _server->sendContent_P(secHtml);
    _server->sendContent_P(footerHtml);
}

void SecureApi::secClearKeysHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);

    _secure->clearKeys();
    doc["result"] = true;

    serializeJson(doc, out);
    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void SecureApi::secAddKeyHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);

    _secure->addKey(_server->arg("key"));
    doc["result"] = true;

    serializeJson(doc, out);
    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

#endif /* SECURE_MOD */
