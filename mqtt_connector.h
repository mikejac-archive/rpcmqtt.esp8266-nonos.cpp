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

#ifndef MQTT_CONNECTOR_H
#define	MQTT_CONNECTOR_H

#ifdef	__cplusplus
extern "C" {
#endif
  
#include "mqtt_options.h"
#include <github.com/mikejac/misc.esp8266-nonos.cpp/espmissingincludes.h>
#include <github.com/mikejac/mqtt.esp8266-nonos.cpp/mqtt_client.h>
#include <queue.h>
    
/******************************************************************************************************************
 * common actor id's
 *
 */
#define ActorIdUpgrader                 "firmware"
    
/******************************************************************************************************************
 * common platform id's
 *
 */
#define PlatformIdChronos               "time"
#define PlatformIdUpgrader              "esp8266upgrader"
#define PlatformIdPackage               "esp8266package"

/******************************************************************************************************************
 * common service id's
 *
 */
#define ServiceIdChronos                "chronos"

/******************************************************************************************************************
 * common task id's
 *
 */

/******************************************************************************************************************
 * common feed id's
 *
 */
#define FeedIdSeconds                   "seconds"
#define FeedIdDebugInfo                 "info"
#define FeedIdDebugWarning              "warn"
#define FeedIdDebugError                "err"

/******************************************************************************************************************
 * 
 *
 */

#define RUN_NO_EVENTS                   0
#define RUN_CONNECTED                   1
#define RUN_DISCONNECTED                2
    
typedef struct Mqtt Mqtt;

#define Info(m, d)                      DebugPublish((m), FeedIdDebugInfo, (d))
#define Warning(m, d)                   DebugPublish((m), FeedIdDebugWarning, (d))
#define Error(m, d)                     DebugPublish((m), FeedIdDebugError, (d))

/******************************************************************************************************************
 * callbacks
 *
 */

typedef void (*OnCommandCallback)(  Mqtt* mqtt, 
                                    void*              ptr,
                                    const char*        nodename,
                                    const char*        actorId,
                                    const char*        platformId,
                                    const char*        feedId,
                                    const char*        payload);

/******************************************************************************************************************
 * 
 *
 */

typedef struct MqttDevice MqttDevice;
    
typedef enum {
    fabricStatusInvalid      = 0,
    fabricStatusOnline       = 1,
    fabricStatusOffline      = 2,
    fabricStatusDisconnected = 3
} fabricStatus;

//
struct Mqtt {
    // MQTT
    MQTT_Client         client;
    int                 shutdown;
    
    // callbacks
    OnCommandCallback   onCommandCallback;
    void*               userdata;
    
    char*               server;
    int                 port;
    
    // Fabric data
    char*               rootTopic;
    char*               actorId;
    char*               actorPlatformId;
    ClassType           classType;	

    // clock
    const char*         chronosNodename;
    
    // service
    MqttDevice*         svcDevice;
};

/******************************************************************************************************************
 * prototypes
 *
 */

/**
 * 
 * @param options
 * @return 
 */
Mqtt* Connector(MqttOptions* options);
/**
 * 
 * @param mqtt
 * @return 
 */
int Close(Mqtt* mqtt);
/**
 * 
 * @param mqtt
 */
int ConnectorRun(Mqtt* mqtt);
/**
 * 
 * @param mqtt
 * @param ptr
 * @param onCommandCallback
 * @return 
 */
int InstallCallbacks(   Mqtt*              mqtt,
                        void*               ptr,
                        OnCommandCallback   onCommandCallback);
/**
 * 
 * @param mqtt
 * @param chronosNodename
 * @return 
 */
int EnableChronos(Mqtt* mqtt, const char* chronosNodename);
/**
 * 
 * @param mqtt
 * @return 
 */
const char* GetNodename(Mqtt* mqtt);
/**
 * 
 * @param mqtt
 * @return 
 */
const char* GetPlatformId(Mqtt* mqtt);
/**
 * 
 * @param mqtt
 * @return 
 */
int IsConnected(Mqtt* mqtt);
/**
 * 
 * @param mqtt
 * @param nodename
 * @param actorId
 * @param platformId
 * @param feedId
 * @param qos
 * @return 
 */
int CommandSubscription(Mqtt* mqtt, const char* nodename, const char* actorId, const char* platformId, const char* feedId, int qos);
/**
 * 
 * @param mqtt
 * @param actorId
 * @param platformId
 * @param feedId
 * @param data
 * @param qos
 * @param retain
 * @return 
 */
int CommandPublish(Mqtt* mqtt, const char* actorId, const char* platformId, const char* feedId, const char* data, int qos, int retain);
/**
 * 
 * @param mqtt
 * @param feedId
 * @param data
 * @return 
 */
int DebugPublish(Mqtt* mqtt, const char* feedId, const char* data);

#ifdef	__cplusplus
}
#endif

#endif	/* MQTT_CONNECTOR_H */

