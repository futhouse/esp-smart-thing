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

#ifndef __API_HPP__
#define __API_HPP__

#include "net/espsrv.hpp"

class IApi
{
public:
    virtual void registerHandlers(const std::shared_ptr<EspServer> &server) = 0;
};

#endif /* __API_HPP__ */
