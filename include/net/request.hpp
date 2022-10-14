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

#ifndef __NET_REQUEST_HPP__
#define __NET_REQUEST_HPP__

#include <Arduino.h>

#include <map>

class NetRequest
{
public:
    NetRequest(const String &req);

    void setArg(const String &name, const String &value);

    void setArgE(const String &name, const String &value);

    void setArg(const String &name, const int value);

    const String& getResult();

private:
    std::map<String, String> _parts;
    const String _req;
    String _res;
};

#endif /* __NET_REQUEST_HPP__ */
