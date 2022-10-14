#include <functional>
#include <ArduinoJson.h>
#ifdef ESP32
#include <HttpClient.h>
#elif defined(ESP8266)
#include <ESP8266HttpClient.h>
#endif /* ESP8266 */

#include "net/api/v1/tgapi.hpp"
#include "net/html/modules/core/telegram.hpp"
#include "net/html/misc.hpp"

TelegramApi::TelegramApi(const std::shared_ptr<IFlash>& flash,
                         const std::shared_ptr<ITelegram>& tg
                        ):
    _flash(move(flash)),
    _tg(move(tg))
{
}

void TelegramApi::registerHandlers(const std::shared_ptr<EspServer> &server)
{
#ifdef TELEGRAM_NOTIFY_MOD
    _server = server;
    _server->on(API_TELEGRAM_INFO, std::bind(&TelegramApi::tgInfoHandler, this));
    _server->on(API_TELEGRAM_CONF, std::bind(&TelegramApi::tgConfHandler, this));
    _server->on(API_TELEGRAM_TEST, std::bind(&TelegramApi::tgTestHandler, this));
    _server->on("/telegram.html", std::bind(&TelegramApi::tgHtmlHandler, this));
#endif
}

#ifdef TELEGRAM_NOTIFY_MOD

void TelegramApi::tgInfoHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);
    
    doc["token"] = _tg->getToken();

    auto users = _tg->getUsers();
    for (uint8_t i = 0; i < users.size(); i++) {
        doc["users"][i]["chatid"] = users[i].ChatID;
        doc["users"][i]["notify"] = users[i].Notify;
        doc["users"][i]["bot"] = users[i].Bot;
        doc["users"][i]["enabled"] = users[i].Enabled;
    }

    serializeJson(doc, out);
    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void TelegramApi::tgConfHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024), doco(1024);
    deserializeJson(doc, _server->arg(0));

    _tg->setToken(doc["token"]);

    auto users = static_cast<JsonArray>(doc["users"]);
    for (uint8_t i = 0; i < users.size(); i++) {
        auto& user = _tg->getUsers()[i];
        user.ChatID = static_cast<unsigned>(users[i]["chatid"]);
        user.Notify = static_cast<bool>(users[i]["notify"]);
        user.Bot = static_cast<bool>(users[i]["bot"]);
        user.Enabled = static_cast<bool>(users[i]["enabled"]);
    }

    doco["result"] = _tg->saveStates();
    _tg->loadStates();

    serializeJson(doco, out);
    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void TelegramApi::tgTestHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);

    doc["result"] = _tg->sendNotify("Test notify!");

    serializeJson(doc, out);
    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void TelegramApi::tgHtmlHandler()
{
    _server->sendContent_P(headerHtml);
    _server->sendContent_P(tgHtml);
    _server->sendContent_P(footerHtml);
}

#endif /* TELEGRAM_NOTIFY_MOD */
