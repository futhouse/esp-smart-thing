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

#include "modules/secure.hpp"
#include "utils.hpp"
#include "net/client.hpp"
#include "net/api/api.hpp"

#define SECURE_TYPES_COUNT  3
#define SECURE_TYPES_LEN    11

const PROGMEM char secureTypes[SECURE_TYPES_COUNT][SECURE_TYPES_LEN] = {
    "ReedSwitch",
    "PIR",
    "MicroWave"
};

Secure::Secure(const std::shared_ptr<ILogger>& log,
               const std::shared_ptr<IGpio>& gpio,
               const std::shared_ptr<ITelegram>& tg,
               const std::shared_ptr<ISms>& sms,
               const std::shared_ptr<IFlash>& flash
               ):
    _log(move(log)),
    _gpio(move(gpio)),
    _tg(move(tg)),
    _sms(move(sms)),
    _flash(move(flash))
{
    for (uint8_t i = 0; i < CONFIG_SECURE_KEYS_COUNT; i++)
        _keys[i] = "";
}

#ifdef SECURE_MOD

void Secure::setup()
{
    _tickMain.begin(std::bind(&Secure::handleMain, this), SECURE_MAIN_DELAY_MS, 0, MILLIS);
    _tickKey.begin(std::bind(&Secure::handleKey, this), SECURE_KEYS_DELAY_MS, 0, MILLIS);
    _tickKey.start();
}

void Secure::loop()
{
    _tickMain.update();
    _tickKey.update();
}

void Secure::setPin(SecurePins pinType, const GpioPin &pin)
{
    switch (pinType)
    {
        case SECURE_ALARM_PIN:
            _pins[SECURE_ALARM_PIN] = pin;
            break;

        case SECURE_KEY_PIN:
            _pins[SECURE_KEY_PIN] = pin;
            break;

        case SECURE_LED_PIN:
            _pins[SECURE_LED_PIN] = pin;
            break;

        default:
            _log->error("SECURE", "Wrong PIN");
    }
}

GpioPin& Secure::getPin(SecurePins pinType)
{
    switch (pinType)
    {
        case SECURE_ALARM_PIN:
            return _pins[SECURE_ALARM_PIN];

        case SECURE_KEY_PIN:
            return _pins[SECURE_KEY_PIN];

        case SECURE_LED_PIN:
            return _pins[SECURE_LED_PIN];

        default:
            _log->error("SECURE", "Wrong PIN");
    }

    return _pins[SECURE_ALARM_PIN];
}

String Secure::typeToStr(SecureType sensType)
{
    return String(secureTypes[sensType]);
}

void Secure::getTypes(std::vector<String> &types)
{
    for (uint8_t i = 0; i < SECURE_TYPES_COUNT; i++)
        types.push_back(String(secureTypes[i]));
}

SecureType Secure::strToType(const String& str)
{
    for (uint8_t i = 0; i < SECURE_TYPES_COUNT; i++) {
        if (str == String(secureTypes[i]))
            return static_cast<SecureType>(i);
    }
    return SECURE_REEDSWITCH_TYPE;
}

SecureSensor* Secure::getSensors()
{
    return _sensors;
}

String* Secure::getKeys()
{
    return _keys;
}

void Secure::setInvertAlarm(bool inverted)
{
    _invertAlarm = inverted;
}

bool Secure::getInvertAlarm(void)
{
    return _invertAlarm;
}

void Secure::clearKeys()
{
    for (uint8_t i = 0; i < CONFIG_SECURE_KEYS_COUNT; i++)
        _keys[i] = "";
}

bool Secure::getAlarm()
{
    return _alarm;
}

void Secure::setAlarm(bool status)
{
    _alarm = status;

    if (_alarm) {
        if (_invertAlarm) {
            _gpio->setPinState(_pins[SECURE_ALARM_PIN], GPIO_LOW);
        } else {
            _gpio->setPinState(_pins[SECURE_ALARM_PIN], GPIO_HIGH);
        }
    } else {
        if (_invertAlarm) {
            _gpio->setPinState(_pins[SECURE_ALARM_PIN], GPIO_HIGH);
        } else {
            _gpio->setPinState(_pins[SECURE_ALARM_PIN], GPIO_LOW);
        }
    }

    if (_master) {
        for (uint8_t i = 0; i < CONFIG_SECURE_REMOTE_COUNT; i++) {
            if (_remote[i].Enabled && (_remote[i].IP != "")) {
                if (sendRemoteStatus(SECURE_REMOTE_ALARM_CMD, _remote[i].IP, status)) {
                    _log->info("SECURE", "Send alarm Status:\""+String(status)+"\" to remote dev IP: \""+_remote[i].IP+"\"");
                } else {
                    _log->error("SECURE", "Failed to send alarm to remote dev IP: \""+_remote[i].IP+"\"");
                }
            }
        }
        for (uint8_t i = 0; i < CONFIG_SECURE_REMOTE_COUNT; i++) {
            if (_light[i].Enabled && (_light[i].IP != "")) {
                if (sendRemoteStatus(SECURE_REMOTE_LIGHT_ON_CMD, _light[i].IP, status)) {
                    _log->info("SECURE", "Send Light ON:\""+String(status)+"\" to remote dev IP: \""+_light[i].IP+"\"");
                } else {
                    _log->error("SECURE", "Failed to send Light ON to remote dev IP: \""+_light[i].IP+"\"");
                }
            }
        }
    }
}

bool Secure::getArmed()
{
    return _armed;
}

String& Secure::getLastKey()
{
    return _lastKey;
}

void Secure::setArmed(bool status, bool save=false)
{
    _armed = status;

    if (!_armed) {
        _tickMain.stop();
        _gpio->setPinState(_pins[SECURE_LED_PIN], GPIO_HIGH);
        setAlarm(false);
        _log->info("SECURE", "Security system was disarmed");
    } else {
        _tickMain.start();
        _gpio->setPinState(_pins[SECURE_LED_PIN], GPIO_LOW);
        _log->info("SECURE", "Security system was armed");
    }

    if (save) {
        saveStates();
    }

    if (_master) {
        for (uint8_t i = 0; i < CONFIG_SECURE_REMOTE_COUNT; i++) {
            if (_remote[i].Enabled && (_remote[i].IP != "")) {
                if (sendRemoteStatus(SECURE_REMOTE_ARM_CMD, _remote[i].IP, status)) {
                    _log->info("SECURE", "Send status \""+String(status)+"\" to remote dev \""+_remote[i].IP+"\"");
                } else {
                    _log->error("SECURE", "Failed to send status to remote dev \""+_remote[i].IP+"\"");
                }
            }
        }
    }
}

void Secure::handleMain()
{
    GpioState state;

    for (uint8_t i = 0; i < CONFIG_SECURE_SENSORS_COUNT; i++) {
        if (_sensors[i].Enabled) {
            state = _gpio->readState(_sensors[i].Pin);
            switch (_sensors[i].Type)
            {
                case SECURE_REEDSWITCH_TYPE:
                case SECURE_MICROWAVE_TYPE:
                    if ((state != _sensors[i].LastState) && (state == GPIO_LOW))
                        runAlarm(_sensors[i]);
                    break;

                case SECURE_PIR_TYPE:
                    if ((state != _sensors[i].LastState) && (state == GPIO_HIGH))
                        runAlarm(_sensors[i]);
                    break;
            }
            _sensors[i].LastState = state;
        }
    }
}

void Secure::runAlarm(const SecureSensor& sensor)
{
    String notify;

    if (sensor.Alarm && !_alarm) {
        _log->info("SECURE", "Alarm running!");
        setAlarm(true);
        saveStates();
    }

    auto cfg = _flash->getConfigs();

    notify = String(cfg->DevName) + ": Sensor \"" + sensor.Name + "\"";
    switch (sensor.Type)
    {
        case SECURE_REEDSWITCH_TYPE:
            notify += " is openned!";
            break;

        case SECURE_MICROWAVE_TYPE:
        case SECURE_PIR_TYPE:
            notify += " motion detected!";
            break;
    }

    _log->info("SECURE", notify);

#ifdef TELEGRAM_NOTIFY_MOD
    if (sensor.Telegram)
        _tg->sendNotify(notify);
#endif
#ifdef SMS_NOTIFY_MOD
    if (sensor.Sms)
        _sms->sendMsg(notify);
#endif
}

bool Secure::verifyKey(const String &key)
{
    for (uint8_t i = 0; i < CONFIG_SECURE_KEYS_COUNT; i++) {
        if (key == _keys[i]) {
            setArmed(!getArmed(), true);
            delay(2000);
            return true;
        }
    }
    return false;
}

void Secure::handleKey()
{
    uint8_t key[CONFIG_SECURE_KEY_LEN];
    char strKey[CONFIG_SECURE_KEY_LEN];

    if (_oneWire.search(key)) {
        HexArrayToStr(key, CONFIG_SECURE_KEY_LEN, strKey);

        _lastKey = String(strKey);
        _log->info("SECURE", "User detected with key: \"" + _lastKey + "\"");

        if (!verifyKey(_lastKey)) {
            _log->info("SECURE", "Key not allowed");
        } else {
            _log->info("SECURE", "Key allowed");
        }
    }

    _oneWire.reset();
}

bool Secure::saveStates()
{
    _log->info("SECURE", "Saving Security configs");

    auto cfg = _flash->getConfigs();

    cfg->SecureCfg.Armed = _armed;
    cfg->SecureCfg.Alarm = _alarm;
    cfg->SecureCfg.InvertedAlarm = _invertAlarm;

    cfg->SecureCfg.AlarmPin = {
        Type: static_cast<uint8_t>(_pins[SECURE_ALARM_PIN].Type),
        Addr: _pins[SECURE_ALARM_PIN].Addr,
        Pin: _pins[SECURE_ALARM_PIN].Pin
    };

    cfg->SecureCfg.KeyPin = {
        Type: static_cast<uint8_t>(_pins[SECURE_KEY_PIN].Type),
        Addr: _pins[SECURE_KEY_PIN].Addr,
        Pin: _pins[SECURE_KEY_PIN].Pin
    };

    cfg->SecureCfg.LedPin = {
        Type: static_cast<uint8_t>(_pins[SECURE_LED_PIN].Type),
        Addr: _pins[SECURE_LED_PIN].Addr,
        Pin: _pins[SECURE_LED_PIN].Pin
    };

    for (uint8_t i = 0; i < CONFIG_SECURE_KEYS_COUNT; i++) {
        strncpy(cfg->SecureCfg.Keys[i], _keys[i].c_str(), CONFIG_SECURE_KEY_LEN);
        cfg->SecureCfg.Keys[i][CONFIG_SECURE_KEY_LEN - 1] = '\0';
    }

    for (uint8_t i = 0; i < CONFIG_SECURE_SENSORS_COUNT; i++) {
        strncpy(cfg->SecureCfg.Sensors[i].Name, _sensors[i].Name.c_str(), 10);
        cfg->SecureCfg.Sensors[i].Enabled = _sensors[i].Enabled;
        cfg->SecureCfg.Sensors[i].Sms = _sensors[i].Sms;
        cfg->SecureCfg.Sensors[i].Telegram = _sensors[i].Telegram;
        cfg->SecureCfg.Sensors[i].Type = _sensors[i].Type;
        cfg->SecureCfg.Sensors[i].Alarm = _sensors[i].Alarm;
        cfg->SecureCfg.Sensors[i].Pin = {
            Type: static_cast<uint8_t>(_sensors[i].Pin.Type),
            Addr: _sensors[i].Pin.Addr,
            Pin: _sensors[i].Pin.Pin
        };
    }

    for (uint8_t i = 0; i < CONFIG_SECURE_REMOTE_COUNT; i++) {
        strncpy(cfg->SecureCfg.Remote[i].IP, _remote[i].IP.c_str(), CONFIG_IP_LEN);
        cfg->SecureCfg.Remote[i].IP[CONFIG_IP_LEN - 1] = '\0';
        cfg->SecureCfg.Remote[i].Enabled = _remote[i].Enabled;
    }

    for (uint8_t i = 0; i < CONFIG_SECURE_REMOTE_COUNT; i++) {
        strncpy(cfg->SecureCfg.Light[i].IP, _light[i].IP.c_str(), CONFIG_IP_LEN);
        cfg->SecureCfg.Light[i].IP[CONFIG_IP_LEN - 1] = '\0';
        cfg->SecureCfg.Light[i].Enabled = _light[i].Enabled;
    }

    cfg->SecureCfg.Master = getMaster();

    return _flash->saveData();
}

void Secure::loadStates()
{
    _log->info("SECURE", "Loading Security configs");

    auto& secCfg = _flash->getConfigs()->SecureCfg;

    _lastKey = "None";
    setInvertAlarm(secCfg.InvertedAlarm);

    _log->info("SECURE", "Armed: \"" + String(secCfg.Armed) + "\"");
    _log->info("SECURE", "Alarm: \"" + String(secCfg.Alarm) + "\"");
    _log->info("SECURE", "InvertedAlarm: \"" + String(secCfg.InvertedAlarm) + "\"");
    _log->info("SECURE", "Master: \"" + String(secCfg.Master) + "\"");

    setPin(SECURE_ALARM_PIN, { 
        Type: static_cast<GpioType>(secCfg.AlarmPin.Type),
        Addr: secCfg.AlarmPin.Addr,
        Pin: secCfg.AlarmPin.Pin 
    });

    setPin(SECURE_KEY_PIN, {
        Type: static_cast<GpioType>(secCfg.KeyPin.Type),
        Addr: secCfg.KeyPin.Addr,
        Pin: secCfg.KeyPin.Pin 
    });

    setPin(SECURE_LED_PIN, {
        Type: static_cast<GpioType>(secCfg.LedPin.Type),
        Addr: secCfg.LedPin.Addr,
        Pin: secCfg.LedPin.Pin 
    });

    for (uint8_t i = 0; i < CONFIG_SECURE_SENSORS_COUNT; i++) {
        const auto sensor = secCfg.Sensors[i];

        setSensor(i, { 
            Name: sensor.Name,
            Enabled: sensor.Enabled,
            Sms: sensor.Sms,
            Telegram: sensor.Telegram,
            Alarm: sensor.Alarm,
            Type: static_cast<SecureType>(sensor.Type),
            Pin: {
                Type: static_cast<GpioType>(sensor.Pin.Type),
                Addr: sensor.Pin.Addr,
                Pin: sensor.Pin.Pin
            },
            LastState: GPIO_NONE
        });

        if (String(sensor.Name) != "") {
            _log->info("SECURE", "Add sensor Name: \"" + String(sensor.Name) +
                "\" Type: \"" + typeToStr(static_cast<SecureType>(sensor.Type)) + 
                "\" Enabled: \"" + String(sensor.Enabled) + "\"");
        }
    }

    for (uint8_t i = 0; i < CONFIG_SECURE_KEYS_COUNT; i++) {
        setKey(i, secCfg.Keys[i]);
        if (String(secCfg.Keys[i]) != "")
            _log->info("SECURE", "Add new key: \"" + String(secCfg.Keys[i]) + "\"");
    }

    for (uint8_t i = 0; i < CONFIG_SECURE_REMOTE_COUNT; i++) {
        setRemote(i, {
            IP: String(secCfg.Remote[i].IP),
            Enabled: secCfg.Remote[i].Enabled
        });
        setLight(i, {
            IP: String(secCfg.Light[i].IP),
            Enabled: secCfg.Light[i].Enabled
        });
    }

    setMaster(secCfg.Master);

    _gpio->setPinMode(_pins[SECURE_ALARM_PIN], GPIO_OUTPUT);
    _gpio->setPinMode(_pins[SECURE_LED_PIN], GPIO_OUTPUT);
    _gpio->setPinMode(_pins[SECURE_KEY_PIN], GPIO_INPUT);

    if (_pins[SECURE_KEY_PIN].Type == GPIO_INTERNAL)
        _oneWire.begin(_pins[SECURE_KEY_PIN].Pin);
    else
        _log->error("SECURE", "iButton key cannot connect to external GPIO");
        
    setArmed(secCfg.Armed);
    setAlarm(secCfg.Alarm);
}

SecureRemoteDev* Secure::getRemoteDevices()
{
    return _remote;
}

SecureRemoteDev* Secure::getLightDevices()
{
    return _light;
}

bool Secure::sendRemoteStatus(SecureRemoteCmd cmd, const String &ip, bool status)
{
    NetClient client(NET_CLIENT_HTTP, ip);

    if (cmd == SECURE_REMOTE_ARM_CMD) {
        NetRequest req(API_SECURE_ARM);
        req.setArg("status", BoolToStr(status));
        return client.getRequest(req);
    }
    else if (cmd == SECURE_REMOTE_ALARM_CMD) {
        NetRequest req(API_SECURE_ALARM);
        req.setArg("status", BoolToStr(status));
        return client.getRequest(req);
    }
    else if (cmd == SECURE_REMOTE_LIGHT_ON_CMD) {
        NetRequest req(API_LIGHT_ALL);
        req.setArg("status", BoolToStr(status));
        return client.getRequest(req);
    }

    return false;
}

bool Secure::getMaster()
{
    return _master;
}

void Secure::setMaster(bool master)
{
    _master = master;
}

void Secure::setKey(size_t id, const String &key)
{
    _keys[id] = key;
}

void Secure::setSensor(size_t id, const SecureSensor &sens)
{
    _sensors[id] = sens;
}

void Secure::setRemote(size_t id, const SecureRemoteDev &dev)
{
    _remote[id] = dev;
}

void Secure::setLight(size_t id, const SecureRemoteDev &dev)
{
    _light[id] = dev;
}

#endif /* SECURE_MOD */
