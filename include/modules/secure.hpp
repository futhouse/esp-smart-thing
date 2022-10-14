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

#ifndef __SECURE_HPP__
#define __SECURE_HPP__

#include "custom/modules.hpp"
#include "gpio.hpp"
#include "logger.hpp"
#include "flash.hpp"
#include "modules/core/telegram.hpp"
#include "modules/core/sms.hpp"

#ifdef SECURE_MOD

#include <Arduino.h>
#include <Ticker.h>
#include <functional>
#include <vector>
#include <OneWire.h>

typedef enum _SecurePins
{
    SECURE_ALARM_PIN,
    SECURE_KEY_PIN,
    SECURE_LED_PIN
} SecurePins;

typedef enum _SecureType
{
    SECURE_REEDSWITCH_TYPE,
    SECURE_PIR_TYPE,
    SECURE_MICROWAVE_TYPE
} SecureType;

typedef struct _SecureSensor
{
    String      Name;
    bool        Enabled;
    bool        Sms;
    bool        Telegram;
    bool        Alarm;
    SecureType  Type;
    GpioPin     Pin;
    GpioState   LastState;
} SecureSensor;

typedef struct _SecureRemoteDev
{
    String  IP;
    bool    Enabled;
} SecureRemoteDev;

typedef enum _SecureRemoteCmd
{
    SECURE_REMOTE_ARM_CMD,
    SECURE_REMOTE_ALARM_CMD
} SecureRemoteCmd;
#endif /* SECURE_MOD */

class ISecure
{
#ifdef SECURE_MOD
public:
    virtual void setup() = 0;
    virtual void loop() = 0;
    virtual void setPin(SecurePins pinType, const GpioPin &pin) = 0;
    virtual GpioPin& getPin(SecurePins pinType) = 0;
    virtual void setArmed(bool status, bool save) = 0;
    virtual bool getAlarm() = 0;
    virtual void setAlarm(bool status) = 0;
    virtual bool getArmed() = 0;
    virtual String getLastKey() = 0;
    virtual void setInvertAlarm(bool inverted) = 0;
    virtual bool getInvertAlarm(void) = 0;
    virtual void addSensor(const SecureSensor& sensor) = 0;
    virtual void addKey(const String &key) = 0;
    virtual void clearKeys() = 0;
    virtual String typeToStr(SecureType sensType) = 0;
    virtual SecureType strToType(const String& str) = 0;
    virtual std::vector<SecureSensor>& getSensors() = 0;
    virtual std::vector<String>& getKeys() = 0;
    virtual void getTypes(std::vector<String> &types) = 0;
    virtual bool verifyKey(const String &key) = 0;
    virtual void addRemoteDevice(const SecureRemoteDev &dev) = 0;
    virtual std::vector<SecureRemoteDev>& getRemoteDevices() = 0;
    virtual bool saveStates() = 0;
    virtual void loadStates() = 0;
    virtual bool getMaster() = 0;
    virtual void setMaster(bool master) = 0;
#endif /* SECURE_MOD */
};

class Secure : public ISecure
{
public:
    Secure(const std::shared_ptr<ILogger>& log,
           const std::shared_ptr<IGpio>& gpio,
           const std::shared_ptr<ITelegram>& tg,
           const std::shared_ptr<ISms>& sms,
           const std::shared_ptr<IFlash>& flash);
#ifdef SECURE_MOD
    /**
     * @brief First initialization
     * 
     */
    void setup();

    /**
     * @brief Timer loop
     * 
     */
    void loop();

    /**
     * @brief Set the Pin object by type
     * 
     * @param pinType
     * @param pin 
     */
    void setPin(SecurePins pinType, const GpioPin &pin);

    /**
     * @brief Get the Pin object by type
     * 
     * @param pinType
     */
    GpioPin& getPin(SecurePins pinType);

    /**
     * @brief Set the Armed status
     * 
     * @param status Armed status
     */
    void setArmed(bool status, bool save);

    /**
     * @brief Get the Alarm status
     * 
     * @return true 
     * @return false 
     */
    bool getAlarm();

    /**
     * @brief Set the Alarm status
     * 
     * @return true 
     * @return false 
     */
    void setAlarm(bool status);

    /**
     * @brief Get the Armed status
     * 
     * @return true 
     * @return false 
     */
    bool getArmed();

    /**
     * @brief Get the Last detected Key
     * 
     * @return String Key
     */
    String getLastKey();

    /**
     * @brief Get the Types of security sensor
     * 
     * @param types 
     */
    void getTypes(std::vector<String> &types);

    /**
     * @brief Set the Invert Alarm status
     * 
     * @param inverted Is inverted LED
     */
    void setInvertAlarm(bool inverted);

    /**
     * @brief Get the Invert Alarm status
     * 
     */
    bool getInvertAlarm(void);

    /**
     * @brief Add new Sensor object
     * 
     * @param sensor Secure sensor
     */
    void addSensor(const SecureSensor& sensor);

    /**
     * @brief Add new security key
     * 
     * @param key Add iButton serial number
     */
    void addKey(const String &key);

    /**
     * @brief Remove all keys
     * 
     */
    void clearKeys();

    /**
     * @brief Sensor type to string
     * 
     * @param pinType 
     * @return String 
     */
    String typeToStr(SecureType sensType);

    /**
     * @brief Conver string to type
     * 
     * @param str 
     * @return SecureType 
     */
    SecureType strToType(const String& str);

    /**
     * @brief Get the Sensors object
     * 
     * @return std::vector<SecureSensor>& 
     */
    std::vector<SecureSensor>& getSensors();

    /**
     * @brief Get the Keys object
     * 
     * @return std::vector<String>& 
     */
    std::vector<String>& getKeys();

    /**
     * @brief Save states to EEPROM
     * 
     * @return true 
     * @return false 
     */
    bool saveStates();

    /**
     * @brief Loading states from EEPROM
     * 
     */
    void loadStates();

    /**
     * @brief Verify key
     * 
     * @param key 
     */
    bool verifyKey(const String &key);

    /**
     * @brief Add new remote security module
     * 
     * @param dev Security module
     */
    void addRemoteDevice(const SecureRemoteDev &dev);

    /**
     * @brief Get the Remote Devices modules
     * 
     * @return std::vector<SecureRemoteDev>& 
     */
    std::vector<SecureRemoteDev>& getRemoteDevices();

    /**
     * @brief Get master status of module
     * 
     * @return true 
     * @return false 
     */
    bool getMaster();
    
    /**
     * @brief Set master status of module
     * 
     * @param master 
     */
    void setMaster(bool master);

#endif /* SECURE_MOD */
private:
    const std::shared_ptr<ILogger> _log;
    const std::shared_ptr<IGpio> _gpio;
    const std::shared_ptr<ITelegram> _tg;
    const std::shared_ptr<ISms> _sms;
    const std::shared_ptr<IFlash> _flash;

#ifdef SECURE_MOD
    Ticker _tickMain;
    Ticker _tickKey;
    bool _armed = false;
    bool _alarm = false;
    bool _invertAlarm = false;
    GpioPin _pinAlarm;
    GpioPin _pinKey;
    GpioPin _pinLed;
    std::vector<SecureSensor> _sensors;
    std::vector<String> _keys;
    std::vector<SecureRemoteDev> _remote;
    OneWire _oneWire;
    String _lastKey = "None";
    bool _master = false;

    void handleMain();
    void handleKey();
    void runAlarm(const SecureSensor& sensor);
    bool sendRemoteStatus(SecureRemoteCmd cmd, const String &ip, bool status);
#endif /* SECURE_MOD */
};

#endif /* __SECURE_HPP__ */
