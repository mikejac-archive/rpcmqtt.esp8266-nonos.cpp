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

#include "svc_services.h"
#include <osapi.h>
#include <mem.h>

#define DTXT(...)   os_printf(__VA_ARGS__)
//#define DTXT(...)

// service types
const char* ServiceTypes[] = {
    "3E",
    "8D",
    "96",
    "62",
    "97",
    "7F",
    "80",
    "81",
    "40",
    "41",
    "82",
    "83",
    "84",
    "43",
    "44",
    "45",
    "85",
    "86",
    "47",
    "7E",
    "87",
    "88",
    "89",
    "49",
    "8A",
    "4A",
    "8B",
    "8C"
};

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
 * @return 
 */
AccessoryInformation* ICACHE_FLASH_ATTR NewAccessoryInformation(void)
{
    AccessoryInformation* svc = (AccessoryInformation*) os_zalloc(sizeof(AccessoryInformation));
    if(svc == 0) {
        DTXT("NewAccessoryInformation(AccessoryInformation): \n");
        return 0;
    }

    svc->Service = NewService(TypeAccessoryInformation);
    if(svc->Service == 0) {
        DTXT("NewAccessoryInformation(Service): mem fail\n");
        goto defer;
    }
    
    svc->Identify = NewIdentify();
    if(svc->Identify == 0) {
        DTXT("NewAccessoryInformation(Identify): mem fail\n");
        goto defer;
    }
    
    svc->Manufacturer = NewManufacturer();
    if(svc->Manufacturer == 0) {
        DTXT("NewAccessoryInformation(Manufacturer): mem fail\n");
        goto defer;
    }
    
    svc->Model = NewModel();
    if(svc->Model == 0) {
        DTXT("NewAccessoryInformation(Model): mem fail\n");
        goto defer;
    }
    
    svc->Name = NewName();
    if(svc->Name == 0) {
        DTXT("NewAccessoryInformation(Name): mem fail\n");
        goto defer;
    }
    
    svc->SerialNumber = NewSerialNumber();
    if(svc->SerialNumber == 0) {
        DTXT("NewAccessoryInformation(SerialNumber): mem fail\n");
        goto defer;
    }
    
    DTXT("NewAccessoryInformation(Identify): %s\n", svc->Identify->Bool->Type);
    DTXT("NewAccessoryInformation(Manufacturer): %s\n", svc->Manufacturer->String->Type);
    DTXT("NewAccessoryInformation(Model): %s\n", svc->Model->String->Type);
    DTXT("NewAccessoryInformation(Name): %s\n", svc->Name->String->Type);
    DTXT("NewAccessoryInformation(SerialNumber): %s\n", svc->SerialNumber->String->Type);
            
    AddCharacteristic(svc->Service, svc->Identify->Bool);
    AddCharacteristic(svc->Service, svc->Manufacturer->String);
    AddCharacteristic(svc->Service, svc->Model->String);
    AddCharacteristic(svc->Service, svc->Name->String);
    AddCharacteristic(svc->Service, svc->SerialNumber->String);

    //DTXT("NewAccessoryInformation(): success\n");
    
    return svc;
    
defer:
    if(svc->SerialNumber != 0) {
        os_free(svc->SerialNumber);
    }
    if(svc->Name != 0) {
        os_free(svc->Name);
    }
    if(svc->Model != 0) {
        os_free(svc->Model);
    }
    if(svc->Manufacturer != 0) {
        os_free(svc->Manufacturer);
    }
    if(svc->Identify != 0) {
        os_free(svc->Identify);
    }
    if(svc != 0) {
        os_free(svc);
    }
    return 0;
}
/**
 * 
 * @return 
 */
Outlet* ICACHE_FLASH_ATTR NewOutlet(void)
{
    Outlet* svc = (Outlet*) os_zalloc(sizeof(Outlet));
    if(svc == 0) {
        DTXT("NewOutlet(Outlet): mem fail\n");
        return 0;
    }

    svc->Service = NewService(TypeOutlet);
    if(svc->Service == 0) {
        DTXT("NewOutlet(Service): mem fail\n");
        goto defer;
    }
    
    svc->On = NewOn();
    if(svc->On == 0) {
        DTXT("NewOutlet(On): mem fail\n");
        goto defer;
    }

    svc->OutletInUse = NewOutletInUse();
    if(svc->OutletInUse == 0) {
        DTXT("NewOutlet(OutletInUse): mem fail\n");
        goto defer;
    }

    AddCharacteristic(svc->Service, svc->On->Bool);
    AddCharacteristic(svc->Service, svc->OutletInUse->Bool);

    //DTXT("NewOutlet(): success\n");

    return svc;
    
defer:
    if(svc->Service != 0) {
        os_free(svc->Service);
    }
    if(svc != 0) {
        os_free(svc);
    }
    return 0;
}
/**
 * 
 * @return 
 */
TemperatureSensor* ICACHE_FLASH_ATTR NewTemperatureSensor(void)
{
    TemperatureSensor* svc = (TemperatureSensor*) os_zalloc(sizeof(TemperatureSensor));
    if(svc == 0) {
        DTXT("NewTemperatureSensor(TemperatureSensor): mem fail\n");
        return 0;
    }

    svc->Service = NewService(TypeTemperatureSensor);
    if(svc->Service == 0) {
        DTXT("NewTemperatureSensor(Service): mem fail\n");
        goto defer;
    }
    
    svc->CurrentTemperature = NewCurrentTemperature();
    if(svc->CurrentTemperature == 0) {
        DTXT("NewTemperatureSensor(CurrentTemperature): mem fail\n");
        goto defer;
    }

    AddCharacteristic(svc->Service, svc->CurrentTemperature->Float);

    return svc;
    
defer:
    if(svc->Service != 0) {
        os_free(svc->Service);
    }
    if(svc != 0) {
        os_free(svc);
    }
    return 0;
}
/**
 * 
 * @return 
 */
HumiditySensor* ICACHE_FLASH_ATTR NewHumiditySensor(void)
{
    HumiditySensor* svc = (HumiditySensor*) os_zalloc(sizeof(HumiditySensor));
    if(svc == 0) {
        DTXT("NewHumiditySensor(HumiditySensor): mem fail\n");
        return 0;
    }

    svc->Service = NewService(TypeHumiditySensor);
    if(svc->Service == 0) {
        DTXT("NewHumiditySensor(Service): mem fail\n");
        goto defer;
    }
    
    svc->CurrentRelativeHumidity = NewCurrentRelativeHumidity();
    if(svc->CurrentRelativeHumidity == 0) {
        DTXT("NewHumiditySensor(CurrentRelativeHumidity): mem fail\n");
        goto defer;
    }

    AddCharacteristic(svc->Service, svc->CurrentRelativeHumidity->Float);

    return svc;
    
defer:
    if(svc->Service != 0) {
        os_free(svc->Service);
    }
    if(svc != 0) {
        os_free(svc);
    }
    return 0;
}
/**
 * 
 * @return 
 */
Thermostat* ICACHE_FLASH_ATTR NewThermostat(void)
{
    Thermostat* svc = (Thermostat*) os_zalloc(sizeof(Thermostat));
    if(svc == 0) {
        DTXT("NewThermostat(Thermostat): mem fail\n");
        return 0;
    }

    svc->Service = NewService(TypeTemperatureSensor);
    if(svc->Service == 0) {
        DTXT("NewThermostat(Service): mem fail\n");
        goto defer;
    }

    svc->CurrentHeatingCoolingState = NewCurrentHeatingCoolingState();
    if(svc->CurrentHeatingCoolingState == 0) {
        DTXT("NewThermostat(CurrentHeatingCoolingState): mem fail\n");
        goto defer;
    }
    
    svc->TargetHeatingCoolingState = NewTargetHeatingCoolingState();
    if(svc->TargetHeatingCoolingState == 0) {
        DTXT("NewThermostat(TargetHeatingCoolingState): mem fail\n");
        goto defer;
    }

    svc->CurrentTemperature = NewCurrentTemperature();
    if(svc->CurrentTemperature == 0) {
        DTXT("NewThermostat(CurrentTemperature): mem fail\n");
        goto defer;
    }

    svc->TargetTemperature = NewTargetTemperature();
    if(svc->TargetTemperature == 0) {
        DTXT("NewThermostat(TargetTemperature): mem fail\n");
        goto defer;
    }

    svc->TemperatureDisplayUnits = NewTemperatureDisplayUnits();
    if(svc->TemperatureDisplayUnits == 0) {
        DTXT("NewThermostat(TTemperatureDisplayUnits): mem fail\n");
        goto defer;
    }

    AddCharacteristic(svc->Service, svc->CurrentHeatingCoolingState->UInt8);
    AddCharacteristic(svc->Service, svc->TargetHeatingCoolingState->UInt8);
    AddCharacteristic(svc->Service, svc->CurrentTemperature->Float);
    AddCharacteristic(svc->Service, svc->TargetTemperature->Float);
    AddCharacteristic(svc->Service, svc->TemperatureDisplayUnits->UInt8);

    return svc;
    
defer:
    if(svc->Service != 0) {
        os_free(svc->Service);
    }
    if(svc != 0) {
        os_free(svc);
    }
    return 0;
}
/**
 * 
 * @return 
 */
StatefulProgrammableSwitch* ICACHE_FLASH_ATTR NewStatefulProgrammableSwitch(void)
{
    StatefulProgrammableSwitch* svc = (StatefulProgrammableSwitch*) os_zalloc(sizeof(StatefulProgrammableSwitch));
    if(svc == 0) {
        DTXT("NewStatefulProgrammableSwitch(StatefulProgrammableSwitch): mem fail\n");
        return 0;
    }

    svc->Service = NewService(TypeStatefulProgrammableSwitch);
    if(svc->Service == 0) {
        DTXT("NewStatefulProgrammableSwitch(Service): mem fail\n");
        goto defer;
    }

    svc->ProgrammableSwitchEvent = NewProgrammableSwitchEvent();
    if(svc->ProgrammableSwitchEvent == 0) {
        DTXT("NewStatefulProgrammableSwitch(ProgrammableSwitchEvent): mem fail\n");
        goto defer;
    }
    
    svc->ProgrammableSwitchOutputState = NewProgrammableSwitchOutputState();
    if(svc->ProgrammableSwitchOutputState == 0) {
        DTXT("NewStatefulProgrammableSwitch(ProgrammableSwitchOutputState): mem fail\n");
        goto defer;
    }

    AddCharacteristic(svc->Service, svc->ProgrammableSwitchEvent->UInt8);
    AddCharacteristic(svc->Service, svc->ProgrammableSwitchOutputState->UInt8);

    return svc;
    
defer:
    if(svc->Service != 0) {
        os_free(svc->Service);
    }
    if(svc != 0) {
        os_free(svc);
    }
    return 0;
}
/**
 * 
 * @return 
 */
StatelessProgrammableSwitch* ICACHE_FLASH_ATTR NewStatelessProgrammableSwitch(void)
{
    StatelessProgrammableSwitch* svc = (StatelessProgrammableSwitch*) os_zalloc(sizeof(StatelessProgrammableSwitch));
    if(svc == 0) {
        DTXT("NewStatelessProgrammableSwitch(StatelessProgrammableSwitch): mem fail\n");
        return 0;
    }

    svc->Service = NewService(TypeStatelessProgrammableSwitch);
    if(svc->Service == 0) {
        DTXT("NewStatelessProgrammableSwitch(Service): mem fail\n");
        goto defer;
    }

    svc->ProgrammableSwitchEvent = NewProgrammableSwitchEvent();
    if(svc->ProgrammableSwitchEvent == 0) {
        DTXT("NewStatelessProgrammableSwitch(ProgrammableSwitchEvent): mem fail\n");
        goto defer;
    }
    
    AddCharacteristic(svc->Service, svc->ProgrammableSwitchEvent->UInt8);

    return svc;
    
defer:
    if(svc->Service != 0) {
        os_free(svc->Service);
    }
    if(svc != 0) {
        os_free(svc);
    }
    return 0;
}
/**
 * 
 * @return 
 */
Text* ICACHE_FLASH_ATTR NewText(void)
{
    Text* svc = (Text*) os_zalloc(sizeof(Text));
    if(svc == 0) {
        DTXT("NewText(Text): mem fail\n");
        return 0;
    }

    svc->Service = NewService("702401");
    if(svc->Service == 0) {
        DTXT("NewText(Service): mem fail\n");
        goto defer;
    }
    
    svc->Version = NewVersion();
    if(svc->Version == 0) {
        DTXT("NewText(Version): mem fail\n");
        goto defer;
    }

    AddCharacteristic(svc->Service, svc->Version->String);

    return svc;
    
defer:
    if(svc->Service != 0) {
        os_free(svc->Service);
    }
    if(svc != 0) {
        os_free(svc);
    }
    return 0;
}

/******************************************************************************************************************
 * base functions
 *
 */

/**
 * 
 * @param typ
 * @return 
 */
Service* ICACHE_FLASH_ATTR NewService(const char* typ)
{
    Service* s = (Service*) os_zalloc(sizeof(Service));
    if(s == 0) {
        return 0;
    }

    s->Type = typ;
    
    return s;
}
/**
 * 
 * @param s
 * @param c
 */
void ICACHE_FLASH_ATTR AddCharacteristic(Service* s, Characteristic* c)
{
    if(s->Characteristics != NULL) {
        Characteristic* ptr = s->Characteristics;
        
        while(ptr->next != NULL) {
            ptr = ptr->next;
        }
        
        ptr->next = c;
    } else {
        s->Characteristics = c;
    }
}
/**
 * SetID sets the service id
 * @param s
 * @param id
 */
void ICACHE_FLASH_ATTR SetID(Service* s, sint64_t id)
{
    s->ID = id;
}
/**
 * GetID returns the service id
 * @param s
 * @return 
 */
sint64_t ICACHE_FLASH_ATTR GetID(Service* s)
{
    return s->ID;
}
