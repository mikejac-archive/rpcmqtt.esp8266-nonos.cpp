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

#include "topics.h"
#include "mqtt_connector.h"
#include <github.com/mikejac/misc.esp8266-nonos.cpp/espmissingincludes.h>
#include <osapi.h>
#include <mem.h>

#define DTXT(...)   os_printf(__VA_ARGS__)

/******************************************************************************************************************
 * public functions
 *
 */

/**
 * 
 * @param mqtt
 * @param topic
 * @return 
 */
int ICACHE_FLASH_ATTR topicStatusPublish(   Mqtt* mqtt,
                                            char* topic)
{
    int ret = 0;

    if(topic == 0) {
        return  os_strlen(mqtt->rootTopic) + 1 +
                os_strlen(mqtt->actorId) +
                os_strlen("/$commands/$clients/") +
                os_strlen(fabricSys) + 1 +
                os_strlen(mqtt->actorPlatformId) + 1 +
                os_strlen(fabricCmdStatus) + 1;
    }
    
    strcpy(topic, mqtt->rootTopic);
    strcat(topic, "/");
    strcat(topic, mqtt->actorId);
    strcat(topic, "/$commands/$clients/");
    strcat(topic, fabricSys);
    strcat(topic, "/");
    strcat(topic, mqtt->actorPlatformId);
    strcat(topic, "/");
    strcat(topic, fabricCmdStatus);

    return ret;
}
/**
 * 
 * @param mqtt
 * @param topic
 * @return 
 */
int ICACHE_FLASH_ATTR topicStatusSubscribe( Mqtt* mqtt,
                                            char* topic)
{
    int ret = 0;

    if(topic == 0) {
        return  os_strlen(mqtt->rootTopic) + 1 +
                os_strlen(fabricTopicAny) +
                os_strlen("/$commands/$clients/") +
                os_strlen(fabricSys) + 1 +
                os_strlen(fabricTopicAny) + 1 +
                os_strlen(fabricCmdStatus) + 1;
    }

    strcpy(topic, mqtt->rootTopic);
    strcat(topic, "/");
    strcat(topic, fabricTopicAny);
    strcat(topic, "/$commands/$clients/");
    strcat(topic, fabricSys);
    strcat(topic, "/");
    strcat(topic, fabricTopicAny);
    strcat(topic, "/");
    strcat(topic, fabricCmdStatus);

    return ret;
}
/**
 * 
 * @param mqtt
 * @param actorId
 * @param platformId
 * @param feedId
 * @param topic
 * @return 
 */
int ICACHE_FLASH_ATTR topicCommandPublish(  Mqtt* mqtt,
                                            const char* actorId, 
                                            const char* platformId, 
                                            const char* feedId,
                                            char* topic)
{
    int ret = 0;

    if(topic == 0) {
        return  os_strlen(mqtt->rootTopic) + 1 +
                os_strlen(mqtt->actorId) +
                os_strlen("/$commands/$clients/") +
                os_strlen(actorId) + 1 +
                os_strlen(platformId) + 1 +
                os_strlen(feedId) + 1;
    }

    strcpy(topic, mqtt->rootTopic);
    strcat(topic, "/");
    strcat(topic, mqtt->actorId);
    strcat(topic, "/$commands/$clients/");
    strcat(topic, actorId);
    strcat(topic, "/");
    strcat(topic, platformId);
    strcat(topic, "/");
    strcat(topic, feedId);

    return ret;
}
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
int ICACHE_FLASH_ATTR topicCommandSubscribe(Mqtt* mqtt,
                                            const char* nodename, 
                                            const char* actorId, 
                                            const char* platformId, 
                                            const char* feedId,
                                            char* topic)
{
    int ret = 0;

    if(topic == 0) {
        return  os_strlen(mqtt->rootTopic) + 1 +
                os_strlen(nodename) +
                os_strlen("/$commands/$clients/") +
                os_strlen(actorId) + 1 +
                os_strlen(platformId) + 1 +
                os_strlen(feedId) + 1;
    }

    strcpy(topic, mqtt->rootTopic);
    strcat(topic, "/");
    strcat(topic, nodename);
    strcat(topic, "/$commands/$clients/");
    strcat(topic, actorId);
    strcat(topic, "/");
    strcat(topic, platformId);
    strcat(topic, "/");
    strcat(topic, feedId);

    return ret;
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
 * @param topic
 * @return 
 */
int ICACHE_FLASH_ATTR topicOfframpSubscribe(Mqtt*       mqtt, 
                                            const char* nodename, 
                                            const char* actorId,
                                            const char* actorPlatformId,
                                            const char* taskId,
                                            const char* platformId,
                                            const char* serviceId,
                                            const char* feedId,
                                            char*       topic)
{
    int ret = 0;

    if(topic == 0) {
        return  os_strlen(mqtt->rootTopic) + 1 +
                os_strlen(nodename) +
                os_strlen("/$feeds/$offramp/") +
                os_strlen(actorId) + 1 +
                os_strlen(actorPlatformId) + 1 +
                os_strlen(taskId) + 1 +
                os_strlen(platformId) + 1 +
                os_strlen(serviceId) + 1 +
                os_strlen(feedId) + 1;
    }

    strcpy(topic, mqtt->rootTopic);
    strcat(topic, "/");
    strcat(topic, nodename);
    strcat(topic, "/$feeds/$offramp/");
    strcat(topic, actorId);
    strcat(topic, "/");
    strcat(topic, actorPlatformId);
    strcat(topic, "/");
    strcat(topic, taskId);
    strcat(topic, "/");
    strcat(topic, platformId);
    strcat(topic, "/");
    strcat(topic, serviceId);
    strcat(topic, "/");
    strcat(topic, feedId);

    return ret;
}
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
int ICACHE_FLASH_ATTR topicOfframpPublish(  Mqtt*       mqtt, 
                                            const char* nodename,
                                            const char* taskId,
                                            const char* platformId,
                                            const char* serviceId,
                                            const char* feedId,
                                            char*       topic)
{
    int ret = 0;

    if(topic == 0) {
        return  os_strlen(mqtt->rootTopic) + 1 +
                os_strlen(nodename) +
                os_strlen("/$feeds/$offramp/") +
                os_strlen(mqtt->actorId) + 1 +
                os_strlen(mqtt->actorPlatformId) + 1 +
                os_strlen(taskId) + 1 +
                os_strlen(platformId) + 1 +
                os_strlen(serviceId) + 1 +
                os_strlen(feedId) + 1;
    }

    strcpy(topic, mqtt->rootTopic);
    strcat(topic, "/");
    strcat(topic, nodename);
    strcat(topic, "/$feeds/$offramp/");
    strcat(topic, mqtt->actorId);
    strcat(topic, "/");
    strcat(topic, mqtt->actorPlatformId);
    strcat(topic, "/");
    strcat(topic, taskId);
    strcat(topic, "/");
    strcat(topic, platformId);
    strcat(topic, "/");
    strcat(topic, serviceId);
    strcat(topic, "/");
    strcat(topic, feedId);

    return ret;
}
/**
 * 
 * @param mqtt
 * @param nodename
 * @param serviceId
 * @param feedId
 * @param topic
 * @return 
 */
int ICACHE_FLASH_ATTR topicOnrampPublish(   Mqtt*       mqtt,
                                            const char* nodename,
                                            const char* serviceId,
                                            const char* feedId,
                                            char*       topic)
{
    int ret = 0;

    strcpy(topic, mqtt->rootTopic);
    strcat(topic, "/");
    strcat(topic, nodename);
    strcat(topic, "/$feeds/$onramp/");
    strcat(topic, mqtt->actorPlatformId);
    strcat(topic, "/");
    strcat(topic, serviceId);
    strcat(topic, "/");
    strcat(topic, feedId);

    return ret;    
}
/**
 * 
 * @param t
 * @param topic
 * @return 
 */
int ICACHE_FLASH_ATTR tokenBegin(FABRIC_TOKEN* t, char* topic, int len)
{
    t->m_Topic = topic;
    
    if(topic == 0) {
        DTXT("fabricTokenBegin(): topic == 0\n");
        return -1;
    }
    else if(topic[0] == '\0') {
        DTXT("fabricTokenBegin(): topic[0] == 0\n");
        t->m_Topic = 0;
        return -1;
    }
    else if(len < 1) {
        DTXT("fabricTokenBegin(): len < 1\n");
        t->m_Topic = 0;
        return -1;
    }
    
    t->m_TopicLen = len;
    t->m_P        = t->m_Topic;
    t->m_Pp       = t->m_Topic;
    t->m_Words    = 0;
    t->m_I        = 0;
    
    return 0;
}
/**
 * 
 * @param t
 * @param subtopic
 * @param index
 * @return 
 */
int ICACHE_FLASH_ATTR tokenNext(FABRIC_TOKEN* t, char** subtopic, int* index)
{
    if(t->m_Topic == 0) {
        //DTXT("fabricTokenNext(): m_Topic == 0\n");
        return -1;
    }
    
    while(t->m_I < t->m_TopicLen) {
        if(*t->m_P == '/') {
            *t->m_P = '\0';

            *subtopic = t->m_Pp;
            *index    = t->m_Words;
            
            ++t->m_Words;
            
            t->m_Pp = t->m_P + 1;
            
            ++t->m_P;
            ++t->m_I;
            
            return 0;
        }

        ++t->m_P;
        ++t->m_I;
    }

    int len = t->m_P - t->m_Pp;
    
    //DTXT("fabricTokenNext(): len = %d\n", len);
    
    if(len > 0) {
        //
        // note: 'm_Pp' is not zero-terminated at this point
        //
        strncpy(t->m_LastSubtopic, t->m_Pp, len);
        t->m_LastSubtopic[len] = '\0';
        
        *subtopic = t->m_LastSubtopic;
        *index    = t->m_Words;
        
        // no more in this topic-string
        t->m_Topic = 0;
        
        return 0;
    }
    
    DTXT("fabricTokenNext(): len <= 0\n");
    
    return -1;
}
/**
 * 
 * @param t
 * @return 
 */
int ICACHE_FLASH_ATTR tokenReset(FABRIC_TOKEN* t)
{
    // no more in this topic-string
    t->m_Topic = 0;

    return 0;
}
