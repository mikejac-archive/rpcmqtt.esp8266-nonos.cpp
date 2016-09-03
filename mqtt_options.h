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

#ifndef MQTT_OPTIONS_H
#define	MQTT_OPTIONS_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct MqttOptions MqttOptions;
 
typedef enum {
    ClassTypeInvalid        = 0,
    ClassTypeDevice         = 1,
    ClassTypeController     = 2,
    ClassTypeDeviceSvc      = 3,
    ClassTypeControllerSvc  = 4
} ClassType;


struct MqttOptions {
    char*           Server;             // name or ip of the MQTT server
    int             Port;               // portnumber of the MQTT server (usually 1883)
    char*           ClientId;           // MQTT client id
    int             Keepalive;          // MQTT keep-alive interval in seconds
    char*           RootTopic;          // Very first part of all MQTT topics
    char*           Nodename;           // Our nodename
    char*           ActorPlatformId;    // Our platform id
    ClassType       ClassType;          // What are we
    
    unsigned char   RetainStatus;
    
    int             BufferSize;
};

/******************************************************************************************************************
 * 
 *
 */

#define MqttOptions_SetServer(options, value)           options->Server          = (char*)(value)
#define MqttOptions_SetPort(options, value)             options->Port            = (value)
#define MqttOptions_SetClientId(options, value)         options->ClientId        = (char*)(value)
#define MqttOptions_SetKeepalive(options, value)        options->Keepalive       = (value)
#define MqttOptions_SetRootTopic(options, value)        options->RootTopic       = (char*)(value)
#define MqttOptions_SetNodename(options, value)         options->Nodename        = (char*)(value)
#define MqttOptions_SetActorPlatformId(options, value)  options->ActorPlatformId = (char*)(value)
#define MqttOptions_SetClassType(options, value)        options->ClassType       = (value)
#define MqttOptions_SetBufferSize(options, value)       options->BufferSize      = (value)

/******************************************************************************************************************
 * prototypes
 *
 */

/**
 * 
 * @return 
 */
MqttOptions* NewMqttOptions(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MQTT_OPTIONS_H */

