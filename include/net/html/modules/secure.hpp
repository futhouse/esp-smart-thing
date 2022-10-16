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

#ifndef __SECURE_HTML_HPP__
#define __SECURE_HTML_HPP__

#include <Arduino.h>

#include "custom/modules.hpp"

#ifdef SECURE_MOD

const PROGMEM char secHtml[] = R"=====(
    <body>
        <center>
            <table border='0' cellpadding='4' cellspacing='0'>
                <tr>
                    <th colspan='2' align='center'><font color='#800080'><h2>Security System</h2></font></th>
                </tr>
                <tr align='center'>
                    <td align='right'><a>Status:</a></td>
                    <td align='left' id="status"></td>
                </tr>
                <tr align='center'>
                    <td align='right'><a>Alarm:</a></td>
                    <td align='left' id="alarm"></td>
                </tr>
                <tr align='center'>
                    <td align='right'><a>Status LED:</a></td>
                    <td align='left'><select id='gpioLED'></select></td>
                </tr>
                <tr align='center'>
                    <td align='right'>
                        <input type='button' onclick='Arm(true);' value='Arm'></input>
                    </td>
                    <td align='left'>
                        <input type='button' onclick='Arm(false);' value='Disarm'></input>
                    </td>
                </tr>
                <tr>
                    <td colspan='2' align='center'><font color='#800080'><h2>Alarm</h2></font></th>
                </tr>
                <tr align='center'>
                    <td alighn="right">
                        <input type='checkbox' id='cbInverted' />
                        <label for='cbInverted'>Inverted</label>
                    </td>
                    <td align='left'><select id='gpioAlarm'></select></td>
                </tr>
                <tr>
                    <td colspan='2' align='center'><font color='#800080'><h2>Keys</h2></font></th>
                </tr>
                <tr align='center'>
                    <td align='right'><a>iButton GPIO:</a></td>
                    <td align='left'><select id='gpioKey'></select></td>
                </tr>
                <tr align='center'>
                    <td align='right'><a>Last Key:</a></td>
                    <td align='left'><font color='blue'><a id="lastKey"></a></font></td>
                </tr>
                <tr align='center'>
                    <td align='center' colspan="2"><select size="5" id='keys'></select></td>
                </tr>
                <tr align='center'>
                    <td align='right'><a>New Key:</a></td>
                    <td align='left'><input type='edit' id='newKey' /></td>
                </tr>
                <tr align='center'>
                    <td align='right'>
                        <input type='button' onclick='AddKey()' value='Add Key'></input>
                    </td>
                    <td align='left'>
                        <input type='button' onclick='ClearKeys()' value='Clear Keys'></input>
                    </td>
                </tr>
                <tr>
                    <td colspan='2' align='center'><font color='#800080'><h2>Sensors</h2></font></th>
                </tr>
            </table>
            <table border='0' cellpadding='4' cellspacing='0'>
                <tr>
                    <th align='center'><b>#</b></th>
                    <th align='center'><b>Name</b></th>
                    <th align='center'><b>Type</b></th>
                    <th align='center'><b>GPIO</b></th>
                    <th align='center'><b>SMS</b></th>
                    <th align='center'><b>Telegram</b></th>
                    <th align='center'><b>Alarm</b></th>
                    <th align='center'><b>Enabled</b></th>
                </tr>
                <tr align='center'>
                    <td><a>1</a></td>
                    <td><input type='edit' id='name0' /></td>
                    <td><select id='type0'></select></td>
                    <td><select id='gpio0'></select></td>
                    <td><input type='checkbox' id='sms0' /></td>
                    <td><input type='checkbox' id='tg0' /></td>
                    <td><input type='checkbox' id='alrm0' /></td>
                    <td><input type='checkbox' id='enable0' onclick='DisableElements()' /></td>
                </tr>
                <tr align='center'>
                    <td><a>2</a></td>
                    <td><input type='edit' id='name1' /></td>
                    <td><select id='type1'></select></td>
                    <td><select id='gpio1'></select></td>
                    <td><input type='checkbox' id='sms1' /></td>
                    <td><input type='checkbox' id='tg1' /></td>
                    <td><input type='checkbox' id='alrm1' /></td>
                    <td><input type='checkbox' id='enable1' onclick='DisableElements()' /></td>
                </tr>
                <tr align='center'>
                    <td><a>3</a></td>
                    <td><input type='edit' id='name2' /></td>
                    <td><select id='type2'></select></td>
                    <td><select id='gpio2'></select></td>
                    <td><input type='checkbox' id='sms2' /></td>
                    <td><input type='checkbox' id='tg2' /></td>
                    <td><input type='checkbox' id='alrm2' /></td>
                    <td><input type='checkbox' id='enable2' onclick='DisableElements()' /></td>
                </tr>
                <tr align='center'>
                    <td><a>4</a></td>
                    <td><input type='edit' id='name3' /></td>
                    <td><select id='type3'></select></td>
                    <td><select id='gpio3'></select></td>
                    <td><input type='checkbox' id='sms3' /></td>
                    <td><input type='checkbox' id='tg3' /></td>
                    <td><input type='checkbox' id='alrm3' /></td>
                    <td><input type='checkbox' id='enable3' onclick='DisableElements()' /></td>
                </tr>
                <tr align='center'>
                    <td><a>5</a></td>
                    <td><input type='edit' id='name4' /></td>
                    <td><select id='type4'></select></td>
                    <td><select id='gpio4'></select></td>
                    <td><input type='checkbox' id='sms4' /></td>
                    <td><input type='checkbox' id='tg4' /></td>
                    <td><input type='checkbox' id='alrm4' /></td>
                    <td><input type='checkbox' id='enable4' onclick='DisableElements()' /></td>
                </tr>
                <tr align='center'>
                    <td><a>6</a></td>
                    <td><input type='edit' id='name5' /></td>
                    <td><select id='type5'></select></td>
                    <td><select id='gpio5'></select></td>
                    <td><input type='checkbox' id='sms5' /></td>
                    <td><input type='checkbox' id='tg5' /></td>
                    <td><input type='checkbox' id='alrm5' /></td>
                    <td><input type='checkbox' id='enable5' onclick='DisableElements()' /></td>
                </tr>
                <tr align='center'>
                    <td><a>7</a></td>
                    <td><input type='edit' id='name6' /></td>
                    <td><select id='type6'></select></td>
                    <td><select id='gpio6'></select></td>
                    <td><input type='checkbox' id='sms6' /></td>
                    <td><input type='checkbox' id='tg6' /></td>
                    <td><input type='checkbox' id='alrm6' /></td>
                    <td><input type='checkbox' id='enable6' onclick='DisableElements()' /></td>
                </tr>
                <tr align='center'>
                    <td><a>8</a></td>
                    <td><input type='edit' id='name7' /></td>
                    <td><select id='type7'></select></td>
                    <td><select id='gpio7'></select></td>
                    <td><input type='checkbox' id='sms7' /></td>
                    <td><input type='checkbox' id='tg7' /></td>
                    <td><input type='checkbox' id='alrm7' /></td>
                    <td><input type='checkbox' id='enable7' onclick='DisableElements()' /></td>
                </tr>
                <tr align='center'>
                    <td><a>9</a></td>
                    <td><input type='edit' id='name8' /></td>
                    <td><select id='type8'></select></td>
                    <td><select id='gpio8'></select></td>
                    <td><input type='checkbox' id='sms8' /></td>
                    <td><input type='checkbox' id='tg8' /></td>
                    <td><input type='checkbox' id='alrm8' /></td>
                    <td><input type='checkbox' id='enable8' onclick='DisableElements()' /></td>
                </tr>
                <tr align='center'>
                    <td><a>10</a></td>
                    <td><input type='edit' id='name9' /></td>
                    <td><select id='type9'></select></td>
                    <td><select id='gpio9'></select></td>
                    <td><input type='checkbox' id='sms9' /></td>
                    <td><input type='checkbox' id='tg9' /></td>
                    <td><input type='checkbox' id='alrm9' /></td>
                    <td><input type='checkbox' id='enable9' onclick='DisableElements()' /></td>
                </tr>
            </table>
            <table border='0' cellpadding='4' cellspacing='0'>
                <tr>
                    <td colspan="3" align='center'>
                        <font color='#800080'><h2>Remote Modules</h2></font>
                    </td>
                </tr>
                <tr align='center'>
                    <td colspan='2'>
                        <form action=''>
                            <input onclick="DisableElements()" type='radio' id='master' name='remm' checked='checked'>
                            <label for='master'>Master Module</label>
                            <input onclick="DisableElements()" type='radio' id='slave' name='remm'>
                            <label for='slave'>Slave Module</label>
                        </form>
                    </td>
                </tr>
                <tr>
                    <td>
                        <table border='0' cellpadding='4' cellspacing='0'>
                            <td colspan="3" align='center'>
                                <font color='green'><h3>Security</h3></font>
                            </td>
                            <tr>
                                <td align='center'><b>#</b></td>
                                <td align='center'><b>IP</b></td>
                                <td align='center'><b>Enabled</b></td>
                            </tr>
                            <tr align='center'>
                                <td><a>1</a></td>
                                <td><input type='edit' id='rmip0' /></td>
                                <td><input type='checkbox' id='rmen0' onclick='DisableElements()' /></td>
                            </tr>
                            <tr align='center'>
                                <td><a>2</a></td>
                                <td><input type='edit' id='rmip1' /></td>
                                <td><input type='checkbox' id='rmen1' onclick='DisableElements()' /></td>
                            </tr>
                            <tr align='center'>
                                <td><a>3</a></td>
                                <td><input type='edit' id='rmip2' /></td>
                                <td><input type='checkbox' id='rmen2' onclick='DisableElements()' /></td>
                            </tr>
                            <tr align='center'>
                                <td><a>4</a></td>
                                <td><input type='edit' id='rmip3' /></td>
                                <td><input type='checkbox' id='rmen3' onclick='DisableElements()' /></td>
                            </tr>
                            <tr align='center'>
                                <td><a>5</a></td>
                                <td><input type='edit' id='rmip4' /></td>
                                <td><input type='checkbox' id='rmen4' onclick='DisableElements()' /></td>
                            </tr>
                        </table>
                    </td>
                    <td>
                        <table border='0' cellpadding='4' cellspacing='0'>
                            <td colspan="3" align='center'>
                                <font color='green'><h3>Light</h3></font>
                            </td>
                            <tr>
                                <td align='center'></td>
                                <td align='center'><b>IP</b></td>
                                <td align='center'><b>Enabled</b></td>
                            </tr>
                            <tr align='center'>
                                <td align='center'></td>
                                <td><input type='edit' id='rmlip0' /></td>
                                <td><input type='checkbox' id='rmlen0' onclick='DisableElements()' /></td>
                            </tr>
                            <tr align='center'>
                                <td align='center'></td>
                                <td><input type='edit' id='rmlip1' /></td>
                                <td><input type='checkbox' id='rmlen1' onclick='DisableElements()' /></td>
                            </tr>
                            <tr align='center'>
                                <td align='center'></td>
                                <td><input type='edit' id='rmlip2' /></td>
                                <td><input type='checkbox' id='rmlen2' onclick='DisableElements()' /></td>
                            </tr>
                            <tr align='center'>
                                <td align='center'></td>
                                <td><input type='edit' id='rmlip3' /></td>
                                <td><input type='checkbox' id='rmlen3' onclick='DisableElements()' /></td>
                            </tr>
                            <tr align='center'>
                                <td align='center'></td>
                                <td><input type='edit' id='rmlip4' /></td>
                                <td><input type='checkbox' id='rmlen4' onclick='DisableElements()' /></td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
            <table border='0' cellpadding='4' cellspacing='0'>
                <tr align='center'>
                    <td colspan='2'>
                        <div id='answ'></div>
                    </td>
                </tr>
                <tr align='center'>
                    <td align='center' colspan="2">
                        <input type='button' onclick='SaveConfigs()' value='Save Configs'></input>
                    </td>
                </tr>
            </table>
        </center>
    </body>
    <script>
        window.onload = async function() {
            let tdStatus = document.querySelector('#status')
            let tdAlarm = document.querySelector('#alarm')
            let aLastKey = document.querySelector('#lastKey')
            let keys = document.querySelector('#keys')
            let gpioLED = document.querySelector('#gpioLED')
            let gpioAlarm = document.querySelector('#gpioAlarm')
            let gpioKey = document.querySelector('#gpioKey')
            let cbInverted = document.querySelector('#cbInverted')
            let cbMaster = document.querySelector("#master");
            let cbSlave = document.querySelector("#slave");

            await fetch('/api/v1/gpio/info').then(function(resp) {
                    return resp.json();
                }).then(function(json) {
                    let gpios = [
                        "#gpioKey", "#gpioAlarm", "#gpioLED", "#gpio0", "#gpio1",
                        "#gpio2", "#gpio3", "#gpio4", "#gpio5", "#gpio6", "#gpio7",
                         "#gpio8", "#gpio9"
                    ];
                    for (let i = 0; i < json.length; i++) {
                        for (let j = 0; j < gpios.length; j++) {
                            let op = document.createElement("option")
                            op.value = json[i]
                            op.text = json[i]
                            let cbGpio = document.querySelector(gpios[j])
                            cbGpio.add(op);
                        }
                    }
                })
            await fetch('/api/v1/secure/types').then(function(resp) {
                    return resp.json();
                }).then(function(json) {
                    for (let i = 0; i < json.types.length; i++) {
                        for (let j = 0; j < 10; j++) {
                            let op = document.createElement("option")
                            op.value = json.types[i]
                            op.text = json.types[i]
                            let cbGpio = document.querySelector("#type"+j)
                            cbGpio.add(op);
                        }
                    }
                })
            await fetch('/api/v1/secure/info').then(function(resp) {
                    return resp.json();
                }).then(function(json) {
                    if (json.armed) {
                        tdStatus.innerHTML = "<font color='green'><a>Armed</a></font>";
                    } else {
                        tdStatus.innerHTML = "<font color='red'><a>Disarmed</a></font>";
                    }
                    if (json.alarm) {
                        tdAlarm.innerHTML = "<font color='green'><a>On</a></font>";
                    } else {
                        tdAlarm.innerHTML = "<font color='red'><a>Off</a></font>";
                    }
                    aLastKey.innerHTML = json.last_key;
                    cbInverted.checked = json.inverted;
                    for (let i = 0; i < gpioLED.children.length; i++) {
                        let item = gpioLED.children[i]
                        if (item.value == json.pins.led) {
                            item.selected = true
                            break
                        }
                    }
                    for (let i = 0; i < gpioKey.children.length; i++) {
                        let item = gpioKey.children[i]
                        if (item.value == json.pins.key) {
                            item.selected = true
                            break
                        }
                    }
                    for (let i = 0; i < gpioAlarm.children.length; i++) {
                        let item = gpioAlarm.children[i]
                        if (item.value == json.pins.alarm) {
                            item.selected = true
                            break
                        }
                    }
                    for (let i = 0; i < json.keys.length; i++) {
                        let op = document.createElement("option")
                        op.value = json.keys[i]
                        op.text = json.keys[i]
                        keys.add(op)
                    }
                    for (let i = 0; i < json.sensors.length; i++) {
                        let edName = document.querySelector("#name"+i);
                        let cbSms = document.querySelector("#sms"+i);
                        let cbTg = document.querySelector("#tg"+i);
                        let cbEnable = document.querySelector("#enable"+i);
                        let cbAlarm = document.querySelector("#alrm"+i);
                        let cmbType = document.querySelector("#type"+i);
                        let cmbGpio = document.querySelector("#gpio"+i);

                        edName.value = json.sensors[i].name
                        cbSms.checked = json.sensors[i].sms;
                        cbTg.checked = json.sensors[i].telegram;
                        cbEnable.checked = json.sensors[i].enabled;
                        cbAlarm.checked = json.sensors[i].alarm;

                        for (let j = 0; j < cmbType.children.length; j++) {
                            let item = cmbType.children[j]
                            if (item.value == json.sensors[i].type) {
                                item.selected = true
                                break
                            }
                        }
                        for (let j = 0; j < cmbGpio.children.length; j++) {
                            let item = cmbGpio.children[j]
                            if (item.value == json.sensors[i].pin) {
                                item.selected = true
                                break
                            }
                        }
                    }
                    for (let i = 0; i < json.remote.length; i++) {
                        let edName = document.querySelector("#rmip"+i);
                        let cbEnable = document.querySelector("#rmen"+i);

                        edName.value = json.remote[i].ip
                        cbEnable.checked = json.remote[i].enabled
                    }
                    for (let i = 0; i < json.light.length; i++) {
                        let edName = document.querySelector("#rmlip"+i);
                        let cbEnable = document.querySelector("#rmlen"+i);

                        edName.value = json.light[i].ip
                        cbEnable.checked = json.light[i].enabled
                    }
                    if (json.master) {
                        cbMaster.checked = true
                        cbSlave.checked = false
                    } else {
                        cbMaster.checked = false
                        cbSlave.checked = true
                    }
                    DisableElements()
                })
        };

        function Arm(stat) {
            fetch('/api/v1/secure/arm?'+ new URLSearchParams({
                        status: stat
                    })).then(function(resp) {
                return resp.json();
            }).then(function(json) {
                if (json.result) {
                    location.reload();
                }
            })
        }

        function AddKey() {
            let edKey = document.querySelector("#newKey")
            fetch('/api/v1/secure/key/add?'+ new URLSearchParams({
                        key: edKey.value
                    })).then(function(resp) {
                return resp.json();
            }).then(function(json) {
                edKey.value = ""
                if (json.result) {
                    location.reload();
                }
            })
        }

        function ClearKeys() {
            let edKey = document.querySelector("#newKey")
            fetch('/api/v1/secure/key/clear').then(function(resp) {
                return resp.json();
            }).then(function(json) {
                if (json.result) {
                    location.reload();
                }
            })
        }

        function DisableElements() {
            let cbMaster = document.querySelector("#master");
            for (let i = 0; i < 5; i++) {
                let edName = document.querySelector("#rmip"+i);
                let cbEnable = document.querySelector("#rmen"+i);
                let edLightName = document.querySelector("#rmlip"+i);
                let cbLightEnable = document.querySelector("#rmlen"+i);

                if (cbMaster.checked) {
                    edName.disabled = !cbEnable.checked
                    cbEnable.disabled = false
                    edLightName.disabled = !cbLightEnable.checked
                    cbLightEnable.disabled = false
                } else {
                    edName.disabled = true
                    cbEnable.disabled = true
                    edLightName.disabled = true
                    cbLightEnable.disabled = true
                }
            }
            for (let i = 0; i < 10; i++) {
                let edName = document.querySelector('#name'+i)
                let cmbType = document.querySelector('#type'+i)
                let cmbGpio = document.querySelector('#gpio'+i)
                let cbSms = document.querySelector('#sms'+i)
                let cbTg = document.querySelector('#tg'+i)
                let cbEnable = document.querySelector('#enable'+i)
                let cbAlarm = document.querySelector("#alrm"+i);

                edName.disabled = !cbEnable.checked
                cmbType.disabled = !cbEnable.checked
                cmbGpio.disabled = !cbEnable.checked
                cbSms.disabled = !cbEnable.checked
                cbTg.disabled = !cbEnable.checked
                cbAlarm.disabled = !cbEnable.checked
            }
        }

        function SaveConfigs() {
            let gpioLED = document.querySelector('#gpioLED')
            let gpioAlarm = document.querySelector('#gpioAlarm')
            let gpioKey = document.querySelector('#gpioKey')
            let cbInverted = document.querySelector('#cbInverted')
            let cbMaster = document.querySelector("#master");

            let remote = []
            let light = []
            for (let i = 0; i < 5; i++) {
                let edName = document.querySelector("#rmip"+i);
                let cbEnable = document.querySelector("#rmen"+i);
                let edLightName = document.querySelector("#rmlip"+i);
                let cbLightEnable = document.querySelector("#rmlen"+i);

                remote.push({
                    "ip": edName.value,
                    "enabled": cbEnable.checked
                });
                light.push({
                    "ip": edLightName.value,
                    "enabled": cbLightEnable.checked
                });
            }

            let sensors = [];
            for (let i = 0; i < 10; i++) {
                let edName = document.querySelector('#name'+i)
                let cmbType = document.querySelector('#type'+i)
                let cmbGpio = document.querySelector('#gpio'+i)
                let cbSms = document.querySelector('#sms'+i)
                let cbTg = document.querySelector('#tg'+i)
                let cbEnable = document.querySelector('#enable'+i)
                let cbAlarm = document.querySelector("#alrm"+i);

                sensors.push({
                        "name": edName.value,
                        "enabled": cbEnable.checked,
                        "sms": cbSms.checked,
                        "telegram": cbTg.checked,
                        "type": cmbType.value,
                        "alarm": cbAlarm.checked,
                        "pin": cmbGpio.value
                    })
            }

            let data = {
                "inverted": cbInverted.checked,
                "pins": {
                    "alarm": gpioAlarm.value,
                    "key": gpioKey.value,
                    "led": gpioLED.value
                },
                "sensors": sensors,
                "remote": remote,
                "light": light,
                "master": cbMaster.checked
            }

            fetch("/api/v1/secure/conf", {
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

#endif /* SECURE_MOD */

#endif /* __SECURE_HTML_HPP__ */
