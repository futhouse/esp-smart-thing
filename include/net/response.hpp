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

#ifndef __NET_RESPONSE_HPP__
#define __NET_RESPONSE_HPP__

#include <ArduinoJson.h>

#include "ESPAsyncWebServer.h"

class NetResponse
{
public:
    /**
     * @brief Construct a new Net Response object
     * 
     * @param req Async request
     * @param len Length of request
     */
    NetResponse(AsyncWebServerRequest *req, size_t len=1024);

    /**
     * @brief Set the Argument of response
     * 
     * @param name Name of argument
     * @param value Value of argument
     */
    void setArg(const String& name, const String& value);

    /**
     * @brief Set the Argument of response
     * 
     * @param name Name of argument
     * @param value Value of argument
     */
    void setArg(const String& name, const bool value);

    /**
     * @brief Set the Argument of response
     * 
     * @param id Element of array
     * @param value Value of argument
     */
    void setArg(size_t id, const String& value);

    /**
     * @brief Set the Argument of response
     * 
     * @param id Element of array
     * @param name Name of argument
     * @param value Value of argument
     */
    void setArg(size_t id, const String& name, const bool value);

    /**
     * @brief Set the Argument of response
     * 
     * @param id Element of array
     * @param name Name of argument
     * @param value Value of argument
     */
    void setArg(const String& field, size_t id, const String& name, const bool value);

    /**
     * @brief Set the Argument of response
     * 
     * @param id Element of array
     * @param name Name of argument
     * @param value Value of argument
     */
    void setArg(const String& field, size_t id, const String& name, const String& value);

    /**
     * @brief Set the Argument of response
     * 
     * @param id Element of array
     * @param name Name of argument
     * @param value Value of argument
     */
    void setArg(const String& field, size_t id, const String& name, const unsigned value);

    /**
     * @brief Set the Argument of response
     * 
     * @param field Name of array
     * @param id Id of array element
     * @param value Value of argument
     */
    void setArg(const String& field, size_t id, const String& value);

    /**
     * @brief Send JSON repsonse to client
     * 
     */
    void sendJson();

    /**
     * @brief Send HTML response
     * 
     * @param content Progmem HTML page
     */
    void sendHtml(PGM_P content);

private:
    std::shared_ptr<DynamicJsonDocument> _json;
    AsyncWebServerRequest *_req;
};

#endif /* __NET_RESPONSE_HPP__ */
