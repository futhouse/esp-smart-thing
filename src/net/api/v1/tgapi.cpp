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
    _server->on("/api/v1/telegram/info", std::bind(&TelegramApi::tgInfoHandler, this));
    _server->on("/api/v1/telegram/conf", std::bind(&TelegramApi::tgConfHandler, this));
    _server->on("/api/v1/telegram/test", std::bind(&TelegramApi::tgTestHandler, this));
    _server->on("/telegram.html", std::bind(&TelegramApi::tgHtmlHandler, this));
#endif
}

#ifdef TELEGRAM_NOTIFY_MOD

void TelegramApi::tgInfoHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);
    auto cfg = _flash->getConfigs();

    doc["token"] = String(cfg->TelegramCfg.Token);
    doc["chat_id"] = String(cfg->TelegramCfg.ChatID);

    serializeJson(doc, out);
    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void TelegramApi::tgConfHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);
    auto cfg = _flash->getConfigs();

    strncpy(cfg->TelegramCfg.Token, _server->arg("token").c_str(), 47);
    strncpy(cfg->TelegramCfg.ChatID, _server->arg("chat_id").c_str(), 10);
    _tg->setCreds(cfg->TelegramCfg.Token, cfg->TelegramCfg.ChatID);

    if (_flash->saveData())
        doc["result"] = true;
    else
        doc["result"] = false;

    serializeJson(doc, out);
    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void TelegramApi::tgTestHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);

    if (_tg->sendMsg("Test notify!"))
        doc["result"] = true;
    else
        doc["result"] = false;

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
