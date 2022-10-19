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
            </table>

            <table border='0' cellpadding='4' cellspacing='0'>
                <tr align='center'>
                    <td colspan='4' align='center'><font color='#800080'><h2>Users</h2></font></td>
                </tr>
                <tr align='center'>
                    <td align='center'><b>#</b></td>
                    <td align='center'><b>ChatID</b></td>
                    <td align='center'><b>Notify</b></td>
                    <td align='center'><b>Bot</b></td>
                    <td align='center'><b>Enabled</b></td>
                </tr>
                <tr align='center'>
                    <td align='center'>1</td>
                    <td align='center'><input type='edit' id='id0' /></td>
                    <td><input type='checkbox' id='ntf0' /></td>
                    <td><input type='checkbox' id='bot0' /></td>
                    <td><input type='checkbox' id='en0' onclick='DisableElements()' /></td>
                </tr>
                <tr align='center'>
                    <td align='center'>2</td>
                    <td align='center'><input type='edit' id='id1' /></td>
                    <td><input type='checkbox' id='ntf1' /></td>
                    <td><input type='checkbox' id='bot1' /></td>
                    <td><input type='checkbox' id='en1' onclick='DisableElements()' /></td>
                </tr>
                <tr align='center'>
                    <td align='center'>3</td>
                    <td align='center'><input type='edit' id='id2' /></td>
                    <td><input type='checkbox' id='ntf2' /></td>
                    <td><input type='checkbox' id='bot2' /></td>
                    <td><input type='checkbox' id='en2' onclick='DisableElements()' /></td>
                </tr>
                <tr align='center'>
                    <td align='center'>4</td>
                    <td align='center'><input type='edit' id='id3' /></td>
                    <td><input type='checkbox' id='ntf3' /></td>
                    <td><input type='checkbox' id='bot3' /></td>
                    <td><input type='checkbox' id='en3' onclick='DisableElements()' /></td>
                </tr>
                <tr align='center'>
                    <td align='center'>5</td>
                    <td align='center'><input type='edit' id='id4' /></td>
                    <td><input type='checkbox' id='ntf4' /></td>
                    <td><input type='checkbox' id='bot4' /></td>
                    <td><input type='checkbox' id='en4' onclick='DisableElements()' /></td>
                </tr>
            </table>

            <table border='0' cellpadding='4' cellspacing='0'>
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

            fetch('/api/v1/telegram/info').then(function(resp) {
                    return resp.json();
                }).then(function(json) {
                    edToken.value = json.token

                    for (let i = 0; i < json.users.length; i++) {
                        let edChat = document.querySelector("#id"+i);
                        let cbNotify = document.querySelector("#ntf"+i);
                        let cbBot = document.querySelector("#bot"+i);
                        let cbEnabled = document.querySelector("#en"+i);

                        edChat.value = json.users[i].chatid
                        cbNotify.checked = json.users[i].notify
                        cbBot.checked = json.users[i].bot
                        cbEnabled.checked = json.users[i].enabled
                    }

                    DisableElements()
                })
        }

        function DisableElements() {
            for (let i = 0; i < 5; i++) {
                let edChat = document.querySelector("#id"+i);
                let cbNotify = document.querySelector("#ntf"+i);
                let cbBot = document.querySelector("#bot"+i);
                let cbEnabled = document.querySelector("#en"+i);

                edChat.disabled = !cbEnabled.checked
                cbNotify.disabled = !cbEnabled.checked
                cbBot.disabled = !cbEnabled.checked
            }
        }

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

            let users = [];
            for (let i = 0; i < 5; i++) {
                let edChat = document.querySelector("#id"+i);
                let cbNotify = document.querySelector("#ntf"+i);
                let cbBot = document.querySelector("#bot"+i);
                let cbEnabled = document.querySelector("#en"+i);

                users.push({
                        "chatid": Number(edChat.value),
                        "notify": cbNotify.checked,
                        "bot": cbBot.checked,
                        "enabled": cbEnabled.checked,
                    })
            }

            let data = {
                "token": edToken.value,
                "users": users,
            }

            fetch("/api/v1/telegram/conf", {
                method: "POST",
                headers: {
                    "Content-Type": "application/json"
                },
                body: JSON.stringify(data)
            }).then(function(resp) {
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
