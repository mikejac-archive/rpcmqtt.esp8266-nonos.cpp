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

#ifndef SVC_CHARACTERISTICS_H
#define	SVC_CHARACTERISTICS_H

#include <github.com/mikejac/misc.esp8266-nonos.cpp/espmissingincludes.h>

#ifdef	__cplusplus
extern "C" {
#endif

/******************************************************************************************************************
 *  
 *
 */

typedef struct Service Service;

/******************************************************************************************************************
 * basic value types 
 *
 */

typedef enum {
    FormatString,
    FormatBool,
    FormatUInt8,
    FormatInt8,
    FormatUInt16,
    FormatInt16,
    FormatUInt32,
    FormatInt32,
    FormatUInt64,
    FormatFloat,
    FormatNone
} CharacteristicFormat;
    
typedef union {
    char*       String;
    bool        Bool;
    uint8_t     UInt8;
    int8_t      Int8;
    uint16_t    UInt16;
    int16_t     Int16;
    uint32_t    UInt32;
    int32_t     Int32;
    uint64_t    UInt64;
    double      Float;
} CharacteristicValue;

typedef enum {
    PermRead    = 0x01,     // can be read
    PermWrite   = 0x02,     // can be written
    PermEvents  = 0x04      // sends events
} CharacteristicPerms;

typedef struct Characteristic Characteristic;

struct Characteristic {
    sint64_t                ID;                 // "iid"
    const char*             Type;               // "type"
    CharacteristicPerms     Perms;              // "perms"
    const char*             Description;        // "description,omitempty"
    
    CharacteristicValue*    Value;              // "value,omitempty" - nil for write-only characteristics
    CharacteristicFormat    Format;             // "format"
    const char*             Unit;               // "unit,omitempty"
    
    int*                    MaxLen;             // "maxLen,omitempty"
    CharacteristicValue*    MaxValue;           // "maxValue,omitempty"
    CharacteristicValue*    MinValue;           // "minValue,omitempty"
    CharacteristicValue*    StepValue;          // "minStep,omitempty"

    Characteristic*         next;
    Service*                parent;
};

#define UInt8SetMinValue(c, v)                  do {CharacteristicValue vv; vv.UInt8 = v; characteristicSetMinValue(c, FormatUInt8, &vv);} while(0)
#define UInt8SetMaxValue(c, v)                  do {CharacteristicValue vv; vv.UInt8 = v; characteristicSetMaxValue(c, FormatUInt8, &vv);} while(0)
#define UInt8SetStepValue(c, v)                 do {CharacteristicValue vv; vv.UInt8 = v; characteristicSetStepValue(c, FormatUInt8, &vv);} while(0)

#define FloatSetMinValue(c, v)                  do {CharacteristicValue vv; vv.Float = v; characteristicSetMinValue(c, FormatFloat, &vv);} while(0)
#define FloatSetMaxValue(c, v)                  do {CharacteristicValue vv; vv.Float = v; characteristicSetMaxValue(c, FormatFloat, &vv);} while(0)
#define FloatSetStepValue(c, v)                 do {CharacteristicValue vv; vv.Float = v; characteristicSetStepValue(c, FormatFloat, &vv);} while(0)

/******************************************************************************************************************
 * 
 *
 */

typedef struct {
    Characteristic* Bool;
} Identify;
/**
 * 
 * @return 
 */
Identify* NewIdentify(void);

///

typedef struct {
    Characteristic* String;
} Manufacturer;
/**
 * 
 * @return 
 */
Manufacturer* NewManufacturer(void);

///

typedef struct {
    Characteristic* String;
} Model;
/**
 * 
 * @return 
 */
Model* NewModel(void);

///

typedef struct {
    Characteristic* String;
} Name;
/**
 * 
 * @return 
 */
Name* NewName(void);

///

typedef struct {
    Characteristic* String;
} SerialNumber;
/**
 * 
 * @return 
 */
SerialNumber* NewSerialNumber(void);

///

typedef struct {
    Characteristic* Bool;
} On;
/**
 * 
 * @return 
 */
On* NewOn(void);

///

typedef struct {
    Characteristic* Bool;
} OutletInUse;
/**
 * 
 * @return 
 */
OutletInUse* NewOutletInUse(void);

///

typedef struct {
    Characteristic* Float;
} CurrentTemperature;
/**
 * 
 * @return 
 */
CurrentTemperature* NewCurrentTemperature(void);

///

typedef struct {
    Characteristic* Float;
} CurrentRelativeHumidity;
/**
 * 
 * @return 
 */
CurrentRelativeHumidity* NewCurrentRelativeHumidity(void);

///

typedef struct {
    Characteristic* Float;
} TargetTemperature;
/**
 * 
 * @return 
 */
TargetTemperature* NewTargetTemperature(void);

///

#define CurrentHeatingCoolingStateOff  0
#define CurrentHeatingCoolingStateHeat 1
#define CurrentHeatingCoolingStateCool 2

typedef struct {
    Characteristic* UInt8;
} CurrentHeatingCoolingState;
/**
 * 
 * @return 
 */
CurrentHeatingCoolingState* NewCurrentHeatingCoolingState(void);

///

#define TargetHeatingCoolingStateOff  0
#define TargetHeatingCoolingStateHeat 1
#define TargetHeatingCoolingStateCool 2
#define TargetHeatingCoolingStateAuto 3

typedef struct {
    Characteristic* UInt8;
} TargetHeatingCoolingState;
/**
 * 
 * @return 
 */
TargetHeatingCoolingState* NewTargetHeatingCoolingState(void);

///

typedef struct {
    Characteristic* UInt8;
} TemperatureDisplayUnits;
/**
 * 
 * @return 
 */
TemperatureDisplayUnits* NewTemperatureDisplayUnits(void);

///

typedef struct {
    Characteristic* String;
} Version;
/**
 * 
 * @return 
 */
Version* NewVersion(void);

///

typedef struct {
    Characteristic* UInt8;
} ProgrammableSwitchEvent;
/**
 * 
 * @return 
 */
ProgrammableSwitchEvent* NewProgrammableSwitchEvent(void);

///

typedef struct {
    Characteristic* UInt8;
} ProgrammableSwitchOutputState;
/**
 * 
 * @return 
 */
ProgrammableSwitchOutputState* NewProgrammableSwitchOutputState(void);

/******************************************************************************************************************
 * 
 *
 */

// characteristic units
#define UnitPercentage                          CharacteristicUnits[0]
#define UnitArcDegrees                          CharacteristicUnits[1]
#define UnitCelsius                             CharacteristicUnits[2]
#define UnitLux                                 CharacteristicUnits[3]
#define UnitSeconds                             CharacteristicUnits[4]

// characterisitic formats
#define FormatStringTxt                         CharacterisiticFormats[0]
#define FormatBoolTxt                           CharacterisiticFormats[1]
#define FormatFloatTxt                          CharacterisiticFormats[2]
#define FormatUInt8Txt                          CharacterisiticFormats[3]
#define FormatUInt16Txt                         CharacterisiticFormats[4]
#define FormatUInt32Txt                         CharacterisiticFormats[5]
#define FormatInt32Txt                          CharacterisiticFormats[6]
#define FormatUInt64Txt                         CharacterisiticFormats[7]
#define FormatDataTxt                           CharacterisiticFormats[8]
#define FormatTLV8Txt                           CharacterisiticFormats[9]
#define FormatInt8Txt                           CharacterisiticFormats[10]
#define FormatInt16Txt                          CharacterisiticFormats[11]

extern const char* CharacterisiticFormats[];

// characteristic types
#define TypeAccessoryIdentifier                 CharacteristicTypes[0]
#define TypeAdministratorOnlyAccess             CharacteristicTypes[1]
#define TypeAirParticulateDensity               CharacteristicTypes[2]
#define TypeAirParticulateSize                  CharacteristicTypes[3]
#define TypeAirQuality                          CharacteristicTypes[4]
#define TypeAudioFeedback                       CharacteristicTypes[5]
#define TypeBatteryLevel                        CharacteristicTypes[6]
#define TypeBrightness                          CharacteristicTypes[7]
#define TypeCarbonDioxideDetected               CharacteristicTypes[8]
#define TypeCarbonDioxideLevel                  CharacteristicTypes[9]
#define TypeCarbonDioxidePeakLevel              CharacteristicTypes[10]
#define TypeCarbonMonoxideDetected              CharacteristicTypes[11]
#define TypeCarbonMonoxideLevel                 CharacteristicTypes[12]
#define TypeCarbonMonoxidePeakLevel             CharacteristicTypes[13]
#define TypeCategory                            CharacteristicTypes[14]
#define TypeChargingState                       CharacteristicTypes[15]
#define TypeConfigureBridgedAccessory           CharacteristicTypes[16]
#define TypeConfigureBridgedAccessoryStatus     CharacteristicTypes[17]
#define TypeContactSensorState                  CharacteristicTypes[18]
#define TypeCoolingThresholdTemperature         CharacteristicTypes[19]
#define TypeCurrentAmbientLightLevel            CharacteristicTypes[20]
#define TypeCurrentDoorState                    CharacteristicTypes[21]
#define TypeCurrentHeatingCoolingState          CharacteristicTypes[22]
#define TypeCurrentHorizontalTiltAngle          CharacteristicTypes[23]
#define TypeCurrentPosition                     CharacteristicTypes[24]
#define TypeCurrentRelativeHumidity             CharacteristicTypes[25]
#define TypeCurrentTemperature                  CharacteristicTypes[26]
#define TypeCurrentTime                         CharacteristicTypes[27]
#define TypeCurrentVerticalTiltAngle            CharacteristicTypes[28]
#define TypeDayOfTheWeek                        CharacteristicTypes[29]
#define TypeDiscoverBridgedAccessories          CharacteristicTypes[30]
#define TypeDiscoveredBridgedAccessories        CharacteristicTypes[31]
#define TypeFirmwareRevision                    CharacteristicTypes[32]
#define TypeHardwareRevision                    CharacteristicTypes[33]
#define TypeHeatingThresholdTemperature         CharacteristicTypes[34]
#define TypeHoldPosition                        CharacteristicTypes[35]
#define TypeHue                                 CharacteristicTypes[36]
#define TypeIdentify                            CharacteristicTypes[37]
#define TypeLeakDetected                        CharacteristicTypes[38]
#define TypeLinkQuality                         CharacteristicTypes[39]
#define TypeLockControlPoint                    CharacteristicTypes[40]
#define TypeLockCurrentState                    CharacteristicTypes[41]
#define TypeLockLastKnownAction                 CharacteristicTypes[42]
#define TypeLockManagementAutoSecurityTimeout   CharacteristicTypes[43]
#define TypeLockTargetState                     CharacteristicTypes[44]
#define TypeLogs                                CharacteristicTypes[45]
#define TypeManufacturer                        CharacteristicTypes[46]
#define TypeModel                               CharacteristicTypes[47]
#define TypeMotionDetected                      CharacteristicTypes[48]
#define TypeName                                CharacteristicTypes[49]
#define TypeObstructionDetected                 CharacteristicTypes[50]
#define TypeOccupancyDetected                   CharacteristicTypes[51]
#define TypeOn                                  CharacteristicTypes[52]
#define TypeOutletInUse                         CharacteristicTypes[53]
#define TypePairSetup                           CharacteristicTypes[54]
#define TypePairVerify                          CharacteristicTypes[55]
#define TypePairingFeatures                     CharacteristicTypes[56]
#define TypePairingPairings                     CharacteristicTypes[57]
#define TypePositionState                       CharacteristicTypes[58]
#define TypeProgrammableSwitchEvent             CharacteristicTypes[59]
#define TypeProgrammableSwitchOutputState       CharacteristicTypes[60]
#define TypeReachable                           CharacteristicTypes[61]
#define TypeRotationDirection                   CharacteristicTypes[62]
#define TypeRotationSpeed                       CharacteristicTypes[63]
#define TypeSaturation                          CharacteristicTypes[64]
#define TypeSecuritySystemAlarmType             CharacteristicTypes[65]
#define TypeSecuritySystemCurrentState          CharacteristicTypes[66]
#define TypeSecuritySystemTargetState           CharacteristicTypes[67]
#define TypeSerialNumber                        CharacteristicTypes[68]
#define TypeSmokeDetected                       CharacteristicTypes[69]
#define TypeSoftwareRevision                    CharacteristicTypes[70]
#define TypeStatusActive                        CharacteristicTypes[71]
#define TypeStatusFault                         CharacteristicTypes[72]
#define TypeStatusJammed                        CharacteristicTypes[73]
#define TypeStatusLowBattery                    CharacteristicTypes[74]
#define TypeStatusTampered                      CharacteristicTypes[75]
#define TypeTargetDoorState                     CharacteristicTypes[76]
#define TypeTargetHeatingCoolingState           CharacteristicTypes[77]
#define TypeTargetHorizontalTiltAngle           CharacteristicTypes[78]
#define TypeTargetPosition                      CharacteristicTypes[79]
#define TypeTargetRelativeHumidity              CharacteristicTypes[80]
#define TypeTargetTemperature                   CharacteristicTypes[81]
#define TypeTargetVerticalTiltAngle             CharacteristicTypes[82]
#define TypeTemperatureDisplayUnits             CharacteristicTypes[83]
#define TypeTimeUpdate                          CharacteristicTypes[84]
#define TypeTunnelConnectionTimeout             CharacteristicTypes[85]
#define TypeTunneledAccessoryAdvertising        CharacteristicTypes[86]
#define TypeTunneledAccessoryConnected          CharacteristicTypes[87]
#define TypeTunneledAccessoryStateNumber        CharacteristicTypes[88]
#define TypeVersion                             CharacteristicTypes[89]

extern const char* CharacteristicTypes[];

/******************************************************************************************************************
 * prototypes
 *
 */

/**
 * 
 * @param typ
 * @return 
 */
Characteristic* NewString(const char* typ);
/**
 * 
 * @param c
 * @param value
 */
void StringSetValue(Characteristic* c, const char* value);
/**
 * 
 * @param typ
 * @return 
 */
Characteristic* NewBool(const char* typ);
/**
 * 
 * @param c
 * @param value
 */
 void BoolSetValue(Characteristic* c, bool value);
 /**
 * 
 * @param typ
 * @return 
 */
Characteristic* NewUInt8(const char* typ);
/**
 * 
 * @param c
 * @param value
 */
 void UInt8SetValue(Characteristic* c, uint8_t value);
/**
 * 
 * @param typ
 * @return 
 */
Characteristic* NewFloat(const char* typ);
/**
 * 
 * @param c
 * @param value
 */
 void FloatSetValue(Characteristic* c, double value);
/**
 * 
 * @param typ
 * @return 
 */
Characteristic* NewCharacteristic(const char* typ);
/**
 * PermsAll returns read, write and event permissions
 * @return 
 */
CharacteristicPerms PermsAll(void);
/**
 * PermsRead returns read and event permissions
 * @return 
 */
CharacteristicPerms PermsRead(void);
/**
 * PermsReadOnly returns read permission
 * @return 
 */
CharacteristicPerms PermsReadOnly(void);
/**
 * PermsWriteOnly returns write permission
 * @return 
 */
CharacteristicPerms PermsWriteOnly(void);
/**
 * 
 * @param c
 * @param format
 * @param value
 * @return 
 */
int characteristicSetValue(Characteristic* c, CharacteristicFormat format, CharacteristicValue* value);
/**
 * 
 * @param c
 * @param format
 * @param value
 * @return 
 */
int characteristicSetMinValue(Characteristic* c, CharacteristicFormat format, CharacteristicValue* value);
/**
 * 
 * @param c
 * @param format
 * @param value
 * @return 
 */
int characteristicSetMaxValue(Characteristic* c, CharacteristicFormat format, CharacteristicValue* value);
/**
 * 
 * @param c
 * @param format
 * @param value
 * @return 
 */
int characteristicSetStepValue(Characteristic* c, CharacteristicFormat format, CharacteristicValue* value);

#ifdef	__cplusplus
}
#endif

#endif	/* SVC_CHARACTERISTICS_H */

