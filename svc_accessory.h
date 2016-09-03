/* 
 * The MIT License (MIT)
 * 
 * ESP8266 Non-OS Firmware
 * Copyright (c) 2015 Michael Jacobsen (github.com/mikejac)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef SVC_ACCESSORY_H
#define	SVC_ACCESSORY_H

#include "svc_services.h"

#ifdef	__cplusplus
extern "C" {
#endif

/******************************************************************************************************************
 * 
 *
 */
    
typedef struct Container Container;

/******************************************************************************************************************
 * 
 *
 */

typedef struct Accessory Accessory;

struct Accessory {
    sint64_t                ID;                 // "iid"
    int                     Type;               // "type"
    Service*                Service;            // "services"
    
    int                     idCount;
    Accessory*              next;
    Container*              parent;
};

/******************************************************************************************************************
 * 
 *
 */

#define AccOutletOnSetValue(a, v)               BoolSetValue((a)->Outlet->On->Bool, (v))
#define AccOutletOutletInUseSetValue(a, v)      BoolSetValue((a)->Outlet->OutletInUse->Bool, (v))

#define AccOutletOnGetValue(a)                  ((a)->Outlet->On->Bool->Value->Bool)
#define AccOutletOutletInUseGetValue(a)         ((a)->Outlet->OutletInUse->Bool->Value->Bool)

typedef struct {
    Accessory*  Accessory;
    
    Outlet*     Outlet;
} AccOutlet;
/**
 * 
 * @param name
 * @param serialnumber
 * @param manufacturer
 * @param model
 * @return 
 */
AccOutlet* NewAccOutlet(const char* name, const char* serialnumber, const char* manufacturer, const char* model);

///

#define AccThermometerCurrentTemperatureSetValue(a, v)               FloatSetValue((a)->TemperatureSensor->CurrentTemperature->Float, (v))
#define AccThermometerCurrentTemperatureGetValue(a)                  ((a)->TemperatureSensor->CurrentTemperature->Float->Value->Float)

typedef struct {
    Accessory*          Accessory;
    
    TemperatureSensor*  TemperatureSensor;
} AccThermometer;
/**
 * 
 * @param name
 * @param serialnumber
 * @param manufacturer
 * @param model
 * @param temp
 * @param min
 * @param max
 * @param steps
 * @return 
 */
AccThermometer* NewAccThermometer(  const char* name, 
                                    const char* serialnumber, 
                                    const char* manufacturer, 
                                    const char* model,
                                    double      temp,
                                    double      min,
                                    double      max,
                                    double      steps);

///

#define AccHumidityCurrentRelativeHumiditySetValue(a, v)               FloatSetValue((a)->HumiditySensor->CurrentRelativeHumidity->Float, (v))
#define AccHumidityCurrentRelativeHumidityGetValue(a)                  ((a)->HumiditySensor->CurrentRelativeHumidity->Float->Value->Float)

typedef struct {
    Accessory*      Accessory;
    
    HumiditySensor* HumiditySensor;
} AccHumidity;
/**
 * 
 * @param name
 * @param serialnumber
 * @param manufacturer
 * @param model
 * @param hum
 * @param min
 * @param max
 * @param steps
 * @return 
 */
AccHumidity* NewAccHumidity(const char* name, 
                            const char* serialnumber, 
                            const char* manufacturer, 
                            const char* model,
                            double      hum,
                            double      min,
                            double      max,
                            double      steps);

///

#define AccThermostatCurrentTemperatureSetValue(a, v)               FloatSetValue((a)->Thermostat->CurrentTemperature->Float, (v))
#define AccThermostatCurrentTemperatureGetValue(a)                  ((a)->Thermostat->CurrentTemperature->Float->Value->Float)
#define AccThermostatTargetTemperatureSetValue(a, v)                FloatSetValue((a)->Thermostat->TargetTemperature->Float, (v))
#define AccThermostatTargetTemperatureGetValue(a)                   ((a)->Thermostat->TargetTemperature->Float->Value->Float)

#define AccThermostatCurrentHeatingCoolingStateSetValue(a, v)       UInt8SetValue((a)->Thermostat->CurrentHeatingCoolingState->UInt8, (v))
#define AccThermostatCurrentHeatingCoolingStateGetValue(a)          ((a)->Thermostat->CurrentHeatingCoolingState->UInt8->Value->UInt8)
#define AccThermostatTargetHeatingCoolingStateSetValue(a, v)        UInt8SetValue((a)->Thermostat->TargetHeatingCoolingState->UInt8, (v))
#define AccThermostatTargetHeatingCoolingStateGetValue(a)           ((a)->Thermostat->TargetHeatingCoolingState->UInt8->Value->UInt8)

//#define AccThermostatCurrentHeatingCoolingStateIid
#define AccThermostatTargetHeatingCoolingStateIid                   9
#define AccThermostatTargetTemperatureIid                           11

typedef struct {
    Accessory*  Accessory;
    
    Thermostat* Thermostat;
} AccThermostat;
/**
 * 
 * @param name
 * @param serialnumber
 * @param manufacturer
 * @param model
 * @param currentTemp
 * @param currentMin
 * @param currentMax
 * @param currentSteps
 * @param targetTemp
 * @param targetMin
 * @param targetMax
 * @param targetSteps
 * @return 
 */
AccThermostat* NewAccThermostat(const char* name, 
                                const char* serialnumber, 
                                const char* manufacturer, 
                                const char* model,
                                double      currentTemp,
                                double      currentMin,
                                double      currentMax,
                                double      currentSteps,
                                double      targetTemp,
                                double      targetMin,
                                double      targetMax,
                                double      targetSteps);

///

#define AccTextSetValue(a, v)   StringSetValue((a)->Text->Version->String, (v))

typedef struct {
    Accessory*  Accessory;
    
    Text*       Text;
} AccText;
/**
 * 
 * @param name
 * @param serialnumber
 * @param manufacturer
 * @param model
 * @param text
 * @return 
 */
AccText* NewAccText(const char* name, const char* serialnumber, const char* manufacturer, const char* model, const char* text);

/******************************************************************************************************************
 * prototypes
 *
 */

/**
 * 
 * @param name
 * @param serialnumber
 * @param manufacturer
 * @param model
 * @param typ
 * @return 
 */
Accessory* NewAccessory(const char* name, const char* serialnumber, const char* manufacturer, const char* model, int typ);
/**
 * 
 * @param a
 * @param s
 */
void AddService(Accessory* a, Service* s);
/**
 * 
 * @param a
 * @param iid
 * @return 
 */
Characteristic* FindCharacteristicByIid(Accessory* a, sint64_t iid);

/******************************************************************************************************************
 * 
 *
 */

// accessory types
#define TypeAccOther              1
#define TypeAccBridge             2
#define TypeAccFan                3
#define TypeAccGarageDoorOpener   4
#define TypeAccLightbulb          5
#define TypeAccDoorLock           6
#define TypeAccOutlet             7
#define TypeAccSwitch             8
#define TypeAccThermostat         9
#define TypeAccSensor             10
#define TypeAccAlarmSystem        11
#define TypeAccDoor               12
#define TypeAccWindow             13
#define TypeAccWindowCovering     14
#define TypeAccProgrammableSwitch 15
#define TypeAccRangeExtender      16

#ifdef	__cplusplus
}
#endif

#endif	/* SVC_ACCESSORY_H */

