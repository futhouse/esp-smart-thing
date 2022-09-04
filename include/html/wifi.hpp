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

#ifndef __WIFI_HTML_HPP__
#define __WIFI_HTML_HPP__

#include <Arduino.h>

String PROGMEM wifiHtml = "\
    <body> \
        <center> \
            <table border='1' cellpadding='4' cellspacing='0'> \
                <tr align='center'> \
                    <th colspan='2'><h2>Wi-Fi Connection</h2></th> \
                </tr> \
                <tr align='center'> \
                    <td colspan='2'> \
                        <form action=''> \
                            <input type='radio' id='master' name='wifi' checked='checked'> \
                            <label for='master'>Start new AP</label> \
                            <input type='radio' id='slave' name='wifi'> \
                            <label for='slave'>Connect to AP</label> \
                        </form> \
                    </td> \
                </tr> \
                <tr align='center'> \
                    <td><h3>New AP</h3></td> \
                    <td><h3>Connect to AP</h3></td> \
                </tr> \
                <tr align='center'> \
                    <td> \
                        <a>SSID:</a><br><font color='blue'><h4>EspSmartHouse</h4></font> \
                        <a>Password:</a><br><font color='blue'><h4>espsmart</h4></font> \
                    </td> \
                    <td> \
                        <a>SSID:</a><br><input type='edit' id='ssid' /> \
                        <br><br><a>Password:</a><br><input type='password' id='passwd' /> \
                        <div id='answ'></div> \
                    </td> \
                </tr> \
                <tr align='center'> \
                    <td colspan='2'> \
                        <form action=''> \
                            <input type='button' value='Save Configs' onclick='ConnectToAP();' /> \
                        </form> \
                    </td> \
                </tr> \
            </table> \
        </center> \
    </body>";

String PROGMEM wifiScript = "\
    <script> \
        function ConnectToAP() { \
            let ssid = document.querySelector('#ssid') \
            let passwd = document.querySelector('#passwd') \
            let answ = document.querySelector('#answ') \
            let master = document.querySelector('#master') \
            if (!master.checked) { \
                if ((ssid.value != '') && (passwd.value != '')) { \
                    fetch('https://ipinfo.io/json').then(function(resp) { \
                            return resp.json(); \
                        }).then(function(json) { \
                            ssid.value = '' \
                            passwd.value = '' \
                            answ.innerHTML = '<a><font color=\"green\">Confirmed</font></a>' \
                        }) \
                } else { \
                    answ.innerHTML = '<a><font color=\"red\">Empty fields!</font></a>' \
                } \
            } else { \
                fetch('https://ipinfo.io/json').then(function(resp) { \
                        return resp.json(); \
                    }).then(function(json) { \
                        ssid.value = '' \
                        passwd.value = '' \
                        answ.innerHTML = '<a><font color=\"green\">Confirmed</font></a>' \
                    }) \
            } \
        } \
    </script>";

#endif /* __WIFI_HTML_HPP__ */
