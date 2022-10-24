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
#include "custom/modules.hpp"

const PROGMEM char wifiHtml[] =
    #include "net/html/header.hpp"
    R"=====(
    <body>
        <center>
            <table border='0' cellpadding='4' cellspacing='0'>
                <tr>
                    <th colspan='2' align='center'><font color='#800080'><h2>Wi-Fi Connection</h2></font></th>
                </tr>
                <tr align='center'>
                    <td colspan='2'>
                        <form action=''>
                            <input onclick="DisableElements()" type='radio' id='rbMaster' name='wifi'>
                            <label for='rbMaster'>Start new AP</label>
                            <input onclick="DisableElements()" type='radio' id='rbSlave' name='wifi'>
                            <label for='rbSlave'>Connect to AP</label>
                        </form>
                    </td>
                </tr>
                <tr>
                    <td colspan='2' align='center'><h2><font color='#800080'>New AP</font></h2></td>
                </tr>
                <tr align='center'>
                    <td align='right'><a>SSID:</a></td>
                    <td align='left'><input type='edit' id='edApSsid' /></td>
                </tr>
                <tr align='center'>
                    <td align='right'>Password:</td>
                    <td align='left'><input type='password' id='edApPasswd' /></td>
                </tr>
                <tr>
                    <td colspan='2' align='center'><font color='#800080'><h2>Connect to AP</h2></font></td>
                </tr>
                <tr align='center'>
                    <td align='right'><a>SSID:</a></td>
                    <td align='left'><input type='edit' id='edSsid' /></td>
                </tr>
                <tr align='center'>
                    <td align='right'>Password:</td>
                    <td align='left'><input type='password' id='edPasswd' /></td>
                </tr>
                <tr>
                    <td colspan='2' align='center'><font color='#800080'><h2>Status LED</h2></font></td>
                </tr>
                <tr>
                    <td colspan='2' align='center'>
                        <input onclick='DisableElements()' type='checkbox' id='cbEnabled' />
                        <label for='cbEnabled'>Enabled</label>
                    </td>
                </tr>
                <tr align='center'>
                    <td>
                        <input type='checkbox' id='cbInverted' />
                        <label for='cmbStatusLed'>Inverted LED</label>
                    </td>
                    <td align='right'><select id='cmbStatusLed'></select></td>
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
            await fetch('/api/v1/gpio/info').then(function(resp) {
                    return resp.json();
                }).then(function(json) {
                    for (let i = 0; i < json.length; i++) {
                        let option = document.createElement("option")
                        option.value = json[i]
                        option.text = json[i]
                        cmbStatusLed.add(option)
                    }
                })

            await fetch('/api/v1/wifi/info').then(function(resp) {
                    return resp.json();
                }).then(function(json) {
                    edSsid.value = json.ssid
                    edPasswd.value = json.passwd
                    edApSsid.value = json.ap_ssid
                    edApPasswd.value = json.ap_passwd
                    if (json.ap) {
                        rbMaster.checked = true
                        rbSlave.checked = false
                    } else {
                        rbMaster.checked = false
                        rbSlave.checked = true
                    }
                    cbInverted.checked = json.inverted
                    cbEnabled.checked = json.enabled

                    for (let i = 0; i < cmbStatusLed.children.length; i++) {
                        let item = cmbStatusLed.children[i]
                        if (item.value == json.gpio) {
                            item.selected = true
                            break
                        }
                    }

                    DisableElements()
                })
        };

        function DisableElements() {
            if (!rbMaster.checked) {
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

            if (cbEnabled.checked) {
                cmbStatusLed.disabled = false
                cbInverted.disabled = false
            } else {
                cmbStatusLed.disabled = true
                cbInverted.disabled = true
            }
        }

        function SaveConfigs() {
            let data = {
                "ssid": edSsid.value,
                "passwd": edPasswd.value,
                "ap_ssid": edApSsid.value,
                "ap_passwd": edApPasswd.value,
                "ap": rbMaster.checked,
                "inverted": cbInverted.checked,
                "enabled": cbEnabled.checked,
                "gpio": cmbStatusLed.value
            }

            fetch("/api/v1/wifi/conf", {
                method: "POST",
                headers: {
                    "Content-Type": "application/x-www-form-urlencoded"
                },
                body: new URLSearchParams(data)
            }).then(function(resp) {
                return resp.json();
            }).then(function(json) {
                if (json.result) {
                    answ.innerHTML = '<a><font color="green">Confirmed</font></a>'
                    setTimeout(function() {
                        location.reload();
                    }, 1000);
                }
                else {
                    answ.innerHTML = '<a><font color="red">Failed</font></a>'
                }
            })
        }
    </script>
    )====="
    #include "net/html/footer.hpp"

#endif /* __WIFI_HTML_HPP__ */
