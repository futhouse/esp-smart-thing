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
#ifdef SECURE_MOD
    _server = server;
    _server->on(API_SECURE_INFO, std::bind(&SecureApi::secInfoHandler, this));
    _server->on(API_SECURE_TYPES, std::bind(&SecureApi::secTypesHandler, this));
    _server->on(API_SECURE_CONF, std::bind(&SecureApi::secConfHandler, this));
    _server->on(API_SECURE_KEY_VERIFY, std::bind(&SecureApi::secVerifyHandler, this));
    _server->on(API_SECURE_KEY_CLEAR, std::bind(&SecureApi::secClearKeysHandler, this));
    _server->on(API_SECURE_ARM, std::bind(&SecureApi::secArmHandler, this));
    _server->on(API_SECURE_ALARM, std::bind(&SecureApi::secAlarmHandler, this));
    _server->on("/secure.html", std::bind(&SecureApi::secHtmlHandler, this));
#endif
}

#ifdef SECURE_MOD

void SecureApi::secInfoHandler()
{
    String out = "";
    DynamicJsonDocument doc(3024);

    doc["armed"] = _secure->getArmed();
    doc["alarm"] = _secure->getAlarm();
    doc["last_key"] = _secure->getLastKey();
    doc["inverted"] = _secure->getInvertAlarm();

    doc["pins"]["alarm"] = _gpio->pinToStr(_secure->getPin(SECURE_ALARM_PIN));
    doc["pins"]["key"] = _gpio->pinToStr(_secure->getPin(SECURE_KEY_PIN));
    doc["pins"]["led"] = _gpio->pinToStr(_secure->getPin(SECURE_LED_PIN));

    const auto keys = _secure->getKeys();
    for (uint8_t i = 0; i < CONFIG_SECURE_KEYS_COUNT; i++) {
         doc["keys"][i] = keys[i];
    }

    const auto sensors = _secure->getSensors();
    for (uint8_t i = 0; i < CONFIG_SECURE_SENSORS_COUNT; i++) {
        doc["sensors"][i]["name"] = sensors[i].Name;
        doc["sensors"][i]["enabled"] = sensors[i].Enabled;
        doc["sensors"][i]["sms"] = sensors[i].Sms;
        doc["sensors"][i]["telegram"] = sensors[i].Telegram;
        doc["sensors"][i]["alarm"] = sensors[i].Alarm;
        doc["sensors"][i]["type"] = _secure->typeToStr(sensors[i].Type);
        doc["sensors"][i]["pin"] = _gpio->pinToStr(sensors[i].Pin);
    }

    const auto remote = _secure->getRemoteDevices();
    for (uint8_t i = 0; i < CONFIG_SECURE_REMOTE_COUNT; i++) {
        doc["remote"][i]["ip"] = remote[i].IP;
        doc["remote"][i]["enabled"] = remote[i].Enabled;
    }

    const auto light = _secure->getLightDevices();
    for (uint8_t i = 0; i < CONFIG_SECURE_REMOTE_COUNT; i++) {
        doc["light"][i]["ip"] = light[i].IP;
        doc["light"][i]["enabled"] = light[i].Enabled;
    }

    doc["master"] = _secure->getMaster();

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
    DynamicJsonDocument doc(3024);
    DynamicJsonDocument doco(1024);

    deserializeJson(doc, _server->arg(0));

    _secure->setInvertAlarm(static_cast<bool>(doc["inverted"]));
    _secure->setPin(SECURE_ALARM_PIN, _gpio->strToPin(doc["pins"]["alarm"]));
    _secure->setPin(SECURE_KEY_PIN, _gpio->strToPin(doc["pins"]["key"]));
    _secure->setPin(SECURE_LED_PIN, _gpio->strToPin(doc["pins"]["led"]));

    JsonArray sensors = static_cast<JsonArray>(doc["sensors"]);
    for (uint8_t i = 0; i < sensors.size(); i++) {
        _secure->setSensor(i, {
            Name: static_cast<String>(sensors[i]["name"]),
            Enabled: static_cast<bool>(sensors[i]["enabled"]),
            Sms: static_cast<bool>(sensors[i]["sms"]),
            Telegram: static_cast<bool>(sensors[i]["telegram"]),
            Alarm: static_cast<bool>(sensors[i]["alarm"]),
            Type: _secure->strToType(sensors[i]["type"]),
            Pin: _gpio->strToPin(sensors[i]["pin"])
        });
    }

    JsonArray remote = static_cast<JsonArray>(doc["remote"]);
    for (uint8_t i = 0; i < remote.size(); i++) {
        _secure->setRemote(i, {
            IP: static_cast<String>(remote[i]["ip"]),
            Enabled: static_cast<bool>(remote[i]["enabled"])
        });
    }

    JsonArray light = static_cast<JsonArray>(doc["light"]);
    for (uint8_t i = 0; i < light.size(); i++) {
        _secure->setLight(i, {
            IP: static_cast<String>(light[i]["ip"]),
            Enabled: static_cast<bool>(light[i]["enabled"])
        });
    }

    JsonArray keys = static_cast<JsonArray>(doc["keys"]);
    for (uint8_t i = 0; i < keys.size(); i++) {
        _secure->setKey(i, static_cast<String>(keys[i]));
    }

    _secure->setMaster(static_cast<bool>(doc["master"]));

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

    _secure->setArmed(StrToBool(_server->arg("status")), true);
    doc["result"] = true;

    serializeJson(doc, out);
    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void SecureApi::secAlarmHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);

    _secure->setAlarm(StrToBool(_server->arg("status")));
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

#endif /* SECURE_MOD */
