#include <functional>
#include <ArduinoJson.h>
#ifdef ESP32
#include <HttpClient.h>
#elif defined(ESP8266)
#include <ESP8266HttpClient.h>
#endif /* ESP8266 */

#include "net/html/misc.hpp"
#include "net/html/wifi.hpp"
#include "net/api/v1/wifiapi.hpp"

WifiApi::WifiApi(const std::shared_ptr<IGpio> &gpio,
                 const std::shared_ptr<IFlash> &flash
                 ):
    _gpio(move(gpio)),
    _flash(move(flash))
{
}

void WifiApi::registerHandlers(const std::shared_ptr<EspServer> &server)
{
    _server = server;
    _server->on("/wifi.html", std::bind(&WifiApi::wifiHandler, this));
    _server->on(API_WIFI_INFO, std::bind(&WifiApi::wifiInfoHandler, this));
    _server->on(API_WIFI_CONF, std::bind(&WifiApi::wifiConfHandler, this));
}

void WifiApi::wifiInfoHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);
    auto cfg = _flash->getConfigs();

    doc["ssid"] = String(cfg.NetCfg.SSID);
    doc["passwd"] = String(cfg.NetCfg.Password);
    doc["ap"] = (!cfg.NetCfg.IsConnectAP) ? true : false;
    doc["inverted"] = cfg.NetCfg.IsInverted;
    doc["enabled"] = cfg.NetCfg.IsLedEnabled;

    auto led = cfg.NetCfg.StatusLED;
    auto ledPin = GpioPin
    {
        Type: static_cast<GpioType>(led.Type),
        Addr: led.Addr,
        Pin: led.Pin
    };

    doc["gpio"] = _gpio->pinToStr(ledPin);
    serializeJson(doc, out);

    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void WifiApi::wifiConfHandler()
{
    String out = "";
    DynamicJsonDocument doc(1024);
    GpioPin ledPin;
    auto cfg = _flash->getConfigs();

    strncpy(cfg.NetCfg.SSID, _server->arg("ssid").c_str(), 19);
    strncpy(cfg.NetCfg.Password, _server->arg("passwd").c_str(), 19);
    if (_server->arg("ap") == "true")
        cfg.NetCfg.IsConnectAP = false;
    else
        cfg.NetCfg.IsConnectAP = true;
    if (_server->arg("inverted") == "true")
        cfg.NetCfg.IsInverted = true;
    else
        cfg.NetCfg.IsInverted = false;
    if (_server->arg("enabled") == "true")
        cfg.NetCfg.IsLedEnabled = true;
    else
        cfg.NetCfg.IsLedEnabled = false;

    ledPin = _gpio->strToPin(_server->arg("gpio"));
    cfg.NetCfg.StatusLED.Addr = ledPin.Addr;
    cfg.NetCfg.StatusLED.Type = ledPin.Type;
    cfg.NetCfg.StatusLED.Pin = ledPin.Pin;

    if (_flash->saveData())
        doc["result"] = true;
    else
        doc["result"] = false;

    serializeJson(doc, out);
    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}

void WifiApi::wifiHandler()
{
    _server->sendContent_P(headerHtml);
    _server->sendContent_P(wifiHtml);
    _server->sendContent_P(footerHtml);
}
