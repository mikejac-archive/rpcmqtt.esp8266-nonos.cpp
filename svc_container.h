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

#ifndef SVC_CONTAINER_H
#define	SVC_CONTAINER_H

#include "svc_accessory.h"

#ifdef	__cplusplus
extern "C" {
#endif

/******************************************************************************************************************
 * 
 *
 */
    
typedef struct MqttDevice MqttDevice;

/******************************************************************************************************************
 * 
 *
 */

typedef struct Container Container;
 
struct Container {
    const char* Nodename;               // "nodename"
    const char* Name;                   // "name"
    const char* SerialNumber;           // "serialnumber"
    const char* Manufacturer;           // "manufacturer"
    const char* Model;                  // "model"

    Accessory*  Accessories;            // "value"
    int         idCount;
    
    int         marshalBufferSize;
    MqttDevice* parent;
};

/******************************************************************************************************************
 * prototypes
 *
 */

/**
 * 
 * @param nodename
 * @param name
 * @param serialnumber
 * @param manufacturer
 * @param model
 * @param marshalBufferSize
 * @return 
 */
Container* NewContainer(const char* nodename, const char* name, const char* serialnumber, const char* manufacturer, const char* model, int marshalBufferSize);
/**
 * AddAccessory adds an accessory to the container.
 * This method ensures that the accessory ids are valid and unique withing the container.
 * @param cont
 * @param a
 * @return 
 */
sint64_t AddAccessory(Container* cont, Accessory* a);
/**
 * 
 * @param cont
 * @param aid
 * @return 
 */
Accessory* FindByAid(Container* cont, sint64_t aid);
/**
 * 
 * @param aid
 * @param iid
 * @param format
 * @param value
 * @return 
 */
char* MarshalValue(sint64_t aid, sint64_t iid, CharacteristicFormat format, CharacteristicValue* value);
/**
 * 
 * @param cont
 * @return 
 */
char* marshalContainer(Container* cont);

#ifdef	__cplusplus
}
#endif

#endif	/* SVC_CONTAINER_H */
