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

#include "service_device.h"
#include "topics.h"
#include <github.com/mikejac/bluemix.esp8266-nonos.cpp/bluemix.h>
#include <github.com/mikejac/misc.esp8266-nonos.cpp/espmissingincludes.h>
#include <osapi.h>
#include <mem.h>

#define DTXT(...)   os_printf(__VA_ARGS__)
//#define DTXT(...)

/******************************************************************************************************************
 * 
 *
 */

static STAILQ_HEAD(deviceStailhead, Device_Message) deviceHead = STAILQ_HEAD_INITIALIZER(deviceHead);

/******************************************************************************************************************
 * prototypes
 *
 */

/**
 * 
 * @param msg
 * @param aid
 * @param iid
 * @return 
 */
static int unmarshalValue(const char* msg, uint64_t* aid, uint64_t* iid);

/******************************************************************************************************************
 * public functions
 *
 */

/**
 * 
 * @param parent
 * @return 
 */
MqttDevice* ICACHE_FLASH_ATTR NewDevice(Mqtt* parent)
{
    if(parent == 0) {
        DTXT("NewDevice(MqttDevice): 'parent' is nil\n");
        return 0;
    }

    MqttDevice* d = (MqttDevice*) os_zalloc(sizeof(MqttDevice));
    if(d == 0) {
        DTXT("NewDevice(MqttDevice): mem fail\n");
        return 0;
    }
    
    d->parent = parent;
    d->qos    = 0;

    STAILQ_INIT(&deviceHead);                     // initialize the queue
    
    return d;
}
/**
 * 
 * @param d
 * @param container
 * @return 
 */
int ICACHE_FLASH_ATTR SetAccessories(MqttDevice* d, Container* container)
{
    if(d == 0) {
        DTXT("SetAccessories(): 'd' is nil\n");
        return -1;
    }

    if(container == 0) {
        DTXT("SetAccessories(): 'container' is nil\n");
        return -1;
    }
    
    if(d->parent->svcDevice != 0) {
        DTXT("SetAccessories(): service device already registered\n");
        return -1;
    }
    
    container->parent = d;
    
    // save/take ownership of the container
    d->container = container;
    
    // link ourselves into MqttConnector
    d->parent->svcDevice = d;
    
    deviceSubscribe(d);
    
    return 0;
}
/**
 * 
 * @param client
 * @return 
 */
Device_Message* ICACHE_FLASH_ATTR DeviceGetEvent(MqttDevice* d)
{
    return STAILQ_FIRST(&deviceHead);
}
/**
 * 
 * @param msg
 * @return 
 */
CharacteristicFormat ICACHE_FLASH_ATTR DeviceGetEventType(Device_Message* msg)
{
    if(msg == NULL) {
        return FormatNone;
    }
    
    return msg->format;
}
/**
 * 
 * @param msg
 */
void ICACHE_FLASH_ATTR DeviceDeleteEvent(Device_Message* msg)
{
    if(msg == NULL) {
        return;
    }

    switch(msg->format) {
        case FormatString:
            if(msg->value.String)   os_free(msg->value.String);
            
            STAILQ_REMOVE(&deviceHead, msg, Device_Message, entries);
            
            os_free(msg);
            break;

        case FormatBool:    
        case FormatUInt8:   
        case FormatInt8:    
        case FormatUInt16:  
        case FormatInt16:   
        case FormatUInt32:  
        case FormatInt32:   
        case FormatUInt64:  
        case FormatFloat:   
            STAILQ_REMOVE(&deviceHead, msg, Device_Message, entries);
            
            os_free(msg);
            break;
            
        case FormatNone:
            break;
    }
}

/******************************************************************************************************************
 * value updates to HomeKit app
 *
 */

/**
 * 
 * @param d
 * @param aid
 * @param iid
 * @param value
 * @return 
 */
int ICACHE_FLASH_ATTR SetValueString(MqttDevice* d, sint64_t aid, sint64_t iid, const char* value)
{
    CharacteristicValue v;
    v.String = (char*) value;
    
    return setValue(d, aid, iid, FormatString, &v, NULL);
}
/**
 * 
 * @param d
 * @param aid
 * @param iid
 * @param value
 * @return 
 */
int ICACHE_FLASH_ATTR SetValueBool(MqttDevice* d, sint64_t aid, sint64_t iid, bool value)
{
    CharacteristicValue v;
    v.Bool = value;
    
    return setValue(d, aid, iid, FormatBool, &v, NULL);
}
/**
 * 
 * @param d
 * @param aid
 * @param iid
 * @param value
 * @return 
 */
int ICACHE_FLASH_ATTR SetValueUInt8(MqttDevice* d, sint64_t aid, sint64_t iid, uint8_t value)
{
    CharacteristicValue v;
    v.UInt8 = value;
    
    return setValue(d, aid, iid, FormatUInt8, &v, NULL);
}
/**
 * 
 * @param d
 * @param aid
 * @param iid
 * @param value
 * @return 
 */
int ICACHE_FLASH_ATTR SetValueInt8(MqttDevice* d, sint64_t aid, sint64_t iid, int8_t value)
{
    CharacteristicValue v;
    v.Int8 = value;
    
    return setValue(d, aid, iid, FormatInt8, &v, NULL);
}
/**
 * 
 * @param d
 * @param aid
 * @param iid
 * @param value
 * @return 
 */
int ICACHE_FLASH_ATTR SetValueUInt16(MqttDevice* d, sint64_t aid, sint64_t iid, uint16_t value)
{
    CharacteristicValue v;
    v.UInt16 = value;
    
    return setValue(d, aid, iid, FormatUInt16, &v, NULL);
}
/**
 * 
 * @param d
 * @param aid
 * @param iid
 * @param value
 * @return 
 */
int ICACHE_FLASH_ATTR SetValueInt16(MqttDevice* d, sint64_t aid, sint64_t iid, int16_t value)
{
    CharacteristicValue v;
    v.Int16 = value;
    
    return setValue(d, aid, iid, FormatInt16, &v, NULL);
}
/**
 * 
 * @param d
 * @param aid
 * @param iid
 * @param value
 * @return 
 */
int ICACHE_FLASH_ATTR SetValueUInt32(MqttDevice* d, sint64_t aid, sint64_t iid, uint32_t value)
{
    CharacteristicValue v;
    v.UInt32 = value;
    
    return setValue(d, aid, iid, FormatUInt32, &v, NULL);
}
/**
 * 
 * @param d
 * @param aid
 * @param iid
 * @param value
 * @return 
 */
int ICACHE_FLASH_ATTR SetValueInt32(MqttDevice* d, sint64_t aid, sint64_t iid, int32_t value)
{
    CharacteristicValue v;
    v.Int32 = value;
    
    return setValue(d, aid, iid, FormatInt32, &v, NULL);
}
/**
 * 
 * @param d
 * @param aid
 * @param iid
 * @param value
 * @return 
 */
int ICACHE_FLASH_ATTR SetValueUInt64(MqttDevice* d, sint64_t aid, sint64_t iid, uint64_t value)
{
    CharacteristicValue v;
    v.UInt64 = value;
    
    return setValue(d, aid, iid, FormatUInt64, &v, NULL);
}
/**
 * 
 * @param d
 * @param aid
 * @param iid
 * @param value
 * @return 
 */
int ICACHE_FLASH_ATTR SetValueFloat(MqttDevice* d, sint64_t aid, sint64_t iid, double value)
{
    CharacteristicValue v;
    v.Float = value;
    
    return setValue(d, aid, iid, FormatFloat, &v, NULL);
}
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
int ICACHE_FLASH_ATTR setValue(MqttDevice* d, sint64_t aid, sint64_t iid, CharacteristicFormat format, CharacteristicValue* value, Characteristic* c)
{
    if(c == 0) {
        Accessory* a = FindByAid(d->container, aid);
        if(a == 0) {
            DTXT("setValue(FindByAid): not found\n");
            return -1;
        }

        c = FindCharacteristicByIid(a, iid);
        if(c == 0) {
            DTXT("setValue(FindCharacteristicByIid): not found\n");
            return -1;
        }
    }
    
    // update the value
    characteristicSetValue(c, format, value);
    
    char* msg = MarshalValue(aid, iid, format, value);
    if(msg == 0) {
        DTXT("setValue(MarshalValue): failed\n");
        return -1;
    }
    
    DTXT("setValue(): msg = '%s'\n", msg);

    const char* formatTxt = NULL;
    
    switch(format) {
        case FormatString:
            formatTxt = FormatStringTxt;
            break;

        case FormatBool:
            formatTxt = FormatBoolTxt;
            break;
            
        case FormatUInt8:
            formatTxt = FormatUInt8Txt;
            break;
            
        case FormatInt8:
            formatTxt = FormatInt8Txt;
            break;
            
        case FormatUInt16:
            formatTxt = FormatUInt16Txt;
            break;
            
        case FormatInt16:
            formatTxt = FormatInt16Txt;
            break;
            
        case FormatUInt32:
            formatTxt = FormatUInt32Txt;
            break;
            
        case FormatInt32:
            formatTxt = FormatInt32Txt;
            break;
            
        case FormatUInt64:
            formatTxt = FormatUInt64Txt;
            break;
            
        case FormatFloat:   
            formatTxt = FormatFloatTxt;
            break;
            
        case FormatNone:
            break;
    }
    
    char* topic = (char*) os_malloc(topicOfframpPublish(d->parent, 
                                                        fabricNodenameBroadcast,		// destination nodename 
                                                        fabricTaskIdService, 			// taskId 
                                                        GetPlatformId(d->parent),		// platformId
                                                        fabricServiceIdToHK,                    // serviceId 
                                                        formatTxt,
                                                        0));
    
    if(topic == 0) {
        DTXT("setValue(topic): mem fail\n");
        os_free(msg);
        return -1;
    }
    
    topicOfframpPublish(d->parent, 
                        fabricNodenameBroadcast,		// destination nodename 
                        fabricTaskIdService, 			// taskId 
                        GetPlatformId(d->parent),		// platformId
                        fabricServiceIdToHK,                    // serviceId 
                        formatTxt,
                        topic);

    DTXT("setValue(): topic = '%s'\n", topic);

    MQTT_Publish(&d->parent->client, topic, msg, os_strlen(msg), d->qos, 0);

    os_free(topic);
    os_free(msg);

    return 0;
}
/**
 * 
 * @param d
 */
void ICACHE_FLASH_ATTR devicePublish(MqttDevice* d)
{
    char* topic = (char*) os_malloc(topicOfframpPublish(d->parent, 
                                                        fabricNodenameBroadcast,		// destination nodename 
                                                        fabricTaskIdService, 			// taskId 
                                                        GetPlatformId(d->parent),		// platformId
                                                        fabricServiceIdAccessories,		// serviceId 
                                                        "list",
                                                        0));
    
    if(topic == 0) {
        DTXT("devicePublish(topic): mem fail\n");
        return;
    }
    
    topicOfframpPublish(d->parent, 
                        fabricNodenameBroadcast,		// destination nodename 
                        fabricTaskIdService, 			// taskId 
                        GetPlatformId(d->parent),		// platformId
                        fabricServiceIdAccessories,		// serviceId 
                        "list",
                        topic);

    DTXT("devicePublish(): topic = '%s'\n", topic);

    char* msg = marshalContainer(d->container);
    if(msg == 0) {
        DTXT("devicePublish(): marshal fail\n");
    } else {
        DTXT("devicePublish(): len = %d\n", os_strlen(msg));
        DTXT("devicePublish(): %s\n", msg);
        MQTT_Publish(&d->parent->client, topic, msg, os_strlen(msg), d->qos, 0);
        os_free(msg);
    }

    os_free(topic);
}
/**
 * 
 * @param d
 */
void ICACHE_FLASH_ATTR deviceSubscribe(MqttDevice* d)
{
    char* topic = (char*) os_malloc(topicOfframpSubscribe(  d->parent, 
                                                            GetNodename(d->parent), 
                                                            fabricTopicAny,             // actorID == senders nodename
                                                            fabricTopicAny,             // actorPlatformID == senders platformId
                                                            fabricTaskIdService,        // taskId 
                                                            fabricTopicAny,             // platformId == aid
                                                            fabricServiceIdFromHK,      // serviceId
                                                            fabricTopicAny,             // feedId
                                                            0));
    
    if(topic == 0) {
        DTXT("deviceSubscribe(topic): mem fail\n");
        return;
    }
    
    topicOfframpSubscribe(  d->parent, 
                            GetNodename(d->parent), 
                            fabricTopicAny,             // actorID == senders nodename
                            fabricTopicAny,             // actorPlatformID == senders platformId
                            fabricTaskIdService,        // taskId 
                            fabricTopicAny,             // platformId 
                            fabricServiceIdFromHK,      // serviceId
                            fabricTopicAny,             // feedId
                            topic);
    
    MQTT_Subscribe(&d->parent->client, topic, d->qos);

    DTXT("deviceSubscribe(): topic = '%s'\n", topic);
    
    os_free(topic);
}

/******************************************************************************************************************
 * value updates from HomeKit app
 *
 */

/**
 * 
 * @param d
 * @param actorId
 * @param feedId
 * @param msg
 */
void ICACHE_FLASH_ATTR onValueUpdate(MqttDevice* d, const char* actorId, const char* feedId, const char* msg)
{
    uint64_t aid;
    uint64_t iid;
    
    if(unmarshalValue(msg, &aid, &iid) == 0) {
        if(os_strcmp(feedId, FormatStringTxt) == 0) {

        } else if(os_strcmp(feedId, FormatBoolTxt) == 0) {
            bool value;

            if(BMix_GetBool("value", &value) == 0) {
                DTXT("onValueUpdate(): bool; value = %s\n", (value == true) ? "True" : "False");

                // append to queue
                Device_Message* msg = os_malloc(sizeof(Device_Message));
                if(msg != 0) {
                    msg->aid        = aid;
                    msg->iid        = iid;
                    msg->acc        = FindByAid(d->container, aid);
                    msg->format     = FormatBool;
                    msg->value.Bool = value;

                    STAILQ_INSERT_TAIL(&deviceHead, msg, entries);                                // insert at end
                }
            }
        } else if(os_strcmp(feedId, FormatFloatTxt) == 0) {
            double value;

            if(BMix_GetDouble("value", &value) == 0) {
                DTXT("onValueUpdate(): float");
                
                // append to queue
                Device_Message* msg = os_malloc(sizeof(Device_Message));
                if(msg != 0) {
                    msg->aid         = aid;
                    msg->iid         = iid;
                    msg->acc         = FindByAid(d->container, aid);
                    msg->format      = FormatFloat;
                    msg->value.Float = value;

                    STAILQ_INSERT_TAIL(&deviceHead, msg, entries);                                // insert at end
                }
            }
        } else if(os_strcmp(feedId, FormatUInt8Txt) == 0) {
            int value;

            if(BMix_GetInt("value", &value) == 0) {
                DTXT("onValueUpdate(): uint8; value = %d\n", value);

                // append to queue
                Device_Message* msg = os_malloc(sizeof(Device_Message));
                if(msg != 0) {
                    msg->aid         = aid;
                    msg->iid         = iid;
                    msg->acc         = FindByAid(d->container, aid);
                    msg->format      = FormatUInt8;
                    msg->value.UInt8 = (uint8_t)value;

                    STAILQ_INSERT_TAIL(&deviceHead, msg, entries);                                // insert at end
                }
            }
        } else if(os_strcmp(feedId, FormatUInt16Txt) == 0) {

        } else if(os_strcmp(feedId, FormatUInt32Txt) == 0) {

        } else if(os_strcmp(feedId, FormatInt32Txt) == 0) {

        } else if(os_strcmp(feedId, FormatUInt64Txt) == 0) {

        } else if(os_strcmp(feedId, FormatDataTxt) == 0) {
            DTXT("onValueUpdate(): unhandled format; '%s'\n", feedId);
        } else if(os_strcmp(feedId, FormatTLV8Txt) == 0) {
            DTXT("onValueUpdate(): unhandled format; '%s'\n", feedId);
        } else if(os_strcmp(feedId, FormatInt8Txt) == 0) {

        } else if(os_strcmp(feedId, FormatInt16Txt) == 0) {

        } else {
            DTXT("onValueUpdate(): unknown format; '%s'\n", feedId);
        }
    }
}
/**
 * 
 * @param msg
 * @param aid
 * @param iid
 * @return 
 */
int unmarshalValue(const char* msg, uint64_t* aid, uint64_t* iid)
{
    int ret = -1;
    
    DTXT("unmarshalValue(): %s\n", msg);
    
    if(BMix_DecoderBegin(msg) != NULL) {
        const char* _type;
        
        if(BMix_GetString("_type", &_type) != 0) {
            DTXT("unmarshalValue(): '_type' not found\n");
        } else if(BMix_GetU64("aid", aid) != 0) {
            DTXT("unmarshalValue(): 'aid' not found\n");
        } else if(BMix_GetU64("iid", iid) != 0) {
            DTXT("unmarshalValue(): 'iid' not found\n");
        } else {
            DTXT("unmarshalValue(): all fields found\n");
            
            ret = 0;
        }
    }
    
    BMix_DecoderEnd();
    
    return ret;
}
