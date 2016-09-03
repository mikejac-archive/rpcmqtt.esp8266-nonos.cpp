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

#include "svc_characteristics.h"
#include "service_device.h"
#include <osapi.h>
#include <mem.h>

#define DTXT(...)   os_printf(__VA_ARGS__)
//#define DTXT(...)

/******************************************************************************************************************
 * 
 *
 */

// characteristic units
const char* CharacteristicUnits[] = {
    "percentage",
    "arcdegrees",
    "celsius",
    "lux",
    "seconds"
};

// characterisitic formats
const char* CharacterisiticFormats[] = {
    "string",
    "bool",
    "float",
    "uint8",
    "uint16",
    "uint32",
    "int32",
    "uint64",
    "data",
    "tlv8",
    "int8",
    "int16"
};

// characteristic types
const char* CharacteristicTypes[] = {
    "57",   //  0
    "1",
    "64",
    "65",
    "95",
    "5",
    "68",
    "8",
    "92",
    "93",
    "94",   // 10
    "69",
    "90",
    "91",
    "A3",
    "8F",
    "A0",
    "9D",
    "6A",
    "D",
    "6B",   // 20
    "E",
    "F",
    "6C",
    "6D",
    "10",
    "11",
    "9B",
    "6E",
    "98",
    "9E",   // 30
    "9F",
    "52",
    "53",
    "12",
    "6F",
    "13",
    "14",
    "70",
    "9C",
    "19",   // 40
    "1D",
    "1C",
    "1A",
    "1E",
    "1F",
    "20",
    "21",
    "22",
    "23",
    "24",   // 50
    "71",
    "25",
    "26",
    "4C",
    "4E",
    "4F",
    "50",
    "72",
    "73",
    "74",
    "63",
    "28",
    "29",
    "2F",
    "8E",
    "66",
    "67",
    "30",
    "76",
    "54",
    "75",
    "77",
    "78",
    "79",
    "7A",
    "32",
    "33",
    "7B",
    "7C",
    "34",
    "35",
    "7D",
    "36",
    "9A",
    "61",
    "60",
    "59",
    "58",
    "37",
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
Identify* ICACHE_FLASH_ATTR NewIdentify(void)
{
    Identify* c = (Identify*) os_zalloc(sizeof(Identify));
    if(c == 0) {
        return 0;
    }
    
    c->Bool = NewBool(TypeIdentify);
    c->Bool->Perms = PermsWriteOnly();
    
    return c;
}
/**
 * 
 * @return 
 */
Manufacturer* ICACHE_FLASH_ATTR NewManufacturer(void)
{
    Manufacturer* c = (Manufacturer*) os_zalloc(sizeof(Manufacturer));
    if(c == 0) {
        return 0;
    }
    
    c->String = NewString(TypeManufacturer);
    c->String->Perms = PermsReadOnly();

    StringSetValue(c->String, "");
    
    return c;
}
/**
 * 
 * @return 
 */
Model* ICACHE_FLASH_ATTR NewModel(void)
{
    Model* c = (Model*) os_zalloc(sizeof(Model));
    if(c == 0) {
        return 0;
    }
    
    c->String = NewString(TypeModel);
    c->String->Perms = PermsReadOnly();

    StringSetValue(c->String, "");
    
    return c;
}
/**
 * 
 * @return 
 */
Name* ICACHE_FLASH_ATTR NewName(void)
{
    Name* c = (Name*) os_zalloc(sizeof(Name));
    if(c == 0) {
        return 0;
    }
    
    c->String = NewString(TypeName);
    c->String->Perms = PermsReadOnly();

    StringSetValue(c->String, "");
    
    return c;
}
/**
 * 
 * @return 
 */
SerialNumber* ICACHE_FLASH_ATTR NewSerialNumber(void)
{
    SerialNumber* c = (SerialNumber*) os_zalloc(sizeof(SerialNumber));
    if(c == 0) {
        return 0;
    }
    
    c->String = NewString(TypeSerialNumber);
    c->String->Perms = PermsReadOnly();

    StringSetValue(c->String, "");
    
    return c;
}
/**
 * 
 * @return 
 */
On* ICACHE_FLASH_ATTR NewOn(void)
{
    On* c = (On*) os_zalloc(sizeof(On));
    if(c == 0) {
        return 0;
    }
    
    c->Bool = NewBool(TypeOn);
    c->Bool->Perms = PermsAll();
    
    BoolSetValue(c->Bool, false);
    
    return c;
}
/**
 * 
 * @return 
 */
OutletInUse* ICACHE_FLASH_ATTR NewOutletInUse(void)
{
    OutletInUse* c = (OutletInUse*) os_zalloc(sizeof(OutletInUse));
    if(c == 0) {
        return 0;
    }
    
    c->Bool = NewBool(TypeOutletInUse);
    c->Bool->Perms = PermsRead();
    
    BoolSetValue(c->Bool, true);
    
    return c;
}
/**
 * 
 * @return 
 */
CurrentTemperature* ICACHE_FLASH_ATTR NewCurrentTemperature(void)
{
    CurrentTemperature* c = (CurrentTemperature*) os_zalloc(sizeof(CurrentTemperature));
    if(c == 0) {
        return 0;
    }
    
    c->Float = NewFloat(TypeCurrentTemperature);
    c->Float->Perms = PermsRead();

    FloatSetMinValue(c->Float, 0);
    FloatSetMaxValue(c->Float, 100);
    FloatSetStepValue(c->Float, 0.1);
    FloatSetValue(c->Float, 0);
    
    c->Float->Unit = UnitCelsius;
    
    return c;
}
/**
 * 
 * @return 
 */
CurrentRelativeHumidity* ICACHE_FLASH_ATTR NewCurrentRelativeHumidity(void)
{
    CurrentRelativeHumidity* c = (CurrentRelativeHumidity*) os_zalloc(sizeof(CurrentRelativeHumidity));
    if(c == 0) {
        return 0;
    }
    
    c->Float = NewFloat(TypeCurrentRelativeHumidity);
    c->Float->Perms = PermsRead();

    FloatSetMinValue(c->Float, 0);
    FloatSetMaxValue(c->Float, 100);
    FloatSetStepValue(c->Float, 1);
    FloatSetValue(c->Float, 0);
    
    c->Float->Unit = UnitPercentage;
    
    return c;
}
/**
 * 
 * @return 
 */
TargetTemperature* ICACHE_FLASH_ATTR NewTargetTemperature(void)
{
    TargetTemperature* c = (TargetTemperature*) os_zalloc(sizeof(TargetTemperature));
    if(c == 0) {
        return 0;
    }
    
    c->Float = NewFloat(TypeTargetTemperature);
    c->Float->Perms = PermsAll();

    FloatSetMinValue(c->Float, 5);
    FloatSetMaxValue(c->Float, 30);
    FloatSetStepValue(c->Float, 1);
    FloatSetValue(c->Float, 0);
    
    c->Float->Unit = UnitCelsius;
    
    return c;
}
/**
 * 
 * @return 
 */
CurrentHeatingCoolingState* ICACHE_FLASH_ATTR NewCurrentHeatingCoolingState(void)
{
    CurrentHeatingCoolingState* c = (CurrentHeatingCoolingState*) os_zalloc(sizeof(CurrentHeatingCoolingState));
    if(c == 0) {
        return 0;
    }
    
    c->UInt8 = NewUInt8(TypeCurrentHeatingCoolingState);
    c->UInt8->Perms = PermsRead();

    UInt8SetValue(c->UInt8, 0);
    
    return c;
}
/**
 * 
 * @return 
 */
TargetHeatingCoolingState* ICACHE_FLASH_ATTR NewTargetHeatingCoolingState(void)
{
    TargetHeatingCoolingState* c = (TargetHeatingCoolingState*) os_zalloc(sizeof(TargetHeatingCoolingState));
    if(c == 0) {
        return 0;
    }
    
    c->UInt8 = NewUInt8(TypeTargetHeatingCoolingState);
    c->UInt8->Perms = PermsAll();

    UInt8SetValue(c->UInt8, 0);
    
    return c;
}
/**
 * 
 * @return 
 */
TemperatureDisplayUnits* ICACHE_FLASH_ATTR NewTemperatureDisplayUnits(void)
{
    TemperatureDisplayUnits* c = (TemperatureDisplayUnits*) os_zalloc(sizeof(TemperatureDisplayUnits));
    if(c == 0) {
        return 0;
    }
    
    c->UInt8 = NewUInt8(TypeTemperatureDisplayUnits);
    c->UInt8->Perms = PermsAll();

    UInt8SetValue(c->UInt8, 0);
    
    return c;
}
/**
 * 
 * @return 
 */
Version* ICACHE_FLASH_ATTR NewVersion(void)
{
    Version* c = (Version*) os_zalloc(sizeof(Version));
    if(c == 0) {
        return 0;
    }
    
    c->String = NewString(TypeVersion);
    c->String->Perms = PermsRead();

    StringSetValue(c->String, "");
    
    return c;
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
Characteristic* ICACHE_FLASH_ATTR NewString(const char* typ)
{
    Characteristic* c = NewCharacteristic(typ);
    if(c == 0) {
        return 0;
    }
    
    c->Format = FormatString;
    
    return c;
}
/**
 * 
 * @param c
 * @param value
 */
void ICACHE_FLASH_ATTR StringSetValue(Characteristic* c, const char* value)
{
    CharacteristicValue v;
    v.String = (char*)value;
    
    characteristicSetValue(c, FormatString, &v);

    DTXT("StringSetValue(): '%s'\n", c->Value->String);
}
/**
 * 
 * @param typ
 * @return 
 */
Characteristic* ICACHE_FLASH_ATTR NewBool(const char* typ)
{
    Characteristic* c = NewCharacteristic(typ);
    if(c == 0) {
        return 0;
    }
    
    c->Format = FormatBool;
    
    return c;
}
/**
 * 
 * @param c
 * @param value
 */
void ICACHE_FLASH_ATTR BoolSetValue(Characteristic* c, bool value)
{
    CharacteristicValue v;
    v.Bool = value;
    
    if(c->parent != 0 && c->parent->parent != 0 && c->parent->parent->parent != 0 && c->parent->parent->parent->parent != 0) {
        setValue(c->parent->parent->parent->parent, c->parent->parent->ID, c->ID, FormatBool, &v, c);
    } else {
        characteristicSetValue(c, FormatBool, &v);
    }
}
/**
 * 
 * @param typ
 * @return 
 */
Characteristic* ICACHE_FLASH_ATTR NewUInt8(const char* typ)
{
    Characteristic* c = NewCharacteristic(typ);
    if(c == 0) {
        return 0;
    }
    
    c->Format = FormatUInt8;
    
    return c;
}
/**
 * 
 * @param c
 * @param value
 */
void ICACHE_FLASH_ATTR UInt8SetValue(Characteristic* c, uint8_t value)
{
    CharacteristicValue v;
    v.UInt8 = value;
    
    if(c->parent != 0 && c->parent->parent != 0 && c->parent->parent->parent != 0 && c->parent->parent->parent->parent != 0) {
        setValue(c->parent->parent->parent->parent, c->parent->parent->ID, c->ID, FormatUInt8, &v, c);
    } else {
        characteristicSetValue(c, FormatUInt8, &v);
    }
}
/**
 * 
 * @param typ
 * @return 
 */
Characteristic* ICACHE_FLASH_ATTR NewFloat(const char* typ)
{
    Characteristic* c = NewCharacteristic(typ);
    if(c == 0) {
        return 0;
    }
    
    c->Format = FormatFloat;
    
    return c;
}
/**
 * 
 * @param c
 * @param value
 */
void ICACHE_FLASH_ATTR FloatSetValue(Characteristic* c, double value)
{
    CharacteristicValue v;
    v.Float = value;
    
    if(c->parent != 0 && c->parent->parent != 0 && c->parent->parent->parent != 0 && c->parent->parent->parent->parent != 0) {
        setValue(c->parent->parent->parent->parent, c->parent->parent->ID, c->ID, FormatFloat, &v, c);
    } else {
        characteristicSetValue(c, FormatFloat, &v);
    }
}
/**
 * 
 * @param typ
 * @return 
 */
Characteristic* ICACHE_FLASH_ATTR NewCharacteristic(const char* typ)
{
    Characteristic* c = (Characteristic*) os_zalloc(sizeof(Characteristic));
    if(c == 0) {
        DTXT("NewCharacteristic(Characteristic): mem fail\n");
        return 0;
    }

    c->Type = typ;
    
    return c;
}
/**
 * PermsAll returns read, write and event permissions
 * @return 
 */
CharacteristicPerms ICACHE_FLASH_ATTR PermsAll(void)
{
    return PermRead | PermWrite | PermEvents;
}
/**
 * PermsRead returns read and event permissions
 * @return 
 */
CharacteristicPerms ICACHE_FLASH_ATTR PermsRead(void)
{
    return PermRead | PermEvents;
}
/**
 * PermsReadOnly returns read permission
 * @return 
 */
CharacteristicPerms ICACHE_FLASH_ATTR PermsReadOnly(void)
{
    return PermRead;
}
/**
 * PermsWriteOnly returns write permission
 * @return 
 */
CharacteristicPerms ICACHE_FLASH_ATTR PermsWriteOnly(void)
{
    return PermWrite;
}
/**
 * 
 * @param c
 * @param format
 * @param value
 * @return 
 */
int ICACHE_FLASH_ATTR characteristicSetValue(Characteristic* c, CharacteristicFormat format, CharacteristicValue* value)
{
    if(c->Value == 0) {
        c->Value  = (CharacteristicValue*) os_zalloc(sizeof(CharacteristicValue));
        if(c->Value == 0) {
            DTXT("characteristicSetValue(CharacteristicValue): mem fail\n");
            return -1;
        }
    }

    switch(format) {
        case FormatString:
            if(c->MaxLen != 0 && os_strlen(value->String) > *c->MaxLen) {
                DTXT("characteristicSetValue(FormatString): truncate\n");
                value->String[*c->MaxLen] = '\0';
            }
            break;
            
        case FormatBool:
            break;
            
        case FormatUInt8:
            if(c->MaxValue != 0 && value->UInt8 > c->MaxValue->UInt8) {
                value->UInt8 = c->MaxValue->UInt8;
            } else if(c->MinValue != 0 && value->UInt8 < c->MinValue->UInt8) {
                value->UInt8 = c->MinValue->UInt8;
            }
            break;
            
        case FormatInt8:
            if(c->MaxValue != 0 && value->Int8 > c->MaxValue->Int8) {
                value->Int8 = c->MaxValue->Int8;
            } else if(c->MinValue != 0 && value->Int8 < c->MinValue->Int8) {
                value->Int8 = c->MinValue->Int8;
            }
            break;
            
        case FormatUInt16:
            if(c->MaxValue != 0 && value->UInt16 > c->MaxValue->UInt16) {
                value->UInt16 = c->MaxValue->UInt16;
            } else if(c->MinValue != 0 && value->UInt16 < c->MinValue->UInt16) {
                value->UInt16 = c->MinValue->UInt16;
            }
            break;
            
        case FormatInt16:
            if(c->MaxValue != 0 && value->Int16 > c->MaxValue->Int16) {
                value->Int16 = c->MaxValue->Int16;
            } else if(c->MinValue != 0 && value->Int16 < c->MinValue->Int16) {
                value->Int16 = c->MinValue->Int16;
            }
            break;
            
        case FormatUInt32:
            if(c->MaxValue != 0 && value->UInt32 > c->MaxValue->UInt32) {
                value->UInt32 = c->MaxValue->UInt32;
            } else if(c->MinValue != 0 && value->UInt32 < c->MinValue->UInt32) {
                value->UInt32 = c->MinValue->UInt32;
            }
            break;
            
        case FormatInt32:
            if(c->MaxValue != 0 && value->Int32 > c->MaxValue->Int32) {
                value->Int32 = c->MaxValue->Int32;
            } else if(c->MinValue != 0 && value->Int32 < c->MinValue->Int32) {
                value->Int32 = c->MinValue->Int32;
            }
            break;
            
        case FormatUInt64:
            if(c->MaxValue != 0 && value->UInt64 > c->MaxValue->UInt64) {
                value->UInt64 = c->MaxValue->UInt64;
            } else if(c->MinValue != 0 && value->UInt64 < c->MinValue->UInt64) {
                value->UInt64 = c->MinValue->UInt64;
            }
            break;
            
        case FormatFloat:
            if(c->StepValue != 0) {
                if(value->Float >= c->Value->Float + c->StepValue->Float) {

                } else if(value->Float <= c->Value->Float - c->StepValue->Float) {

                } else {
                    value->Float = c->Value->Float;
                }
            }

            if(c->MaxValue != 0 && value->Float > c->MaxValue->Float) {
                value->Float = c->MaxValue->Float;
            } else if(c->MinValue != 0 && value->Float < c->MinValue->Float) {
                value->Float = c->MinValue->Float;
            }
            break;
            
        case FormatNone:
            break;
    }

    switch(format) {
        case FormatString:
            if(c->Value->String != 0) {
                os_free(c->Value->String);
            }

            if(value == 0) {
                DTXT("characteristicSetValue(value): 'value' is nil\n");
                c->Value->String = 0;
                return -1;
            }
            if(os_strlen(value->String) == 0) {
                DTXT("characteristicSetValue(value): 'value' is zero-length\n");
                c->Value->String = 0;
                return -1;
            }

            c->Value->String = (char*) os_malloc(os_strlen(value->String) + 1);
            if(c->Value->String == 0) {
                DTXT("characteristicSetValue(char): mem fail\n");
                os_free(c->Value);
                c->Value = 0;
                return -1;
            }

            os_strcpy(c->Value->String, value->String);
            break;

        case FormatBool:
            c->Value->Bool = value->Bool;
            break;
            
        case FormatUInt8:
            c->Value->UInt8 = value->UInt8;
            break;
            
        case FormatInt8:
            c->Value->Int8 = value->Int8;
            break;
            
        case FormatUInt16:
            c->Value->UInt16 = value->UInt16;
            break;
            
        case FormatInt16:
            c->Value->Int16 = value->UInt16;
            break;
            
        case FormatUInt32:
            c->Value->UInt32 = value->UInt32;
            break;
            
        case FormatInt32:
            c->Value->Int32 = value->Int32;
            break;
            
        case FormatUInt64:
            c->Value->UInt64 = value->UInt64;
            break;
            
        case FormatFloat:
            c->Value->Float = value->Float;
            break;
            
        case FormatNone:
            break;
    }
    
    return 0;
}
/**
 * 
 * @param c
 * @param format
 * @param value
 * @return 
 */
int ICACHE_FLASH_ATTR characteristicSetMinValue(Characteristic* c, CharacteristicFormat format, CharacteristicValue* value)
{
    if(c->MinValue == 0) {
        c->MinValue  = (CharacteristicValue*) os_zalloc(sizeof(CharacteristicValue));
        if(c->MinValue == 0) {
            DTXT("characteristicSetMinValue(CharacteristicValue): mem fail\n");
            return -1;
        }
    }

    switch(format) {
        case FormatString:
            break;

        case FormatBool:
            c->MinValue->Bool = value->Bool;
            break;
            
        case FormatUInt8:
            c->MinValue->UInt8 = value->UInt8;
            break;
            
        case FormatInt8:
            c->MinValue->Int8 = value->Int8;
            break;
            
        case FormatUInt16:
            c->MinValue->UInt16 = value->UInt16;
            break;
            
        case FormatInt16:
            c->MinValue->Int16 = value->UInt16;
            break;
            
        case FormatUInt32:
            c->MinValue->UInt32 = value->UInt32;
            break;
            
        case FormatInt32:
            c->MinValue->Int32 = value->Int32;
            break;
            
        case FormatUInt64:
            c->MinValue->UInt64 = value->UInt64;
            break;
            
        case FormatFloat:
            c->MinValue->Float = value->Float;
            break;
            
        case FormatNone:
            break;
    }
    
    return 0;
}
/**
 * 
 * @param c
 * @param format
 * @param value
 * @return 
 */
int ICACHE_FLASH_ATTR characteristicSetMaxValue(Characteristic* c, CharacteristicFormat format, CharacteristicValue* value)
{
    if(c->MaxValue == 0) {
        c->MaxValue  = (CharacteristicValue*) os_zalloc(sizeof(CharacteristicValue));
        if(c->MaxValue == 0) {
            DTXT("characteristicSetMaxValue(CharacteristicValue): mem fail\n");
            return -1;
        }
    }

    switch(format) {
        case FormatString:
            break;

        case FormatBool:
            c->MaxValue->Bool = value->Bool;
            break;
            
        case FormatUInt8:
            c->MaxValue->UInt8 = value->UInt8;
            break;
            
        case FormatInt8:
            c->MaxValue->Int8 = value->Int8;
            break;
            
        case FormatUInt16:
            c->MaxValue->UInt16 = value->UInt16;
            break;
            
        case FormatInt16:
            c->MaxValue->Int16 = value->UInt16;
            break;
            
        case FormatUInt32:
            c->MaxValue->UInt32 = value->UInt32;
            break;
            
        case FormatInt32:
            c->MaxValue->Int32 = value->Int32;
            break;
            
        case FormatUInt64:
            c->MaxValue->UInt64 = value->UInt64;
            break;
            
        case FormatFloat:
            c->MaxValue->Float = value->Float;
            break;
            
        case FormatNone:
            break;
    }
    
    return 0;
}
/**
 * 
 * @param c
 * @param format
 * @param value
 * @return 
 */
int ICACHE_FLASH_ATTR characteristicSetStepValue(Characteristic* c, CharacteristicFormat format, CharacteristicValue* value)
{
    if(c->StepValue == 0) {
        c->StepValue  = (CharacteristicValue*) os_zalloc(sizeof(CharacteristicValue));
        if(c->StepValue == 0) {
            DTXT("characteristicSetStepValue(CharacteristicValue): mem fail\n");
            return -1;
        }
    }

    switch(format) {
        case FormatString:
            break;

        case FormatBool:
            c->StepValue->Bool = value->Bool;
            break;
            
        case FormatUInt8:
            c->StepValue->UInt8 = value->UInt8;
            break;
            
        case FormatInt8:
            c->StepValue->Int8 = value->Int8;
            break;
            
        case FormatUInt16:
            c->StepValue->UInt16 = value->UInt16;
            break;
            
        case FormatInt16:
            c->StepValue->Int16 = value->UInt16;
            break;
            
        case FormatUInt32:
            c->StepValue->UInt32 = value->UInt32;
            break;
            
        case FormatInt32:
            c->StepValue->Int32 = value->Int32;
            break;
            
        case FormatUInt64:
            c->StepValue->UInt64 = value->UInt64;
            break;
            
        case FormatFloat:
            c->StepValue->Float = value->Float;
            break;
            
        case FormatNone:
            break;
    }
    
    return 0;
}


