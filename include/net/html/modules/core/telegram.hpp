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

#ifndef __TG_HTML_HPP__
#define __TG_HTML_HPP__

#include <Arduino.h>

#include "custom/modules.hpp"

#ifdef TELEGRAM_NOTIFY_MOD

const PROGMEM char tgHtml[] = R"=====(
    <body>
        <center>
            <table border='0' cellpadding='4' cellspacing='0'>
                <tr>
                    <th colspan='2' align='center'><font color='#800080'><h2>Telegram Notifier</h2></font></th>
                </tr>
                <tr align='center'>
                    <td align='right'><a>Token:</a></td>
                    <td align='left'><font color='blue'><input type='edit' id='token' /></font></td>
                </tr>
                <tr align='center'>
                    <td align='right'>Chat ID:</td>
                    <td align='left'><font color='blue'><input type='edit' id='chatid' /></td>
                </tr>
                <tr align='center'>
                    <td colspan='2'>
                        <div id='answ'></div>
                    </td>
                </tr>
                <tr align='center'>
                    <td align='right'>
                        <input type='button' onclick='TestNotify()' value='Test Notify'></input>
                    </td>
                    <td align='left'>
                        <input type='button' onclick='SaveConfigs()' value='Save Configs'></input>
                    </td>
                </tr>
            </table>
        </center>
    </body>
    <script>
        window.onload = function() {
            let edToken = document.querySelector('#token')
            let edChatID = document.querySelector('#chatid')

            fetch('/api/v1/telegram/info').then(function(resp) {
                    return resp.json();
                }).then(function(json) {
                    edToken.value = json.token
                    edChatID.value = json.chat_id
                })
        };

        function TestNotify() {
            fetch('/api/v1/telegram/test').then(function(resp) {
                return resp.json();
            }).then(function(json) {
                if (json.result)
                    answ.innerHTML = '<a><font color="green">Test Complete</font></a>'
                else
                    answ.innerHTML = '<a><font color="red">Test Failed</font></a>'
            })
        }

        function SaveConfigs() {
            let edToken = document.querySelector('#token')
            let edChatID = document.querySelector('#chatid')

            if ((edToken.value == '') || (edChatID.value == '')) {
                answ.innerHTML = '<a><font color="red">Empty fields!</font></a>'
                return
            }

            fetch('/api/v1/telegram/conf?'+ new URLSearchParams({
                        token: edToken.value,
                        chat_id: edChatID.value,
                    })).then(function(resp) {
                return resp.json();
            }).then(function(json) {
                if (json.result)
                    answ.innerHTML = '<a><font color="green">Confirmed</font></a>'
                else
                    answ.innerHTML = '<a><font color="red">Failed</font></a>'
            })
        }
    </script>
    )=====";

#endif /* TELEGRAM_NOTIFY_MOD */

#endif /* __TG_HTML_HPP__ */
