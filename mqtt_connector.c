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

#include "mqtt_connector.h"
#include "topics.h"
#include "service_device.h"
#include <github.com/mikejac/date_time.esp8266-nonos.cpp/system_time.h>
#include <github.com/mikejac/wifi.esp8266-nonos.cpp/wifi.h>
#include <github.com/mikejac/bluemix.esp8266-nonos.cpp/bluemix.h>
#include <github.com/mikejac/misc.esp8266-nonos.cpp/espmissingincludes.h>
#include <osapi.h>
#include <mem.h>

#define DTXT(...)   os_printf(__VA_ARGS__)
//#define DTXT(...)

#define classTypeDeviceTxt		"device"
#define classTypeControllerTxt		"controller"
#define classTypeDeviceSvcTxt 		"device_svc"
#define classTypeControllerSvcTxt	"controller_svc"

#define fabricStatusQos                 0 // 2
#define fabricStatusRetain              1

/******************************************************************************************************************
 * 
 *
 */

typedef enum {
    none,
    offramp,
    onramp,
    command,
    connect,
    disconnect,
    svcCtrlOnline,
    svcFromHK
} MqttFabric_MessageType;

typedef struct {
    char* nodename;
    char* actor_id;
    char* actor_platform_id;
    char* task_id;
    char* platform_id;
    char* service_id;
    char* feed_id;
} MqttFabric_MessageOfframp;

typedef struct {
    char* nodename;
    char* actor_id;
    char* platform_id;
    char* feed_id;
} MqttFabric_MessageCommand;

typedef struct {
    char* actor_id;     // senders nodename
    char* feed_id;      // format
} MqttFabric_MessageFromHk;

#define MqttFabric_GetOfframp_Nodename(m)           (m->m_Message.m_MessageOfframp.nodename)
#define MqttFabric_GetOfframp_ActorId(m)            (m->m_Message.m_MessageOfframp.actor_id)
#define MqttFabric_GetOfframp_ActorPlatformId(m)    (m->m_Message.m_MessageOfframp.actor_platform_id)
#define MqttFabric_GetOfframp_TaskId(m)             (m->m_Message.m_MessageOfframp.task_id)
#define MqttFabric_GetOfframp_PlatformId(m)         (m->m_Message.m_MessageOfframp.platform_id)
#define MqttFabric_GetOfframp_ServiceId(m)          (m->m_Message.m_MessageOfframp.service_id)
#define MqttFabric_GetOfframp_FeedId(m)             (m->m_Message.m_MessageOfframp.feed_id)
#define MqttFabric_GetOfframp_Payload(m)            (m->m_Payload)
#define MqttFabric_GetOfframp_PayloadLen(m)         (m->m_PayloadLen)

#define MqttFabric_GetCommand_Nodename(m)           (m->m_Message.m_MessageCommand.nodename)
#define MqttFabric_GetCommand_ActorId(m)            (m->m_Message.m_MessageCommand.actor_id)
#define MqttFabric_GetCommand_PlatformId(m)         (m->m_Message.m_MessageCommand.platform_id)
#define MqttFabric_GetCommand_FeedId(m)             (m->m_Message.m_MessageCommand.feed_id)
#define MqttFabric_GetCommand_Payload(m)            (m->m_Payload)
#define MqttFabric_GetCommand_PayloadLen(m)         (m->m_PayloadLen)

#define MqttFabric_GetFromHK_ActorId(m)             (m->m_Message.m_MessageFromHK.actor_id)
#define MqttFabric_GetFromHK_FeedId(m)              (m->m_Message.m_MessageFromHK.feed_id)
#define MqttFabric_GetFromHK_Payload(m)             (m->m_Payload)

typedef struct MqttFabric_Message MqttFabric_Message;

struct MqttFabric_Message {
    STAILQ_ENTRY(MqttFabric_Message) entries;    // tail queue
            
    MqttFabric_MessageType m_MessageType;
    
    union {
        MqttFabric_MessageOfframp m_MessageOfframp;
        MqttFabric_MessageCommand m_MessageCommand;
        MqttFabric_MessageFromHk  m_MessageFromHK;
    } m_Message;
    
    char*    m_Payload;
    uint32_t m_PayloadLen;
};

static STAILQ_HEAD(mqttStailhead, MqttFabric_Message) mqttHead = STAILQ_HEAD_INITIALIZER(mqttHead);

/******************************************************************************************************************
 * prototypes
 *
 */

/**
 * 
 * @param mqtt
 * @return 
 */
static MqttFabric_Message* GetEvent(Mqtt* mqtt);
/**
 * 
 * @param msg
 * @return 
 */
static MqttFabric_MessageType GetEventType(MqttFabric_Message* msg);
/**
 * 
 * @param msg
 */
void ICACHE_FLASH_ATTR DeleteEvent(MqttFabric_Message* msg);

/**
 * 
 * @param sessionPresent
 * @param connack_rc
 * @param args
 */
static void onConnect(unsigned char sessionPresent, unsigned char connack_rc, void* args);
/**
 * 
 * @param args
 */
static void onDisconnect(void* args);
/**
 * 
 * @param topic
 * @param payload
 * @param payloadlen
 * @param qos
 * @param retained
 * @param dup
 * @param ptr
 */
static void onMessage(const char* topic, const unsigned char* payload, int payloadlen, int qos, unsigned char retained, unsigned char dup, void* ptr);
/**
 * 
 * @param mqtt
 * @param nodename
 * @param actorId
 * @param platformId
 * @param feedId
 * @param payload
 * @param payloadlen
 */
static void onCommandHandler(   Mqtt*                   mqtt, 
                                const char*             nodename, 
                                const char*             actorId, 
                                const char*             platformId, 
                                const char*             feedId, 
                                const unsigned char*    payload, 
                                int                     payloadlen);
/**
 * 
 * @param mqtt
 * @param nodename
 * @param actorId
 * @param actorPlatformId
 * @param taskId
 * @param platformId
 * @param serviceId
 * @param feedId
 * @param msg
 */
static void onOfframpHandler(   Mqtt*                   mqtt, 
                                const char*             nodename,
                                const char*             actorId,
                                const char*             actorPlatformId,
                                const char*             taskId,
                                const char*             platformId,
                                const char*             serviceId,
                                const char*             feedId,
                                const unsigned char*    payload, 
                                int                     payloadlen);
/**
 * 
 * @param mqtt
 * @param fabricStatus
 * @param seconds
 * @param topic
 * @param msg
 * @return 
 */
static int statusMessage(Mqtt* mqtt, fabricStatus fabricStatus, int seconds, char** topic, char** msg);
/**
 * 
 * @param mqtt
 */
static void chronosSubscribe(Mqtt* mqtt);
/**
 * 
 * @param dest
 * @param source
 * @return 
 */
static char* strcpy_alloc(char** dest, const char* source);
/**
 * 
 * @param name
 * @param ipaddr
 * @param callback_arg
 */
static void _dns_found_callback(const char* name, ip_addr_t* ipaddr, void* callback_arg);

/******************************************************************************************************************
 * public functions
 *
 */

/**
 * 
 * @param options
 * @return 
 */
Mqtt* ICACHE_FLASH_ATTR Connector(MqttOptions* options) {
    DTXT("Connector(): begin\n");
    
    Mqtt* mqtt = (Mqtt*) os_zalloc(sizeof(Mqtt));
    if(mqtt == 0) {
        DTXT("Connector(1): mem error\n");
        return 0;
    }

    if(strcpy_alloc(&mqtt->server, options->Server) == 0) {
        goto defer;
    }
    
    if(strcpy_alloc(&mqtt->rootTopic, options->RootTopic) == 0) {
        goto defer;
    }
    
    if(strcpy_alloc(&mqtt->actorId, options->Nodename) == 0) {
        goto defer;
    }
    
    if(strcpy_alloc(&mqtt->actorPlatformId, options->ActorPlatformId) == 0) {
        goto defer;
    }
    
    BMix_Initialize();

    mqtt->port      = options->Port;
    mqtt->classType = options->ClassType;

    STAILQ_INIT(&mqttHead);                     // initialize the queue
    
    // create last-will-and-testament topic and message
    char* lwt_topic; char* lwt_msg;
    if(statusMessage(mqtt, fabricStatusDisconnected, 0, &lwt_topic, &lwt_msg) != 0) {
        goto defer;
    }
    
    MQTT_InitConnection(&mqtt->client, 
                        options->ClientId,      // client id
                        options->Keepalive,     // keepalive interval (seconds))
                        1,                      // clean session
                        mqtt,                   // user data
                        options->BufferSize);
    
    MQTT_InitLWT(&mqtt->client, 
                lwt_topic,                      // topic
                lwt_msg,                        // message
                fabricStatusQos,                // QoS
                fabricStatusRetain);            // retain

    MQTT_OnConnected(&mqtt->client, onConnect);
    MQTT_OnDisconnected(&mqtt->client, onDisconnect);
    MQTT_OnPublish(&mqtt->client, onMessage);
    
    DTXT("Connector(): end\n");

    return mqtt;
    
defer:
    DTXT("Connector(): defer!\n");

    if(lwt_topic) {
        os_free(lwt_topic);
    }
    if(lwt_msg) {
        os_free(lwt_msg);
    }
    if(mqtt->rootTopic) {
        os_free(mqtt->rootTopic);
    }
    if(mqtt->actorId) {
        os_free(mqtt->actorId);
    }
    if(mqtt->actorPlatformId) {
        os_free(mqtt->actorPlatformId);
    }

    os_free(mqtt);

    return 0;
}
/**
 * 
 * @param mqtt
 * @return 
 */
int ICACHE_FLASH_ATTR Close(Mqtt* mqtt)
{
    int ret = 0;
    
    if(mqtt->shutdown == 0) {
        mqtt->shutdown = 1;

        if(MQTT_IsConnected(&mqtt->client)) {
            DTXT("Close(): connected, send status message and disconnect\n");
            
            // send status message
            char* topic; char* msg;
            if(statusMessage(mqtt, fabricStatusOffline, (int) esp_uptime(0), &topic, &msg) == 0) {
                MQTT_Publish(&mqtt->client, topic, msg, os_strlen(msg), fabricStatusQos, fabricStatusRetain);
            }
            if(topic != 0) {
                os_free(topic);
            }
            if(msg != 0) {
                os_free(msg);
            }

            // also send disconnect
            MQTT_Disconnect(&mqtt->client);
        }
        else {
            // not connected so nothing more to do
            ret = 1;
        }
    }
    else if(mqtt->shutdown == 1) {
        DTXT("Close(): wait for transmit queue to become empty\n");
        // wait for transmit queue to become empty
        if(STAILQ_FIRST(&mqtt->client.m_QueueIngress) == NULL) {
            mqtt->shutdown = 2;
        }
    }
    else if(mqtt->shutdown == 2) {
        //DTXT("Close(): transmit queue is empty\n");
        if(MQTT_IsConnected(&mqtt->client) == 0) {
            ret = 1;
        }
    }
    
    return ret;
}
/**
 * 
 * @param mqtt
 * @return 
 */
int ICACHE_FLASH_ATTR ConnectorRun(Mqtt* mqtt)
{
    int ret = RUN_NO_EVENTS;
    
    if(mqtt == 0) {
        return ret;
    }
    
    static int wifi_connected = 0;
    
    WIFI_Run();

    if(WIFI_IsConnected() == 1 && wifi_connected == 0) {
        DTXT("ConnectorRun(): WiFi has been connected; uptime = %lu\n", (unsigned long) esp_uptime(0));
        
        wifi_connected = 1;
        
        err_t result = espconn_gethostbyname(&mqtt->client.m_Conn, mqtt->server, &mqtt->client.m_IpAddr, _dns_found_callback);
        if(result == ESPCONN_OK) {
            DTXT("ConnectorRun(espconn_gethostbyname): ESPCONN_OK\n");

            wifi_connected = 3;
        } else if(result == ESPCONN_INPROGRESS) {
            DTXT("ConnectorRun(espconn_gethostbyname): ESPCONN_INPROGRESS\n");
            
            wifi_connected = 2;
        } else if(result == ESPCONN_ARG) {
            DTXT("ConnectorRun(espconn_gethostbyname): ESPCONN_ARG\n");
            
            wifi_connected = 99;
        } else {
            DTXT("ConnectorRun(espconn_gethostbyname): Unknown\n");

            wifi_connected = 99;
        }
    } else if(WIFI_IsConnected() == 1 && wifi_connected == 3) {
        DTXT("ConnectorRun(): got ip-address\n");

        if(mqtt->shutdown == 0) {
            MQTT_Connect(&mqtt->client, &mqtt->client.m_IpAddr, mqtt->port);

            wifi_connected = 100;
        }
    } else if(WIFI_IsConnected() == 0 && wifi_connected != 0) {
        DTXT("ConnectorRun(): WiFi has been disconnected; uptime = %lu\n", (unsigned long) esp_uptime(0));
        
        wifi_connected = 0;
        
        MQTT_Disconnect(&mqtt->client);
    }

    MQTT_Run(&mqtt->client);
    
    // get ingress MQTT Fabric message if any
    MqttFabric_Message* msg = GetEvent(mqtt);
    
    switch(GetEventType(msg)) {
        case none:
            break;
            
        case offramp:
            /*if(client->m_OnOfframpCallback) {
                client->m_OnOfframpCallback(client, 
                                            client->m_Ptr,
                                            MqttFabric_GetOfframp_Nodename(msg),
                                            MqttFabric_GetOfframp_ActorId(msg),
                                            MqttFabric_GetOfframp_ActorPlatformId(msg),
                                            MqttFabric_GetOfframp_TaskId(msg),
                                            MqttFabric_GetOfframp_PlatformId(msg),
                                            MqttFabric_GetOfframp_ServiceId(msg),
                                            MqttFabric_GetOfframp_FeedId(msg),
                                            MqttFabric_GetOfframp_Payload(msg));
            }*/
            break;
            
        case command:
            if(mqtt->onCommandCallback) {
                mqtt->onCommandCallback(mqtt, 
                                        mqtt->userdata,
                                        MqttFabric_GetCommand_Nodename(msg),
                                        MqttFabric_GetCommand_ActorId(msg),
                                        MqttFabric_GetCommand_PlatformId(msg),
                                        MqttFabric_GetCommand_FeedId(msg),
                                        MqttFabric_GetCommand_Payload(msg));
            }
            break;
            
        case onramp:
            break;
            
        case connect:
            ret = RUN_CONNECTED;
            
            if(mqtt->chronosNodename != 0) {
                chronosSubscribe(mqtt);
            }
            
            if(mqtt->svcDevice != 0) {
                deviceSubscribe(mqtt->svcDevice);
                devicePublish(mqtt->svcDevice);
            } 
            break;
            
        case disconnect:
            ret = RUN_DISCONNECTED;
            break;
            
        case svcCtrlOnline:
            if(mqtt->svcDevice != 0) {
                devicePublish(mqtt->svcDevice);
            } 
            break;
            
        case svcFromHK:
            DTXT("ConnectorRun(): event svcFromHK; actorId = '%s', feedId = '%s'\n", MqttFabric_GetFromHK_ActorId(msg), MqttFabric_GetFromHK_FeedId(msg));
            onValueUpdate(mqtt->svcDevice, MqttFabric_GetFromHK_ActorId(msg), MqttFabric_GetFromHK_FeedId(msg), MqttFabric_GetFromHK_Payload(msg));
            break;
    }
    
    // delete event data
    DeleteEvent(msg);    
    
    return ret;
}
/**
 * 
 * @param mqtt
 * @param ptr
 * @param onCommandCallback
 * @return 
 */
int ICACHE_FLASH_ATTR InstallCallbacks( Mqtt*               mqtt,
                                        void*               ptr,
                                        OnCommandCallback   onCommandCallback)
{
    if(mqtt == 0) {
        return -1;
    }
    
    mqtt->userdata          = ptr;
    mqtt->onCommandCallback = onCommandCallback;
    
    return 0;
}
/**
 * 
 * @param mqtt
 * @param chronosNodename
 * @return 
 */
int ICACHE_FLASH_ATTR EnableChronos(Mqtt* mqtt, const char* chronosNodename)
{
    if(mqtt == 0) {
        return -1;
    }

    mqtt->chronosNodename = chronosNodename;
    
    return 0;
}
/**
 * 
 * @param mqtt
 * @return 
 */
const char* ICACHE_FLASH_ATTR GetNodename(Mqtt* mqtt)
{
    return mqtt->actorId;
}
/**
 * 
 * @param mqtt
 * @return 
 */
const char* ICACHE_FLASH_ATTR GetPlatformId(Mqtt* mqtt)
{
    if(mqtt == 0) {
        return NULL;
    }
    
    return mqtt->actorPlatformId;
}
/**
 * 
 * @param mqtt
 * @return 
 */
int ICACHE_FLASH_ATTR IsConnected(Mqtt* mqtt)
{
    if(mqtt == 0) {
        return 0;
    }

    return MQTT_IsConnected(&mqtt->client);
}
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
int ICACHE_FLASH_ATTR CommandSubscription(Mqtt* mqtt, const char* nodename, const char* actorId, const char* platformId, const char* feedId, int qos)
{
    if(mqtt == 0) {
        return -1;
    }

    if(mqtt->shutdown == 0) {
        char* topic = (char*) os_malloc(topicCommandSubscribe(mqtt, nodename, actorId, platformId, feedId, 0));
        if(topic != 0) {
            topicCommandSubscribe(mqtt, nodename, actorId, platformId, feedId, topic);

            DTXT("CommandSubscription(): topic = %s\n", topic);
            
            MQTT_Subscribe(&mqtt->client, topic, qos);
        
            os_free(topic);
        } else {
            DTXT("CommandSubscription(topic): mem fail\n");
        }
    }
    
    return 0;
}
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
int ICACHE_FLASH_ATTR CommandPublish(Mqtt* mqtt, const char* actorId, const char* platformId, const char* feedId, const char* data, int qos, int retain)
{
    if(mqtt == 0) {
        return -1;
    }

    if(mqtt->shutdown == 0) {
        char* topic = (char*) os_malloc(topicCommandPublish(mqtt, actorId, platformId, feedId, 0));
        if(topic != 0) {
            topicCommandPublish(mqtt, actorId, platformId, feedId, topic);

            MQTT_Publish(&mqtt->client, topic, data, os_strlen(data), qos, retain);
        
            os_free(topic);
        } else {
            DTXT("CommandPublish(topic): mem fail\n");
        }
    }
    
    return 0;
}
/**
 * 
 * @param mqtt
 * @param feedId
 * @param data
 * @return 
 */
int ICACHE_FLASH_ATTR DebugPublish(Mqtt* mqtt, const char* feedId, const char* data)
{
    if(mqtt == 0) {
        return -1;
    }

    if(mqtt->shutdown == 0) {
        char* topic = (char*) os_malloc(topicOfframpPublish(mqtt, 
                                                            fabricNodenameBroadcast,		// destination nodename 
                                                            fabricTaskIdDebug, 			// taskId 
                                                            GetPlatformId(mqtt),		// platformId
                                                            fabricServiceIdDebug,               // serviceId 
                                                            feedId,
                                                            0));

        if(topic == 0) {
            DTXT("DebugPublish(topic): mem fail\n");
            return -1;
        }

        topicOfframpPublish(mqtt, 
                            fabricNodenameBroadcast,		// destination nodename 
                            fabricTaskIdDebug, 			// taskId 
                            GetPlatformId(mqtt),		// platformId
                            fabricServiceIdDebug,               // serviceId 
                            feedId,
                            topic);

        DTXT("DebugPublish(): topic = '%s'\n", topic);
        
        MQTT_Publish(&mqtt->client, topic, data, os_strlen(data), 0, 0);
        
        os_free(topic);
    }
    
    return 0;
}
/**
 * 
 * @param client
 * @return 
 */
MqttFabric_Message* ICACHE_FLASH_ATTR GetEvent(Mqtt* mqtt)
{
    if(mqtt == 0) {
        return NULL;
    }

    return STAILQ_FIRST(&mqttHead);
}
/**
 * 
 * @param msg
 * @return 
 */
MqttFabric_MessageType ICACHE_FLASH_ATTR GetEventType(MqttFabric_Message* msg)
{
    if(msg == NULL) {
        return none;
    }
    
    return msg->m_MessageType;
}
/**
 * 
 * @param msg
 */
void ICACHE_FLASH_ATTR DeleteEvent(MqttFabric_Message* msg)
{
    if(msg == NULL) {
        return;
    }
    
    switch(msg->m_MessageType) {
        case none:
            break;
            
        case onramp:
            break;
            
        case offramp:
            if(msg->m_Message.m_MessageOfframp.feed_id)             os_free(msg->m_Message.m_MessageOfframp.feed_id);
            if(msg->m_Message.m_MessageOfframp.service_id)          os_free(msg->m_Message.m_MessageOfframp.service_id);
            if(msg->m_Message.m_MessageOfframp.platform_id)         os_free(msg->m_Message.m_MessageOfframp.platform_id);
            if(msg->m_Message.m_MessageOfframp.task_id)             os_free(msg->m_Message.m_MessageOfframp.task_id);
            if(msg->m_Message.m_MessageOfframp.actor_platform_id)   os_free(msg->m_Message.m_MessageOfframp.actor_platform_id);
            if(msg->m_Message.m_MessageOfframp.actor_id)            os_free(msg->m_Message.m_MessageOfframp.actor_id);
            if(msg->m_Message.m_MessageOfframp.nodename)            os_free(msg->m_Message.m_MessageOfframp.nodename);
            if(msg->m_Payload)                                      os_free(msg->m_Payload);
            
            STAILQ_REMOVE(&mqttHead, msg, MqttFabric_Message, entries);
            
            os_free(msg);
            break;
            
        case command:
            if(msg->m_Message.m_MessageCommand.feed_id)             os_free(msg->m_Message.m_MessageCommand.feed_id);
            if(msg->m_Message.m_MessageCommand.platform_id)         os_free(msg->m_Message.m_MessageCommand.platform_id);
            if(msg->m_Message.m_MessageCommand.actor_id)            os_free(msg->m_Message.m_MessageCommand.actor_id);
            if(msg->m_Message.m_MessageCommand.nodename)            os_free(msg->m_Message.m_MessageCommand.nodename);
            if(msg->m_Payload)                                      os_free(msg->m_Payload);
            
            STAILQ_REMOVE(&mqttHead, msg, MqttFabric_Message, entries);
            
            os_free(msg);
            break;
            
        case connect:
        case disconnect:
        case svcCtrlOnline:
            STAILQ_REMOVE(&mqttHead, msg, MqttFabric_Message, entries);            
            os_free(msg);
            break;
            
        case svcFromHK:
            if(msg->m_Message.m_MessageFromHK.feed_id)              os_free(msg->m_Message.m_MessageFromHK.feed_id);
            if(msg->m_Message.m_MessageFromHK.actor_id)             os_free(msg->m_Message.m_MessageFromHK.actor_id);
            if(msg->m_Payload)                                      os_free(msg->m_Payload);
            
            STAILQ_REMOVE(&mqttHead, msg, MqttFabric_Message, entries);
            
            os_free(msg);
            break;
    }
}

/******************************************************************************************************************
 * private functions
 *
 */

/**
 * 
 * @param sessionPresent
 * @param connack_rc
 * @param args
 */
void ICACHE_FLASH_ATTR onConnect(unsigned char sessionPresent, unsigned char connack_rc, void* args)
{
    DTXT("onConnect()\n");

    Mqtt* mqtt = (Mqtt*)(args);

    if(mqtt == 0) {
        return;
    }

    // send status message
    char* topic1; char* msg;
    if(statusMessage(mqtt, fabricStatusOnline, (int) esp_uptime(0), &topic1, &msg) == 0) {
        MQTT_Publish(&mqtt->client, topic1, msg, os_strlen(msg), fabricStatusQos, fabricStatusRetain);
    }

    // create status message topic
    char* topic2 = (char*) os_malloc(topicStatusSubscribe(mqtt, 0));
    topicStatusSubscribe(mqtt, topic2);

    MQTT_Subscribe(&mqtt->client, topic2, fabricStatusQos);
    
    if(topic2 != 0) {
        os_free(topic2);
    }
    if(topic1 != 0) {
        os_free(topic1);
    }
    if(msg != 0) {
        os_free(msg);
    }

    // append to queue
    MqttFabric_Message* notif = os_malloc(sizeof(MqttFabric_Message));
        
    notif->m_MessageType = connect;
    STAILQ_INSERT_TAIL(&mqttHead, notif, entries);                                // insert at end
}
/**
 * 
 * @param args
 */
void ICACHE_FLASH_ATTR onDisconnect(void* args)
{
    DTXT("onDisconnect\n");
    
    if(args == 0) {
        return;
    }

    // append to queue
    MqttFabric_Message* notif = os_malloc(sizeof(MqttFabric_Message));
        
    notif->m_MessageType = disconnect;
    STAILQ_INSERT_TAIL(&mqttHead, notif, entries);                                // insert at end
}
/**
 * 
 * @param topic
 * @param payload
 * @param payloadlen
 * @param qos
 * @param retained
 * @param dup
 * @param ptr
 */
void ICACHE_FLASH_ATTR onMessage(const char* topic, const unsigned char* payload, int payloadlen, int qos, unsigned char retained, unsigned char dup, void* ptr)
{
    DTXT("onMessage\n");

    Mqtt* mqtt = (Mqtt*)(ptr);
    
    if(mqtt == 0) {
        return;
    }

    char* nodename          = NULL;
    char* actorId           = NULL;
    char* actorPlatformId   = NULL;
    char* taskId            = NULL;
    char* platformId        = NULL;
    char* serviceId         = NULL;
    char* feedId            = NULL;
    
    int   idx;

    FABRIC_TOKEN           token;
    MqttFabric_MessageType ramp = none;

    if(tokenBegin(&token, (char*)topic, os_strlen(topic)) == 0) {
        char* subtopic;

        while(tokenNext(&token, &subtopic, &idx) == 0) {
            switch(idx) {
                case 0:
                    if(os_strcmp(mqtt->rootTopic, subtopic) != 0) {
                        DTXT("onMessage(): invalid root topic; '%s'\n", subtopic);
                        tokenReset(&token);
                    }
                    break;
                    
                case 1:
                    nodename = subtopic;
                    break;
                    
                case 2:
                    if(os_strcmp("$feeds", subtopic) == 0 || os_strcmp("$commands", subtopic) == 0) {

                    } else {
                        DTXT("onMessage(): '$feeds' or '$commands' missing; '%s'\n", subtopic);
                        tokenReset(&token);
                    }
                    break;
                    
                case 3:
                    if(os_strcmp("$offramp", subtopic) == 0) {
                        DTXT("onMessage(): '$offramp'\n");
                        ramp = offramp;
                    } else if(os_strcmp("$onramp", subtopic) == 0) {
                        DTXT("onMessage(): '$onramp'\n");
                        ramp = onramp;
                    } else if(os_strcmp("$clients", subtopic) == 0) {
                        DTXT("onMessage(): '$clients'\n");
                        ramp = command;
                    } else {
                        DTXT("onMessage(): subtopic 3 = '%s'\n", subtopic);
                    }
                    break;
                    
                case 4:
                    if(ramp == offramp) {
                        actorId = subtopic;
                    } else if(ramp == command) {
                        actorId = subtopic;
                    }
                    break;
                    
                case 5:
                    if(ramp == offramp) {
                        actorPlatformId = subtopic;
                    } else if(ramp == command) {
                        platformId = subtopic;
                    }
                    break;
                    
                case 6:
                    if(ramp == offramp) {
                        taskId = subtopic;
                    } else if(ramp == command) {
                        feedId = subtopic;
                    }
                    break;
                    
                case 7:
                    if(ramp == offramp) {
                        platformId = subtopic;
                    }
                    break;
                    
                case 8:
                    if(ramp == offramp) {
                        serviceId = subtopic;
                    }
                    break;
                    
                case 9:
                    if(ramp == offramp) {
                        feedId = subtopic;
                    }
                    break;
            }
        }
    }

    if(ramp == command) {
        DTXT("onMessage(): $commands\n");
        DTXT("onMessage(): nodename          = %s\n", nodename);
        DTXT("onMessage(): platformId        = %s\n", platformId);
        DTXT("onMessage(): feedId            = %s\n", feedId);
        
        if(os_strcmp(nodename, mqtt->actorId) != 0) {
            onCommandHandler(   mqtt, 
                                nodename, 
                                actorId, 
                                platformId, 
                                feedId, 
                                payload, 
                                payloadlen);
        }
    } else if(ramp == onramp) {
        DTXT("onMessage(): $onramp\n");
        DTXT("onMessage(): nodename          = %s\n", nodename);
        DTXT("onMessage(): platformId        = %s\n", platformId);
        DTXT("onMessage(): serviceId         = %s\n", serviceId);
        DTXT("onMessage(): feedId            = %s\n", feedId);
        
    } else if(ramp == offramp) {
        DTXT("onMessage(): $offramp\n");
        DTXT("onMessage(): nodename          = %s\n", nodename);
        DTXT("onMessage(): actorId           = %s\n", actorId);
        DTXT("onMessage(): actorPlatformId   = %s\n", actorPlatformId);
        DTXT("onMessage(): taskId            = %s\n", taskId);
        DTXT("onMessage(): platformId        = %s\n", platformId);
        DTXT("onMessage(): serviceId         = %s\n", serviceId);
        DTXT("onMessage(): feedId            = %s\n", feedId);
        
        if(os_strcmp(actorId, mqtt->actorId) != 0) {
            onOfframpHandler(   mqtt, 
                                nodename,
                                actorId,
                                actorPlatformId,
                                taskId,
                                platformId,
                                serviceId,
                                feedId,
                                payload, 
                                payloadlen);
        }
    } else {
        DTXT("onMessage(): <unhandled>\n");
        DTXT("onMessage(): nodename          = %s\n", nodename);
        //DTXT("onMessage(): actor_id          = %s\n", actorId);
        //DTXT("onMessage(): actor_platform_id = %s\n", actorPlatformId);
        //DTXT("onMessage(): task_id           = %s\n", taskId);
        //DTXT("onMessage(): platform_id       = %s\n", platformId);
        //DTXT("onMessage(): service_id        = %s\n", serviceId);
        //DTXT("onMessage(): feed_id           = %s\n", feedId);
    }
}
/**
 * 
 * @param mqtt
 * @param nodename
 * @param actorId
 * @param platformId
 * @param feedId
 * @param payload
 * @param payloadlen
 */
void ICACHE_FLASH_ATTR onCommandHandler(Mqtt*                   mqtt, 
                                        const char*             nodename, 
                                        const char*             actorId, 
                                        const char*             platformId, 
                                        const char*             feedId, 
                                        const unsigned char*    payload, 
                                        int                     payloadlen)
{
    if(os_strcmp(actorId, fabricSys) == 0 && os_strcmp(feedId, fabricCmdStatus) == 0) {
        if(BMix_DecoderBegin((const char*)payload) != NULL) {
            const char* _type;
            const char* status;
            const char* nodename;
            const char* platformId;
            const char* classType;

            if(BMix_GetString("_type", &_type) != 0) {
                DTXT("onCommandHandler(): '_type' not found\n");
            } else if(BMix_GetString("status", &status) != 0) {
                DTXT("onCommandHandler(): 'status' not found\n");
            } else if(BMix_GetString("nodename", &nodename) != 0) {
                DTXT("onCommandHandler(): 'nodename' not found\n");
            } else if(BMix_GetString("platform_id", &platformId) != 0) {
                DTXT("onCommandHandler(): 'platform_id' not found\n");
            } else if(BMix_GetString("class", &classType) != 0) {
                DTXT("onCommandHandler(): 'class' not found\n");
            } else {
                //DTXT("onCommandHandler(): all fields found\n");

                if(os_strcmp(status, "online") == 0 && os_strcmp(classType, classTypeControllerSvcTxt) == 0) {
                    DTXT("onCommandHandler(): service controller online\n");

                    MqttFabric_Message* notif = os_malloc(sizeof(MqttFabric_Message));

                    notif->m_MessageType = svcCtrlOnline;
                    STAILQ_INSERT_TAIL(&mqttHead, notif, entries);                                // insert at end
                }
            }
        }

        BMix_DecoderEnd();
    } else {
        // append to queue
        MqttFabric_Message* msg = os_malloc(sizeof(MqttFabric_Message));
        
        msg->m_MessageType = command;
        
        msg->m_Message.m_MessageCommand.nodename = os_malloc(os_strlen(nodename) + 1);
        strcpy(msg->m_Message.m_MessageCommand.nodename, nodename);
        
        msg->m_Message.m_MessageCommand.actor_id = os_malloc(os_strlen(actorId) + 1);
        strcpy(msg->m_Message.m_MessageCommand.actor_id, actorId);
        
        msg->m_Message.m_MessageCommand.platform_id = os_malloc(os_strlen(platformId) + 1);
        strcpy(msg->m_Message.m_MessageCommand.platform_id, platformId);
        
        msg->m_Message.m_MessageCommand.feed_id = os_malloc(os_strlen(feedId) + 1);
        strcpy(msg->m_Message.m_MessageCommand.feed_id, feedId);
        
        // the payload
        msg->m_Payload    = os_malloc(payloadlen + 1);
        msg->m_PayloadLen = payloadlen + 1;
        os_memcpy(msg->m_Payload, payload, payloadlen);
        msg->m_Payload[payloadlen] = '\0';
        
        STAILQ_INSERT_TAIL(&mqttHead, msg, entries);                                // insert at end
    }
}
/**
 * 
 * @param mqtt
 * @param nodename
 * @param actorId
 * @param actorPlatformId
 * @param taskId
 * @param platformId
 * @param serviceId
 * @param feedId
 * @param msg
 */
void ICACHE_FLASH_ATTR onOfframpHandler(Mqtt*       mqtt, 
                                        const char* nodename,
                                        const char* actorId,
                                        const char* actorPlatformId,
                                        const char* taskId,
                                        const char* platformId,
                                        const char* serviceId,
                                        const char* feedId,
                                        const unsigned char*    payload, 
                                        int                     payloadlen)
{
    if(os_strcmp(nodename, mqtt->actorId) == 0 && os_strcmp(taskId, fabricTaskIdService) == 0 && os_strcmp(serviceId, fabricServiceIdFromHK) == 0) {
        DTXT("onOfframpHandler(fabricServiceIdFromHK): \n");
        
        if(mqtt->svcDevice != 0) {
            DTXT("onOfframpHandler(fabricServiceIdFromHK): queue begin\n");
            // append to queue
            MqttFabric_Message* msg = os_malloc(sizeof(MqttFabric_Message));
            if(msg == 0) {
                DTXT("onOfframpHandler(fabricServiceIdFromHK/msg): mem fail\n");
                goto defer;
            }
            
            msg->m_MessageType = svcFromHK;

            msg->m_Message.m_MessageFromHK.actor_id = os_malloc(os_strlen(actorId) + 1);   // senders nodename
            if(msg->m_Message.m_MessageFromHK.actor_id == 0) {
                goto defer;
            }
            
            strcpy(msg->m_Message.m_MessageFromHK.actor_id, actorId);

            msg->m_Message.m_MessageFromHK.feed_id = os_malloc(os_strlen(feedId) + 1);     // format
            if(msg->m_Message.m_MessageFromHK.feed_id == 0) {
                goto defer;
            }
            
            strcpy(msg->m_Message.m_MessageFromHK.feed_id, feedId);

            // the payload
            msg->m_Payload = os_malloc(payloadlen + 1);
            if(msg->m_Payload == 0) {
                goto defer;
            }
            
            msg->m_PayloadLen = payloadlen + 1;
            os_memcpy(msg->m_Payload, payload, payloadlen);
            msg->m_Payload[payloadlen] = '\0';

            STAILQ_INSERT_TAIL(&mqttHead, msg, entries);                                // insert at end

            DTXT("onOfframpHandler(fabricServiceIdFromHK): queue end\n");
            
            return;
defer:
            if(msg->m_Payload != 0) {
                os_free(msg->m_Payload);
            }
            if(msg->m_Message.m_MessageOfframp.actor_id != 0) {
                os_free(msg->m_Message.m_MessageOfframp.actor_id);
            }
            if(msg != 0) {
                os_free(msg);
            }

            return;
        }
    } else if(os_strcmp(platformId, PlatformIdChronos) == 0 && os_strcmp(serviceId, ServiceIdAnalogOut) == 0 && os_strcmp(feedId, FeedIdSeconds) == 0) {
        if(BMix_DecoderBegin((const char*)payload) != NULL) {
           const char* _type;
           uint64_t    val;

           if(BMix_GetString("_type", &_type) == 0 && os_strcmp(_type, TaskIdAnalogWrite) == 0 && BMix_GetU64("value", &val) == 0) {
               DTXT("onOfframpHandler(chronos): time set\n");

               esp_stime((esp_time_t*) &val);
           }
       }

        BMix_DecoderEnd();
    }
}
/**
 * 
 * @param mqtt
 * @param fabricStatus
 * @param seconds
 * @param topic
 * @param msg
 * @return 
 */
int ICACHE_FLASH_ATTR statusMessage(Mqtt* mqtt, fabricStatus fabricStatus, int seconds, char** topic, char** msg) {
    // create the topic
    *topic = (char*) os_malloc(topicStatusPublish(mqtt, 0));
    topicStatusPublish(mqtt, *topic);

    char class_type[16];
    
    switch(mqtt->classType) {
        case ClassTypeDevice:
            strcpy(class_type, classTypeDeviceTxt);
            break;
            
        case ClassTypeController:
            strcpy(class_type, classTypeControllerTxt);
            break;

        case ClassTypeDeviceSvc:
            strcpy(class_type, classTypeDeviceSvcTxt);
            break;
            
        case ClassTypeControllerSvc:
            strcpy(class_type, classTypeControllerSvcTxt);
            break;
            
        default:
            goto defer;
    }
    
    *msg = (char*) os_malloc(256);
    if(*msg == 0) {
        goto defer;
    }
    
    // create the message
    switch(fabricStatus) {
        case fabricStatusOnline:
            os_sprintf(*msg, "{\"d\":{\"_type\":\"status\",\"status\":\"online\",\"uptime\":%d,\"nodename\":\"%s\",\"platform_id\":\"%s\",\"class\":\"%s\"}}", 
                    seconds, 
                    mqtt->actorId, 
                    mqtt->actorPlatformId,
                    class_type);
            break;

        case fabricStatusOffline:
            os_sprintf(*msg, "{\"d\":{\"_type\":\"status\",\"status\":\"offline\",\"uptime\":%d,\"nodename\":\"%s\",\"platform_id\":\"%s\",\"class\":\"%s\"}}", 
                    seconds, 
                    mqtt->actorId, 
                    mqtt->actorPlatformId,
                    class_type);
            break;

        case fabricStatusDisconnected:
            os_sprintf(*msg, "{\"d\":{\"_type\":\"status\",\"status\":\"disconnected\",\"uptime\":null,\"nodename\":\"%s\",\"platform_id\":\"%s\",\"class\":\"%s\"}}", 
                    mqtt->actorId, 
                    mqtt->actorPlatformId,
                    class_type);
            break;
            
        default:
            goto defer;
    }
    
    return 0;
    
defer:
    if(*topic != 0) {
        os_free(*topic);
    }
    if(*msg != 0) {
        os_free(*msg);
    }

    return -1;
}
/**
 * 
 * @param d
 */
void ICACHE_FLASH_ATTR chronosSubscribe(Mqtt* mqtt)
{
    char* topic = (char*) os_malloc(topicOfframpSubscribe(  mqtt, 
                                                            fabricNodenameBroadcast, 
                                                            mqtt->chronosNodename,      // actorID == senders nodename
                                                            fabricTopicAny,             // actorPlatformID == senders platformId
                                                            fabricTopicAny,             // taskId 
                                                            PlatformIdChronos,          // platformId
                                                            ServiceIdAnalogOut,         // serviceId
                                                            FeedIdSeconds,              // feedId
                                                            0));
    
    if(topic == 0) {
        DTXT("chronosSubscribe(topic): mem fail\n");
        return;
    }
    
    topicOfframpSubscribe(  mqtt, 
                            fabricNodenameBroadcast, 
                            mqtt->chronosNodename,      // actorID == senders nodename
                            fabricTopicAny,             // actorPlatformID == senders platformId
                            fabricTopicAny,             // taskId 
                            PlatformIdChronos,          // platformId 
                            ServiceIdAnalogOut,         // serviceId
                            FeedIdSeconds,              // feedId
                            topic);
    
    MQTT_Subscribe(&mqtt->client, topic, 0);

    DTXT("chronosSubscribe(): topic = '%s'\n", topic);
    
    os_free(topic);
}
/**
 * 
 * @param name
 * @param ipaddr
 * @param callback_arg
 */
static void ICACHE_FLASH_ATTR _dns_found_callback(const char* name, ip_addr_t* ipaddr, void* callback_arg)
{
    DTXT("_dns_found_callback(): name = %s\n", name);
}

/******************************************************************************************************************
 * private helpers
 *
 */

/**
 * 
 * @param dest
 * @param source
 * @return 
 */
char* ICACHE_FLASH_ATTR strcpy_alloc(char** dest, const char* source)
{
    *dest = (char*) os_malloc(os_strlen(source) + 1);
    if(*dest == 0) {
       return 0;
    }
    
    os_strcpy(*dest, source);
    
    return *dest;
}
