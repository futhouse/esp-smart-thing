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

const PROGMEM char infoHtml[] =
    #include "net/html/header.hpp"
    R"=====(
    <body>
            <center>
                <table border='0' cellpadding='4' cellspacing='0'>
                    <tr align='center'>
                        <td colspan='2' align='center'><font color='#800080'><h2>Device Info</h2></font></td>
                    </tr>
                    <tr>
                        <td align='right'><a>Device Name:</a></td>
                        <td align='left'><font color='blue'><b><a id='aDevName'></a></b></font></td>
                    </tr>
                    <tr>
                        <td align='right'><a>IP address:</a></td>
                        <td align='left'><font color='blue'><b><a id='aDevIP'></a></b></font></td>
                    </tr>
                    <tr>
                        <td align='right'><a>MAC address:</a></td>
                        <td align='left'><font color='blue'><b><a id='aDevMac'></a></b></font></td>
                    </tr>
                    <tr align='center'>
                        <td colspan='2' align='center'><font color='#800080'><h2>Device Name</h2></font></td>
                    </tr>
                    <tr align='center'>
                        <td align='right'><a>Name:</a></td>
                        <td align='left'><font color='blue'><input type='edit' id='edName' /></font></td>
                    </tr>
                    <tr align='center'>
                        <td colspan='2'>
                            <div id='answ'></div>
                            <form action=''>
                                <br>
                                <input type='button' value='Save Configs' onclick='SetNewName();' />
                            </form>
                        </td>
                    </tr>
                </table>
            </center>
        </body>
        <script>
            function UpdateFields() {
                fetch('/api/v1/device/info').then(function(resp) {
                        return resp.json();
                    }).then(function(json) {
                        edName.value = json.name
                        aDevName.innerHTML = json.name
                        aDevIP.innerHTML = json.ip
                        aDevMac.innerHTML = json.mac
                    })
            }

            window.onload = function() {
                UpdateFields()
            }

            function SetNewName() {
                let data = {
                    "name": edName.value
                }

                fetch("/api/v1/device/conf", {
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

#endif /* __INFO_HTML_HPP__ */
