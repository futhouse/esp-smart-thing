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

#ifndef __MISC_HTML_HPP__
#define __MISC_HTML_HPP__

#include <Arduino.h>

#include "custom/modules.hpp"

const PROGMEM char headerHtml[] = R"=====(
    <!DOCTYPE html>
    <html>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <head>
            <center>
                <h1>ESP Smart Thing</h1>
                <table>
                    <tr>
                        <td><a href='/'>Info</a></td>
                        <td><a href='/wifi.html'>Wi-Fi</a></td>
    )====="
#ifdef SMS_NOTIFY_MOD
    R"=====(
                        <td><a href='/sms.html'>SMS</a></td>
    )====="
#endif
#ifdef TELEGRAM_NOTIFY_MOD
    R"=====(
                        <td><a href='/telegram.html'>Telegram</a></td>
    )====="
#endif
#ifdef SECURE_MOD
    R"=====(
                        <td><a href='/secure.html'>Secure</a></td>
    )====="
#endif
    R"=====(
                    </tr>
                </table>
            </center>
        </head>
        <hr>
        <style type="text/css">
            * { font-family: arial; }
        </style>
        )=====";

const PROGMEM char footerHtml[] = R"=====(
        <hr>
        <footer>
            <center><h4>Denisov Foundation Limited (c) 2022</h4></center>
        </footer>
    </html>
    )=====";

#endif /* __MISC_HTML_HPP__ */
