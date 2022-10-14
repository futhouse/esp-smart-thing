#include <functional>
#include <ArduinoJson.h>
#ifdef ESP32
#include <HttpClient.h>
#elif defined(ESP8266)
#include <ESP8266HttpClient.h>
#endif /* ESP8266 */

#include "net/api/v1/devapi.hpp"
#include "net/html/misc.hpp"
#include "net/html/info.hpp"

DeviceApi::DeviceApi(const std::shared_ptr<IFlash>& flash,
                     const std::shared_ptr<INetwork>& net):
    _flash(move(flash)),
    _net(move(net))
{
}

void DeviceApi::registerHandlers(const std::shared_ptr<EspServer> &server)
{
    _server = server;
    _server->on("/api/v1/device/info", std::bind(&DeviceApi::devInfoHandler, this));
    _server->on("/api/v1/device/conf", std::bind(&DeviceApi::devConfHandler, this));
    _server->on("/", std::bind(&DeviceApi::devHtmlHandler, this));
}

void DeviceApi::devHtmlHandler()
{
    _server->sendContent_P(headerHtml);
    _server->sendContent_P(infoHtml);
    _server->sendContent_P(footerHtml);
}

void DeviceApi::devInfoHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);
    auto cfg = _flash->getConfigs();

    doc["name"] = String(cfg.DevName);
    doc["ip"] = _net->getIP();
    doc["mac"] = _net->getMAC();
    serializeJson(doc, out);

    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void DeviceApi::devConfHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);
    auto cfg = _flash->getConfigs();
    
    strncpy(cfg.DevName, _server->arg("name").c_str(), 19);

    if (_flash->saveData())
        doc["result"] = true;
    else
        doc["result"] = false;

    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}