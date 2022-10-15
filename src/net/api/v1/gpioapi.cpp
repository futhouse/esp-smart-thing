#include <functional>
#include <ArduinoJson.h>
#ifdef ESP32
#include <HttpClient.h>
#elif defined(ESP8266)
#include <ESP8266HttpClient.h>
#endif /* ESP8266 */

#include "net/api/v1/gpioapi.hpp"
#include "net/html/misc.hpp"
#include "net/html/info.hpp"

GpioApi::GpioApi(const std::shared_ptr<IGpio> &gpio):
    _gpio(move(gpio))
{
}

void GpioApi::registerHandlers(const std::shared_ptr<EspServer> &server)
{
    _server = server;
    _server->on(API_GPIO_INFO, std::bind(&GpioApi::gpioInfoHandler, this));
}

void GpioApi::gpioInfoHandler()
{
    String out;
    std::vector<String> gpios;
    DynamicJsonDocument doc(2048);

    _gpio->getGpioNames(gpios);
    for (uint8_t i = 0; i < gpios.size(); i++)
        doc[i] = gpios[i];

    serializeJson(doc, out);

    _server->send(HTTP_CODE_OK, HTTP_CONTENT_JSON, out); 
}
