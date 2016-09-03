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

#ifndef SERVICE_DEVICE_H
#define	SERVICE_DEVICE_H

#include "mqtt_connector.h"
#include "svc_container.h"

#ifdef	__cplusplus
extern "C" {
#endif

/******************************************************************************************************************
 * 
 *
 */

typedef struct MqttDevice MqttDevice;

struct MqttDevice {
    Mqtt*       parent;
    int         qos;
    Container*  container;
};

typedef struct Device_Message Device_Message;

struct Device_Message {
    STAILQ_ENTRY(Device_Message) entries;    // tail queue
            
    sint64_t                aid;
    sint64_t                iid;
    CharacteristicFormat    format;
    CharacteristicValue     value;
    
    Accessory*              acc;
};

#define Device_GetAid(m)            (m->aid)
#define Device_GetIid(m)            (m->iid)
#define Device_GetAcc(m)            (m->acc)
#define Device_GetValueBool(m)      (m->value.Bool)
#define Device_GetValueUInt8(m)     (m->value.UInt8)
#define Device_GetValueFloat(m)     (m->value.Float)

/******************************************************************************************************************
 * prototypes
 *
 */

/**
 * 
 * @param parent
 * @return 
 */
MqttDevice* NewDevice(Mqtt* parent);
/**
 * 
 * @param d
 * @param container
 * @return 
 */
int SetAccessories(MqttDevice* d, Container* container);
/**
 * 
 * @param client
 * @return 
 */
Device_Message* DeviceGetEvent(MqttDevice* d);
/**
 * 
 * @param msg
 * @return 
 */
CharacteristicFormat DeviceGetEventType(Device_Message* msg);
/**
 * 
 * @param msg
 */
void DeviceDeleteEvent(Device_Message* msg);
/**
 * 
 * @param d
 * @param aid
 * @param iid
 * @param value
 * @return 
 */
int SetValueString(MqttDevice* d, sint64_t aid, sint64_t iid, const char* value);
/**
 * 
 * @param d
 * @param aid
 * @param iid
 * @param value
 * @return 
 */
int SetValueBool(MqttDevice* d, sint64_t aid, sint64_t iid, bool value);
/**
 * 
 * @param d
 * @param aid
 * @param iid
 * @param value
 * @return 
 */
int SetValueUInt8(MqttDevice* d, sint64_t aid, sint64_t iid, uint8_t value);
/**
 * 
 * @param d
 * @param aid
 * @param iid
 * @param value
 * @return 
 */
int SetValueInt8(MqttDevice* d, sint64_t aid, sint64_t iid, int8_t value);
/**
 * 
 * @param d
 * @param aid
 * @param iid
 * @param value
 * @return 
 */
int SetValueUInt16(MqttDevice* d, sint64_t aid, sint64_t iid, uint16_t value);
/**
 * 
 * @param d
 * @param aid
 * @param iid
 * @param value
 * @return 
 */
int SetValueInt16(MqttDevice* d, sint64_t aid, sint64_t iid, int16_t value);
/**
 * 
 * @param d
 * @param aid
 * @param iid
 * @param value
 * @return 
 */
int SetValueUInt32(MqttDevice* d, sint64_t aid, sint64_t iid, uint32_t value);
/**
 * 
 * @param d
 * @param aid
 * @param iid
 * @param value
 * @return 
 */
int SetValueInt32(MqttDevice* d, sint64_t aid, sint64_t iid, int32_t value);
/**
 * 
 * @param d
 * @param aid
 * @param iid
 * @param value
 * @return 
 */
int SetValueUInt64(MqttDevice* d, sint64_t aid, sint64_t iid, uint64_t value);
/**
 * 
 * @param d
 * @param aid
 * @param iid
 * @param value
 * @return 
 */
int SetValueFloat(MqttDevice* d, sint64_t aid, sint64_t iid, double value);
/**
 * 
 * @param d
 * @param aid
 * @param iid
 * @param format
 * @param value
 * @param c
 * @return 
 */
int setValue(MqttDevice* d, sint64_t aid, sint64_t iid, CharacteristicFormat format, CharacteristicValue* value, Characteristic* c);
/**
 * 
 * @param d
 */
void devicePublish(MqttDevice* d);
/**
 * 
 * @param d
 */
void deviceSubscribe(MqttDevice* d);
/**
 * 
 * @param d
 * @param actorId
 * @param feedId
 * @param msg
 */
void onValueUpdate(MqttDevice* d, const char* actorId, const char* feedId, const char* msg);

#ifdef	__cplusplus
}
#endif

#endif	/* SERVICE_DEVICE_H */

