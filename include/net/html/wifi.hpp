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

const PROGMEM char wifiHtml[] = R"=====(
    <body>
        <center>
            <table border='0' cellpadding='4' cellspacing='0'>
                <tr>
                    <th colspan='2' align='center'><font color='#800080'><h2>Wi-Fi Connection</h2></font></th>
                </tr>
                <tr align='center'>
                    <td colspan='2'>
                        <form action=''>
                            <input onclick="DisableElements()" type='radio' id='master' name='wifi' checked='checked'>
                            <label for='master'>Start new AP</label>
                            <input onclick="DisableElements()" type='radio' id='slave' name='wifi'>
                            <label for='slave'>Connect to AP</label>
                        </form>
                    </td>
                </tr>
                <tr>
                    <td colspan='2' align='center'><h2><font color='#800080'>New AP</font></h2></td>
                </tr>
                <tr align='center'>
                    <td align='right'><a>SSID:</a></td>
                    <td align='left'><input type='edit' id='ap_ssid' /></td>
                </tr>
                <tr align='center'>
                    <td align='right'>Password:</td>
                    <td align='left'><input type='password' id='ap_passwd' /></td>
                </tr>
                <tr>
                    <td colspan='2' align='center'><font color='#800080'><h2>Connect to AP</h2></font></td>
                </tr>
                <tr align='center'>
                    <td align='right'><a>SSID:</a></td>
                    <td align='left'><input type='edit' id='ssid' /></td>
                </tr>
                <tr align='center'>
                    <td align='right'>Password:</td>
                    <td align='left'><input type='password' id='passwd' /></td>
                </tr>
                <tr>
                    <td colspan='2' align='center'><font color='#800080'><h2>Status LED</h2></font></td>
                </tr>
                <tr>
                    <td colspan='2' align='center'>
                        <input onclick='DisableElements()' type='checkbox' id='EnabledLed' />
                        <label for='EnabledLed'>Enabled</label>
                    </td>
                </tr>
                <tr align='center'>
                    <td>
                        <input type='checkbox' id='InvertedLed' />
                        <label for='StatusLed'>Inverted LED</label>
                    </td>
                    <td align='right'><select id='StatusLed'></select></td>
                </tr>
                <tr align='center'>
                    <td colspan='2'>
                        <div id='answ'></div>
                        <form action=''>
                            <br>
                            <input type='button' value='Save Configs' onclick='SaveConfigs();' />
                        </form>
                    </td>
                </tr>
            </table>
        </center>
    </body>
    <script>
        window.onload = async function() {
            let ssid = document.querySelector('#ssid')
            let passwd = document.querySelector('#passwd')
            let ap_ssid = document.querySelector('#ap_ssid')
            let ap_passwd = document.querySelector('#ap_passwd')
            let master = document.querySelector('#master')
            let slave = document.querySelector('#slave')
            let statusLed = document.querySelector('#StatusLed')
            let enabledLed = document.querySelector('#EnabledLed')
            let invertedLed = document.querySelector('#InvertedLed')

            await fetch('/api/v1/gpio/info').then(function(resp) {
                    return resp.json();
                }).then(function(json) {
                    for (let i = 0; i < json.length; i++) {
                        let option = document.createElement("option")
                        option.value = json[i]
                        option.text = json[i]
                        statusLed.add(option)
                    }
                })

            await fetch('/api/v1/wifi/info').then(function(resp) {
                    return resp.json();
                }).then(function(json) {
                    ssid.value = json.ssid
                    passwd.value = json.passwd
                    ap_ssid.value = json.ap_ssid
                    ap_passwd.value = json.ap_passwd
                    if (json.ap) {
                        master.checked = false
                        slave.checked = true
                    } else {
                        master.checked = true
                        slave.checked = false
                    }
                    invertedLed.checked = json.inverted
                    enabledLed.checked = json.enabled

                    for (let i = 0; i < statusLed.children.length; i++) {
                        let item = statusLed.children[i]
                        if (item.value == json.gpio) {
                            item.selected = true
                            break
                        }
                    }

                    DisableElements()
                })
        };

        function DisableElements() {
            let edSsid = document.querySelector('#ssid')
            let edPasswd = document.querySelector('#passwd')
            let edApSsid = document.querySelector('#ap_ssid')
            let edApPasswd = document.querySelector('#ap_passwd')
            let master = document.querySelector('#master')
            let statusLed = document.querySelector('#StatusLed')
            let enabledLed = document.querySelector('#EnabledLed')
            let invertedLed = document.querySelector('#InvertedLed')
            
            if (!master.checked) {
                edSsid.disabled = false
                edPasswd.disabled = false
                edApSsid.disabled = true
                edApPasswd.disabled = true
            } else {
                edSsid.disabled = true
                edPasswd.disabled = true
                edApSsid.disabled = false
                edApPasswd.disabled = false
            }

            if (enabledLed.checked) {
                statusLed.disabled = false
                invertedLed.disabled = false
            } else {
                statusLed.disabled = true
                invertedLed.disabled = true
            }
        }

        function SaveConfigs() {
            let edSsid = document.querySelector('#ssid')
            let edPasswd = document.querySelector('#passwd')
            let edApSsid = document.querySelector('#ap_ssid')
            let edApPasswd = document.querySelector('#ap_passwd')
            let master = document.querySelector('#master')
            let slave = document.querySelector('#slave')
            let statusLed = document.querySelector('#StatusLed')
            let enabledLed = document.querySelector('#EnabledLed')
            let invertedLed = document.querySelector('#InvertedLed')

            fetch('/api/v1/wifi/conf?'+ new URLSearchParams({
                        ssid: edSsid.value,
                        passwd: edPasswd.value,
                        ap_ssid: edApSsid.value,
                        ap_passwd: edApPasswd.value,
                        ap: master.checked,
                        inverted: invertedLed.checked,
                        enabled: enabledLed.checked,
                        gpio: statusLed.value,
                    })).then(function(resp) {
                return resp.json();
            }).then(function(json) {
                answ.innerHTML = '<a><font color="green">Confirmed</font></a>'
            })
        }
    </script>
    )=====";

#endif /* __WIFI_HTML_HPP__ */
