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

const PROGMEM char infoHtml[] = R"=====(
    <body>
            <center>
                <table border='0' cellpadding='4' cellspacing='0'>
                    <tr align='center'>
                        <td colspan='2' align='center'><font color='#800080'><h2>Device Info</h2></font></td>
                    </tr>
                    <tr>
                        <td align='right'><a>Device Name:</a></td>
                        <td align='left'><b><font color="blue"><a id='devName'></a></font></b></td>
                    </tr>
                    <tr>
                        <td align='right'><a>IP address:</a></td>
                        <td align='left'><b><font color="blue"><a id='devIP'></a></font></b></td>
                    </tr>
                    <tr>
                        <td align='right'><a>MAC address:</a></td>
                        <td align='left'><b><font color="blue"><a id='devMAC'></a></font></b></td>
                    </tr>
                    <tr align='center'>
                        <td colspan='2' align='center'><font color='#800080'><h2>Device Name</h2></font></td>
                    </tr>
                    <tr align='center'>
                        <td align='right'><a>Name:</a></td>
                        <td align='left'><font color='blue'><input type='edit' id='EditName' /></font></td>
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
                let devName = document.querySelector('#devName')
                let edName = document.querySelector('#EditName')
                let devIP = document.querySelector('#devIP')
                let devMAC = document.querySelector('#devMAC')

                fetch('/api/v1/device/info').then(function(resp) {
                        return resp.json();
                    }).then(function(json) {
                        edName.value = json.name
                        devName.innerHTML = json.name
                        devIP.innerHTML = json.ip
                        devMAC.innerHTML = json.mac
                    })
            }

            window.onload = function() {
                UpdateFields()
            }

            function SetNewName() {
                let edName = document.querySelector('#EditName')

                fetch('/api/v1/device/conf?' + new URLSearchParams({
                        name: edName.value,
                    })).then(function(resp) {
                        return resp.json();
                    }).then(function(json) {
                        window.location.reload()
                    })
            }
        </script>
    )=====";

#endif /* __INFO_HTML_HPP__ */
