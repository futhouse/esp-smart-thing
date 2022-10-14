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

#include "net/request.hpp"

#include <UrlEncode.h>

NetRequest::NetRequest(const String &req):
    _req(std::move(req))
{
}

void NetRequest::setArg(const String &name, const String &value)
{
    _parts[name] = value;
}

void NetRequest::setArgE(const String &name, const String &value)
{
    _parts[name] = urlEncode(value);
}

void NetRequest::setArg(const String &name, const int value)
{
   _parts[name] = String(value);
}

const String& NetRequest::getResult()
{
    _res = _req + "?";

    size_t i = 0;
    for (const auto &pair : _parts) {
        _res += pair.first + "=" + pair.second;
        if (i != (_parts.size() - 1))
            _res += "&";
        i++;
    }

    return _res;
}
