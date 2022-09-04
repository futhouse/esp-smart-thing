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

#ifndef __INFO_HTML_HPP__
#define __INFO_HTML_HPP__

#include <Arduino.h>

String PROGMEM infoHtml = "\
    <body> \
        <center> \
            <table border='1' cellpadding='4' cellspacing='0'> \
                <tr align='center'> \
                    <th colspan='2'><h2>Device Info</h2></th> \
                </tr> \
                <tr align='center'> \
                    <td><h3>Param</h3></td> \
                    <td><h3>Value</h3></td> \
                </tr> \
                <tr align='center'> \
                    <td><a>Device Name</a></td> \
                    <td><p id='devName'></p></td> \
                </tr> \
                <tr align='center'> \
                    <td><a>IP address</a></td> \
                    <td><p id='devIP'></p></td> \
                </tr> \
            </table> \
        </center> \
    </body>";

String PROGMEM infoScript = "\
    <script> \
        window.onload = function() { \
            let devName = document.querySelector('#devName') \
            let devIP = document.querySelector('#devIP') \
            fetch('https://ipinfo.io/json').then(function(resp) { \
                    return resp.json(); \
                }).then(function(json) { \
                    devName.innerHTML = '<a><font color=\"blue\">Confirmed</font></a>' \
                    devIP.innerHTML = '<a><font color=\"blue\">'+json.ip+'</font></a>' \
                }) \
        }; \
    </script>";

#endif /* __INFO_HTML_HPP__ */
