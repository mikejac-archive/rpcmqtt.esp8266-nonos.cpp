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

#ifndef SVC_SERVICES_H
#define	SVC_SERVICES_H

#include "svc_characteristics.h"

#ifdef	__cplusplus
extern "C" {
#endif

/******************************************************************************************************************
 * 
 *
 */

typedef struct Accessory Accessory;

/******************************************************************************************************************
 * 
 *
 */

typedef struct Service Service;
    
struct Service {
    sint64_t                ID;                 // "iid"
    const char*             Type;               // "type"
    Characteristic*         Characteristics;	// "characteristics"
    
    Service*                next;
    Accessory*              parent;
};

/******************************************************************************************************************
 * 
 *
 */

typedef struct {
    Service*        Service;

    On*             On;
    OutletInUse*    OutletInUse;
} Outlet;
/**
 * 
 * @return 
 */
Outlet* NewOutlet(void);

///

typedef struct {
    Service*            Service;

    CurrentTemperature* CurrentTemperature;
} TemperatureSensor;
/**
 * 
 * @return 
 */
TemperatureSensor* NewTemperatureSensor(void);

///

typedef struct {
    Service*            Service;

    CurrentRelativeHumidity* CurrentRelativeHumidity;
} HumiditySensor;
/**
 * 
 * @return 
 */
HumiditySensor* NewHumiditySensor(void);

///

typedef struct {
    Service*                    Service;

    CurrentHeatingCoolingState* CurrentHeatingCoolingState;
    TargetHeatingCoolingState*  TargetHeatingCoolingState;
    CurrentTemperature*         CurrentTemperature;
    TargetTemperature*          TargetTemperature;
    TemperatureDisplayUnits*    TemperatureDisplayUnits;
} Thermostat;
/**
 * 
 * @return 
 */
Thermostat* NewThermostat(void);

//

typedef struct {
    Service*        Service;
    
    Identify*       Identify;
    Manufacturer*   Manufacturer;
    Model*          Model;
    Name*           Name;
    SerialNumber*   SerialNumber;
} AccessoryInformation;
/**
 * 
 * @return 
 */
AccessoryInformation* NewAccessoryInformation(void);

///

typedef struct {
    Service*    Service;

    Version*    Version;
} Text;
/**
 * 
 * @return 
 */
Text* NewText(void);

/******************************************************************************************************************
 * prototypes
 *
 */

/**
 * 
 * @param typ
 * @return 
 */
Service* NewService(const char* typ);
/**
 * 
 * @param s
 * @param c
 */
void AddCharacteristic(Service* s, Characteristic* c);
/**
 * SetID sets the service id
 * @param s
 * @param id
 */
void SetID(Service* s, sint64_t id);
/**
 * GetID returns the service id
 * @param s
 * @return 
 */
sint64_t GetID(Service* s);

/******************************************************************************************************************
 * 
 *
 */

// service types
#define TypeAccessoryInformation            ServiceTypes[0]
#define TypeAirQualitySensor                ServiceTypes[1]
#define TypeBatteryService                  ServiceTypes[2]
#define TypeBridgingState                   ServiceTypes[3]
#define TypeCarbonDioxideSensor             ServiceTypes[4]
#define TypeCarbonMonoxideSensor            ServiceTypes[5]
#define TypeContactSensor                   ServiceTypes[6]
#define TypeDoor                            ServiceTypes[7]
#define TypeFan                             ServiceTypes[8]
#define TypeGarageDoorOpener                ServiceTypes[9]
#define TypeHumiditySensor                  ServiceTypes[10]
#define TypeLeakSensor                      ServiceTypes[11]
#define TypeLightSensor                     ServiceTypes[12]
#define TypeLightbulb                       ServiceTypes[13]
#define TypeLockManagement                  ServiceTypes[14]
#define TypeLockMechanism                   ServiceTypes[15]
#define TypeMotionSensor                    ServiceTypes[16]
#define TypeOccupancySensor                 ServiceTypes[17]
#define TypeOutlet                          ServiceTypes[18]
#define TypeSecuritySystem                  ServiceTypes[19]
#define TypeSmokeSensor                     ServiceTypes[20]
#define TypeStatefulProgrammableSwitch      ServiceTypes[21]
#define TypeStatelessProgrammableSwitch     ServiceTypes[22]
#define TypeSwitch                          ServiceTypes[23]
#define TypeTemperatureSensor               ServiceTypes[24]
#define TypeThermostat                      ServiceTypes[25]
#define TypeWindow                          ServiceTypes[26]
#define TypeWindowCovering                  ServiceTypes[27]

#ifdef	__cplusplus
}
#endif

#endif	/* SVC_SERVICES_H */

