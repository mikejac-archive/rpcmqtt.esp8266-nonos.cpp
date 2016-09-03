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

#include "svc_accessory.h"
#include <osapi.h>
#include <mem.h>

#define DTXT(...)   os_printf(__VA_ARGS__)
//#define DTXT(...)

/******************************************************************************************************************
 * prototypes
 *
 */

/******************************************************************************************************************
 * public functions
 *
 */

/**
 * 
 * @param name
 * @param serialnumber
 * @param manufacturer
 * @param model
 * @return 
 */
AccOutlet* ICACHE_FLASH_ATTR NewAccOutlet(const char* name, const char* serialnumber, const char* manufacturer, const char* model)
{
    AccOutlet* acc = (AccOutlet*) os_zalloc(sizeof(AccOutlet));
    if(acc == 0) {
        DTXT("NewAccOutlet(AccOutlet): mem fail\n");
        return 0;
    }

    acc->Accessory = NewAccessory(name, serialnumber, manufacturer, model, TypeAccOutlet);
    if(acc->Accessory == 0) {
        DTXT("NewAccOutlet(Accessory): mem fail\n");
        goto defer;
    }
    
    acc->Outlet = NewOutlet();
    if(acc->Outlet == 0) {
        DTXT("NewAccOutlet(Outlet): mem fail\n");
        goto defer;
    }
    
    acc->Outlet->Service->parent           = acc->Accessory;
    acc->Outlet->On->Bool->parent          = acc->Outlet->Service;
    acc->Outlet->OutletInUse->Bool->parent = acc->Outlet->Service;
    
    AddService(acc->Accessory, acc->Outlet->Service);
        
    return acc;
    
defer:
    if(acc->Outlet != 0) {
        os_free(acc->Outlet);
    }
    if(acc->Accessory != 0) {
        os_free(acc->Accessory);
    }
    if(acc != 0) {
        os_free(acc);
    }
    return 0;
}
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
AccThermometer* ICACHE_FLASH_ATTR NewAccThermometer(const char* name, 
                                                    const char* serialnumber, 
                                                    const char* manufacturer, 
                                                    const char* model,
                                                    double      temp,
                                                    double      min,
                                                    double      max,
                                                    double      steps)
{
    AccThermometer* acc = (AccThermometer*) os_zalloc(sizeof(AccThermometer));
    if(acc == 0) {
        DTXT("NewAccThermometer(AccThermometer): mem fail\n");
        return 0;
    }

    acc->Accessory = NewAccessory(name, serialnumber, manufacturer, model, TypeAccThermostat);
    if(acc->Accessory == 0) {
        DTXT("NewAccThermometer(Accessory): mem fail\n");
        goto defer;
    }
    
    acc->TemperatureSensor = NewTemperatureSensor();
    if(acc->TemperatureSensor == 0) {
        DTXT("NewAccThermometer(TemperatureSensor): mem fail\n");
        goto defer;
    }
    
    acc->TemperatureSensor->Service->parent = acc->Accessory;
    acc->TemperatureSensor->CurrentTemperature->Float->parent = acc->TemperatureSensor->Service;
    
    FloatSetMinValue(acc->TemperatureSensor->CurrentTemperature->Float, min);
    FloatSetMaxValue(acc->TemperatureSensor->CurrentTemperature->Float, max);
    FloatSetStepValue(acc->TemperatureSensor->CurrentTemperature->Float, steps);
    FloatSetValue(acc->TemperatureSensor->CurrentTemperature->Float, temp);

    AddService(acc->Accessory, acc->TemperatureSensor->Service);
        
    return acc;
    
defer:
    if(acc->TemperatureSensor != 0) {
        os_free(acc->TemperatureSensor);
    }
    if(acc->Accessory != 0) {
        os_free(acc->Accessory);
    }
    if(acc != 0) {
        os_free(acc);
    }
    return 0;
}
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
AccHumidity* ICACHE_FLASH_ATTR NewAccHumidity(  const char* name, 
                                                const char* serialnumber, 
                                                const char* manufacturer, 
                                                const char* model,
                                                double      hum,
                                                double      min,
                                                double      max,
                                                double      steps)
{
    AccHumidity* acc = (AccHumidity*) os_zalloc(sizeof(AccHumidity));
    if(acc == 0) {
        DTXT("NewAccHumidity(AccHumidity): mem fail\n");
        return 0;
    }

    acc->Accessory = NewAccessory(name, serialnumber, manufacturer, model, TypeAccThermostat);
    if(acc->Accessory == 0) {
        DTXT("NewAccHumidity(Accessory): mem fail\n");
        goto defer;
    }
    
    acc->HumiditySensor = NewHumiditySensor();
    if(acc->HumiditySensor == 0) {
        DTXT("NewAccHumidity(HumiditySensor): mem fail\n");
        goto defer;
    }
    
    acc->HumiditySensor->Service->parent = acc->Accessory;
    acc->HumiditySensor->CurrentRelativeHumidity->Float->parent = acc->HumiditySensor->Service;
    
    FloatSetMinValue(acc->HumiditySensor->CurrentRelativeHumidity->Float, min);
    FloatSetMaxValue(acc->HumiditySensor->CurrentRelativeHumidity->Float, max);
    FloatSetStepValue(acc->HumiditySensor->CurrentRelativeHumidity->Float, steps);
    FloatSetValue(acc->HumiditySensor->CurrentRelativeHumidity->Float, hum);

    AddService(acc->Accessory, acc->HumiditySensor->Service);
        
    return acc;
    
defer:
    if(acc->HumiditySensor != 0) {
        os_free(acc->HumiditySensor);
    }
    if(acc->Accessory != 0) {
        os_free(acc->Accessory);
    }
    if(acc != 0) {
        os_free(acc);
    }
    return 0;
}
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
AccThermostat* ICACHE_FLASH_ATTR NewAccThermostat(const char* name, 
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
                                                    double      targetSteps)
{
    AccThermostat* acc = (AccThermostat*) os_zalloc(sizeof(AccThermostat));
    if(acc == 0) {
        DTXT("NewAccThermostat(AccThermostat): mem fail\n");
        return 0;
    }

    acc->Accessory = NewAccessory(name, serialnumber, manufacturer, model, TypeAccThermostat);
    if(acc->Accessory == 0) {
        DTXT("NewAccThermometer(Accessory): mem fail\n");
        goto defer;
    }
    
    acc->Thermostat = NewThermostat();
    if(acc->Thermostat == 0) {
        DTXT("NewAccThermostat(TemperatureSensor): mem fail\n");
        goto defer;
    }
    
    acc->Thermostat->Service->parent = acc->Accessory;
    acc->Thermostat->CurrentTemperature->Float->parent = acc->Thermostat->Service;
    acc->Thermostat->TargetTemperature->Float->parent = acc->Thermostat->Service;
    acc->Thermostat->CurrentHeatingCoolingState->UInt8->parent = acc->Thermostat->Service;
    acc->Thermostat->TargetHeatingCoolingState->UInt8->parent = acc->Thermostat->Service;
    acc->Thermostat->TemperatureDisplayUnits->UInt8->parent = acc->Thermostat->Service;
    
    FloatSetMinValue(acc->Thermostat->CurrentTemperature->Float, currentMin);
    FloatSetMaxValue(acc->Thermostat->CurrentTemperature->Float, currentMax);
    FloatSetStepValue(acc->Thermostat->CurrentTemperature->Float, currentSteps);
    FloatSetValue(acc->Thermostat->CurrentTemperature->Float, currentTemp);

    FloatSetMinValue(acc->Thermostat->TargetTemperature->Float, targetMin);
    FloatSetMaxValue(acc->Thermostat->TargetTemperature->Float, targetMax);
    FloatSetStepValue(acc->Thermostat->TargetTemperature->Float, targetSteps);
    FloatSetValue(acc->Thermostat->TargetTemperature->Float, targetTemp);

    AddService(acc->Accessory, acc->Thermostat->Service);
        
    return acc;
    
defer:
    if(acc->Thermostat != 0) {
        os_free(acc->Thermostat);
    }
    if(acc->Accessory != 0) {
        os_free(acc->Accessory);
    }
    if(acc != 0) {
        os_free(acc);
    }
    return 0;
}
/**
 * 
 * @param name
 * @param serialnumber
 * @param manufacturer
 * @param model
 * @param text
 * @return 
 */
AccText* ICACHE_FLASH_ATTR NewAccText(const char* name, const char* serialnumber, const char* manufacturer, const char* model, const char* text)
{
    AccText* acc = (AccText*) os_zalloc(sizeof(AccText));
    if(acc == 0) {
        DTXT("NewAccText(AccText): mem fail\n");
        return 0;
    }

    acc->Accessory = NewAccessory(name, serialnumber, manufacturer, model, 702401);
    if(acc->Accessory == 0) {
        DTXT("NewAccText(Accessory): mem fail\n");
        goto defer;
    }
    
    acc->Text = NewText();
    if(acc->Text == 0) {
        DTXT("NewAccText(Text): mem fail\n");
        goto defer;
    }
    
    StringSetValue(acc->Text->Version->String, text);
    
    acc->Text->Service->parent         = acc->Accessory;
    acc->Text->Version->String->parent = acc->Text->Service;
    
    AddService(acc->Accessory, acc->Text->Service);
        
    return acc;
    
defer:
    if(acc->Text != 0) {
        os_free(acc->Text);
    }
    if(acc->Accessory != 0) {
        os_free(acc->Accessory);
    }
    if(acc != 0) {
        os_free(acc);
    }
    return 0;
}
/**
 * 
 * @param name
 * @param serialnumber
 * @param manufacturer
 * @param model
 * @param typ
 * @return 
 */
Accessory* ICACHE_FLASH_ATTR NewAccessory(const char* name, const char* serialnumber, const char* manufacturer, const char* model, int typ)
{
    Accessory* acc = (Accessory*) os_zalloc(sizeof(Accessory));
    if(acc == 0) {
        DTXT("NewAccessory(Accessory): mem fail\n");
        return 0;
    }

    acc->Type       = typ;
    acc->idCount    = 1;

    AccessoryInformation* svc = NewAccessoryInformation();
    if(svc == 0) {
        DTXT("NewAccessory(AccessoryInformation): mem fail\n");
        goto defer;
    }
    
    DTXT("NewAccessory(name):\n");
    if(name != 0 && os_strlen(name) > 0) {
        StringSetValue(svc->Name->String, name);
    } else {
        StringSetValue(svc->Name->String, "undefined");
    }
    
    DTXT("NewAccessory(serialnumber):\n");
    if(serialnumber != 0 && os_strlen(serialnumber) > 0) {
        StringSetValue(svc->SerialNumber->String, serialnumber);
    } else {
        StringSetValue(svc->SerialNumber->String, "undefined");
    }
 
    DTXT("NewAccessory(manufacturer):\n");
    if(manufacturer != 0 && os_strlen(manufacturer) > 0) {
        StringSetValue(svc->Manufacturer->String, manufacturer);
    } else {
        StringSetValue(svc->Manufacturer->String, "undefined");
    }

    DTXT("NewAccessory(model):\n");
    if(model != 0 && os_strlen(model) > 0) {
        StringSetValue(svc->Model->String, model);
    } else {
        StringSetValue(svc->Model->String, "undefined");
    }

    AddService(acc, svc->Service);

    //DTXT("NewAccessory(): success\n");
    
    return acc;

defer:
    if(svc != 0) {
        os_free(svc);
    }

    return 0;
}
/**
 * Adds a service to the accessory and updates the ids of the service and the corresponding characteristics
 * @param a
 * @param s
 */
void ICACHE_FLASH_ATTR AddService(Accessory* a, Service* s)
{
    // first, set id's
    s->ID = a->idCount;
    a->idCount++;

    Characteristic* c = s->Characteristics;
    
    while(c != NULL) {
        c->ID = a->idCount;
        a->idCount++;
        
        c = c->next;
    }
    
    // now append the service
    if(a->Service != NULL) {
        Service* ptr = a->Service;
        
        while(ptr->next != NULL) {
            ptr = ptr->next;
        }
        
        ptr->next = s;
    } else {
        a->Service = s;
    }
}
/**
 * 
 * @param a
 * @param iid
 * @return 
 */
Characteristic* ICACHE_FLASH_ATTR FindCharacteristicByIid(Accessory* a, sint64_t iid)
{
    if(a == 0) {
        DTXT("FindCharacteristicByIid(): 'a' is nil\n");
        return 0;
    }
    
    if(a->Service != NULL) {
        Service* s = a->Service;
        
        while(s != NULL) {
            if(s->Characteristics != NULL) {
                Characteristic* c = s->Characteristics;
                
                while(c != NULL) {
                    if(c->ID == iid) {
                        return c;
                    }
                    
                    c = c->next;
                }
            }
            
            s = s->next;
        }
    }
    
    return 0;
}
