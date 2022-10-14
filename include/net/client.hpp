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

#ifndef __NET_CLIENT_HPP__
#define __NET_CLIENT_HPP__

#include <Arduino.h>

typedef enum _NetClientType
{
    NET_CLIENT_HTTP,
    NET_CLIENT_HTTPS
} NetClientType;

class NetClient
{
public:
    /**
     * @brief Construct a new Net Client object
     * 
     * @param typ Type of connection
     * @param addr Server address
     */
    NetClient(NetClientType typ, const String& addr);

    /**
     * @brief GET request
     * 
     * @param req Request string
     * @return true 
     * @return false 
     */
    bool getRequest(const String& req);

    /**
     * @brief Get the Payload after request complete
     * 
     * @return String& 
     */
    String& getPayload();

private:
    String _payload = "";
    String _addr = "";
};

#endif /* __NET_CLIENT_HPP__ */
