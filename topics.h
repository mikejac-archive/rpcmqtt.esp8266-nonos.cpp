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

#ifndef TOPICS_H
#define	TOPICS_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct Mqtt Mqtt;

/******************************************************************************************************************
 * 
 *
 */

#define FabricNodenameAny           "+"

#define fabricSys                   "sysctl"
#define fabricCmdStatus             "status"

#define fabricTopicAny              "+"
#define fabricNodenameBroadcast     "broadcast"

#define fabricServiceIdRpcServer    "rpc_server"
#define fabricServiceIdRpcClient    "rpc_client"
#define fabricServiceIdFromHK       "from_hk"
#define fabricServiceIdToHK         "to_hk"
#define fabricServiceIdAccessories  "accessories"
#define fabricServiceIdDebug        "debug"

#define fabricTaskIdService         "svc"
#define fabricTaskIdDebug           "dbg"

#define SUBTOPIC_SIZE               32

typedef struct {
    char*               m_Topic;
    int                 m_TopicLen;
    char*               m_P;
    char*               m_Pp;
    int                 m_Words;
    int                 m_I;
    char                m_LastSubtopic[SUBTOPIC_SIZE];
} FABRIC_TOKEN;

/******************************************************************************************************************
 * prototypes
 *
 */

/**
 * 
 * @param mqtt
 * @param topic
 * @return 
 */
int topicStatusPublish( Mqtt* mqtt,
                        char* topic);
/**
 * 
 * @param mqtt
 * @param topic
 * @return 
 */
int topicStatusSubscribe(   Mqtt* mqtt,
                            char* topic);
/**
 * 
 * @param mqtt
 * @param actorId
 * @param platformId
 * @param feedId
 * @param topic
 * @return 
 */
int topicCommandPublish(Mqtt* mqtt,
                        const char* actorId, 
                        const char* platformId, 
                        const char* feedId,
                        char* topic);
/**
 * 
 * @param mqtt
 * @param nodename
 * @param actorId
 * @param platformId
 * @param feedId
 * @param topic
 * @return 
 */
int topicCommandSubscribe(  Mqtt* mqtt,
                            const char* nodename, 
                            const char* actorId, 
                            const char* platformId, 
                            const char* feedId,
                            char* topic);
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
 * @param topic
 * @return 
 */
int topicOfframpSubscribe(  Mqtt *      mqtt, 
                            const char* nodename, 
                            const char *actorId,
                            const char* actorPlatformId,
                            const char* taskId,
                            const char* platformId,
                            const char* serviceId,
                            const char* feedId,
                            char*       topic);
/**
 * 
 * @param mqtt
 * @param nodename
 * @param taskId
 * @param platformId
 * @param serviceId
 * @param feedId
 * @param topic
 * @return 
 */
int topicOfframpPublish(Mqtt*       mqtt, 
                        const char* nodename,
                        const char* taskId,
                        const char* platformId,
                        const char* serviceId,
                        const char* feedId,
                        char*       topic);
/**
 * 
 * @param mqtt
 * @param nodename
 * @param serviceId
 * @param feedId
 * @param topic
 * @return 
 */
int topicOnrampPublish( Mqtt*       mqtt,
                        const char* nodename,
                        const char* serviceId,
                        const char* feedId,
                        char*       topic);
/**
 * 
 * @param t
 * @param topic
 * @param len
 * @return 
 */
int tokenBegin(FABRIC_TOKEN* t, char* topic, int len);
/**
 * 
 * @param t
 * @param subtopic
 * @param index
 * @return 
 */
int tokenNext(FABRIC_TOKEN* t, char** subtopic, int* index);
/**
 * 
 * @param t
 * @return 
 */
int tokenReset(FABRIC_TOKEN* t);

#ifdef	__cplusplus
}
#endif

#endif	/* TOPICS_H */

