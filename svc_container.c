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

#include "svc_container.h"
#include <github.com/mikejac/realtimelogic.json.esp8266-nonos.cpp/JEncoder.h>
#include <osapi.h>
#include <mem.h>

#define DTXT(...)   os_printf(__VA_ARGS__)
//#define DTXT(...)

/******************************************************************************************************************
 * prototypes
 *
 */

/**
 * 
 * @param o
 * @param a
 * @return 
 */
static int marshalAccessory(JEncoder* o, Accessory* a);
/**
 * 
 * @param o
 * @param s
 * @return 
 */
static int marshalService(JEncoder* o, Service* s);
/**
 * 
 * @param o
 * @param c
 * @return 
 */
static int marshalCharacteristic(JEncoder* o, Characteristic* c);
/**
 * 
 * @param o
 * @param name
 * @param format
 * @param value
 * @return 
 */
static int marshalValue_private(JEncoder* o, const char* name, CharacteristicFormat format, CharacteristicValue* value);
/**
 * 
 * @param o
 * @param sizeRequired
 * @return 
 */
static int _BufPrint_flush(BufPrint* o, int sizeRequired);

/******************************************************************************************************************
 * public functions
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
Container* ICACHE_FLASH_ATTR NewContainer(const char* nodename, const char* name, const char* serialnumber, const char* manufacturer, const char* model, int marshalBufferSize)
{
    Container* cont = (Container*) os_zalloc(sizeof(Container));
    if(cont == 0) {
        DTXT("NewContainer(Container): mem fail\n");
        return 0;
    }
    
    cont->idCount           = 1;
    cont->marshalBufferSize = marshalBufferSize;
    cont->Nodename          = nodename;
    cont->Name              = name;
    cont->SerialNumber      = serialnumber;
    cont->Manufacturer      = manufacturer;
    cont->Model             = model;
    
    return cont;
}
/**
 * AddAccessory adds an accessory to the container.
 * This method ensures that the accessory ids are valid and unique withing the container.
 * @param cont
 * @param a
 * @return 
 */
sint64_t ICACHE_FLASH_ATTR AddAccessory(Container* cont, Accessory* a)
{
    if(cont == 0) {
        DTXT("AddAccessory(): 'cont' is nil\n");
        return -1;
    } else if(a == 0) {
        DTXT("AddAccessory(): 'a' is nil\n");
        return -1;
    }
    
    // first, set id
    a->ID = cont->idCount;
    cont->idCount++;
    
    a->parent = cont;
    
    // now append the accessory
    if(cont->Accessories != NULL) {
        Accessory* ptr = cont->Accessories;
        
        while(ptr->next != NULL) {
            ptr = ptr->next;
        }
        
        ptr->next = a;
    } else {
        cont->Accessories = a;
    }
    
    return a->ID;
}
/**
 * 
 * @param cont
 * @param aid
 * @return 
 */
Accessory* ICACHE_FLASH_ATTR FindByAid(Container* cont, sint64_t aid)
{
    if(cont == 0) {
        DTXT("FindByAid(): 'cont' is nil\n");
        return 0;
    }
    
    if(cont->Accessories != NULL) {
        Accessory* ptr = cont->Accessories;
        
        while(ptr != NULL) {
            if(ptr->ID == aid) {
                return ptr;
            }
            
            ptr = ptr->next;
        }
    }

    return 0;
}
/**
 * 
 * @param aid
 * @param iid
 * @param format
 * @param value
 * @return 
 */
char* ICACHE_FLASH_ATTR MarshalValue(sint64_t aid, sint64_t iid, CharacteristicFormat format, CharacteristicValue* value)
{
    JEncoder o;
    JErr     err;
    BufPrint out;
    char*    b;
    
    b = (char*) os_malloc(1024);
    if(b == 0) {
        DTXT("MarshalValue(malloc): mem fail");
        return 0;
    }
    
    BufPrint_constructor(&out, NULL, _BufPrint_flush);
    BufPrint_setBuf(&out, b, 1024);
    
    JErr_constructor(&err);
    
    JEncoder_constructor(&o, &err, &out);

    // begin the object
    JEncoder_beginObject(&o);
    
    JEncoder_setName(&o, "d");    
    JEncoder_beginObject(&o);
    
    JEncoder_setName(&o, "_type");
    switch(format) {
        case FormatString:
            JEncoder_setString(&o, FormatStringTxt);
            break;

        case FormatBool:
            JEncoder_setString(&o, FormatBoolTxt);
            break;
            
        case FormatUInt8:
            JEncoder_setString(&o, FormatUInt8Txt);
            break;
            
        case FormatInt8:
            JEncoder_setString(&o, FormatInt8Txt);
            break;
            
        case FormatUInt16:
            JEncoder_setString(&o, FormatUInt16Txt);
            break;
            
        case FormatInt16:
            JEncoder_setString(&o, FormatInt16Txt);
            break;
            
        case FormatUInt32:
            JEncoder_setString(&o, FormatUInt32Txt);
            break;
            
        case FormatInt32:
            JEncoder_setString(&o, FormatInt32Txt);
            break;
            
        case FormatUInt64:
            JEncoder_setString(&o, FormatUInt64Txt);
            break;
            
        case FormatFloat:   
            JEncoder_setString(&o, FormatFloatTxt);
            break;
            
        case FormatNone:
            break;
    }

    JEncoder_setName(&o, "aid");  JEncoder_setLong(&o, aid);
    JEncoder_setName(&o, "iid");  JEncoder_setLong(&o, iid);

    marshalValue_private(&o, "value", format, value);

    // end the object
    JEncoder_endObject(&o);     // "d"
    JEncoder_endObject(&o);
    
    if(JErr_isError(&err)) {
        DTXT("MarshalValue(): fail; err = %s\n", JErr_getErrS(&err));
        os_free(b);
        
        return 0;
    } else {
        JEncoder_commit(&o);    // this will activate _BufPrint_flush()
        
        return b;
    }
    
}
/**
 * 
 * @param cont
 * @return 
 */
char* ICACHE_FLASH_ATTR marshalContainer(Container* cont)
{
    JEncoder o;
    JErr     err;
    BufPrint out;
    char*    b;
    
    b = (char*) os_malloc(cont->marshalBufferSize);
    if(b == 0) {
        DTXT("MarshalContainer(malloc): mem fail");
        return 0;
    }
    
    BufPrint_constructor(&out, NULL, _BufPrint_flush);
    BufPrint_setBuf(&out, b, cont->marshalBufferSize);
    
    JErr_constructor(&err);
    
    JEncoder_constructor(&o, &err, &out);
    
    JEncoder_beginObject(&o);
    
    JEncoder_setName(&o, "d");    
    JEncoder_beginObject(&o);
    
    JEncoder_setName(&o, "_type");          JEncoder_setString(&o, "accessories_list");
    JEncoder_setName(&o, "nodename");       JEncoder_setString(&o, cont->Nodename);
    JEncoder_setName(&o, "name");           JEncoder_setString(&o, cont->Name);
    JEncoder_setName(&o, "model");          JEncoder_setString(&o, cont->Model);
    JEncoder_setName(&o, "serialnumber");   JEncoder_setString(&o, cont->SerialNumber);
    JEncoder_setName(&o, "manufacturer");   JEncoder_setString(&o, cont->Manufacturer);

    JEncoder_setName(&o, "value");
    JEncoder_beginObject(&o);
    
    JEncoder_setName(&o, "accessories");
    JEncoder_beginArray(&o);
    
    for(Accessory* a = cont->Accessories; a != NULL; a = a->next) {
        marshalAccessory(&o, a);
    }
    
    JEncoder_endArray(&o);      // "accessories"
    
    JEncoder_endObject(&o);     // "value"
    JEncoder_endObject(&o);     // "d"
    JEncoder_endObject(&o);
    
    if(JErr_isError(&err)) {
        DTXT("MarshalContainer(): fail; err = %s\n", JErr_getErrS(&err));
        os_free(b);
        
        return 0;
    } else {
        JEncoder_commit(&o);    // this will activate _BufPrint_flush()
        DTXT("MarshalContainer(): b = %s\n", b);
        
        return b;
    }
}
/**
 * 
 * @param o
 * @param a
 * @return 
 */
int ICACHE_FLASH_ATTR marshalAccessory(JEncoder* o, Accessory* a)
{
    JEncoder_beginObject(o);
    JEncoder_setName(o, "aid");  JEncoder_setLong(o, a->ID);
    JEncoder_setName(o, "type"); JEncoder_setInt(o, a->Type);

    JEncoder_setName(o, "services");
    JEncoder_beginArray(o);

    for(Service* svc = a->Service; svc != NULL; svc = svc->next) {
        marshalService(o, svc);
    }

    JEncoder_endArray(o);   // "services"
    JEncoder_endObject(o);

    return 0;
}
/**
 * 
 * @param o
 * @param s
 * @return 
 */
int ICACHE_FLASH_ATTR marshalService(JEncoder* o, Service* s)
{
    JEncoder_beginObject(o);
    JEncoder_setName(o, "iid");  JEncoder_setLong(o, s->ID);
    JEncoder_setName(o, "type"); JEncoder_setString(o, s->Type);

    JEncoder_setName(o, "characteristics");
    JEncoder_beginArray(o);

    for(Characteristic* ch = s->Characteristics; ch != NULL; ch = ch->next) {
        marshalCharacteristic(o, ch);
    }

    JEncoder_endArray(o);       // "characteristics"
    JEncoder_endObject(o);

    return 0;
}
/**
 * 
 * @param o
 * @param c
 * @return 
 */
int ICACHE_FLASH_ATTR marshalCharacteristic(JEncoder* o, Characteristic* c)
{
    JEncoder_beginObject(o);
    JEncoder_setName(o, "iid");  JEncoder_setLong(o, c->ID);
    JEncoder_setName(o, "type"); JEncoder_setString(o, c->Type);

    // perms
    JEncoder_setName(o, "perms");
    JEncoder_beginArray(o);

    if((c->Perms & PermRead) == PermRead) {
        JEncoder_setString(o, "pr");
    }
    if((c->Perms & PermWrite) == PermWrite) {
        JEncoder_setString(o, "pw");
    }
    if((c->Perms & PermEvents) == PermEvents) {
        JEncoder_setString(o, "ev");
    }
    
    JEncoder_endArray(o);       // "perms"

    // value - optional
    marshalValue_private(o, "value", c->Format, c->Value);

    // format
    JEncoder_setName(o, "format");

    switch(c->Format) {
        case FormatString:  
            JEncoder_setString(o, FormatStringTxt);    
            break;
        case FormatBool:    
            JEncoder_setString(o, FormatBoolTxt);    
            break;
        case FormatUInt8:   
            JEncoder_setString(o, FormatUInt8Txt);    
            break;
        case FormatInt8:    
            JEncoder_setString(o, FormatInt8Txt);    
            break;
        case FormatUInt16:  
            JEncoder_setString(o, FormatUInt16Txt);    
            break;
        case FormatInt16:   
            JEncoder_setString(o, FormatInt16Txt);    
            break;
        case FormatUInt32:  
            JEncoder_setString(o, FormatUInt32Txt);    
            break;
        case FormatInt32:   
            JEncoder_setString(o, FormatInt32Txt);    
            break;
        case FormatUInt64:  
            JEncoder_setString(o, FormatUInt64Txt);    
            break;
        case FormatFloat:   
            JEncoder_setString(o, FormatFloatTxt);    
            break;
        case FormatNone:
            break;
    }
    
    // unit - optional
    if(c->Unit != NULL) {
        JEncoder_setName(o, "unit");
        JEncoder_setString(o, c->Unit);
    }
    
    // maxValue - optional
    marshalValue_private(o, "maxValue", c->Format, c->MaxValue);
    
    // minValue - optional
    marshalValue_private(o, "minValue", c->Format, c->MinValue);
    
    // minStep - optional
    marshalValue_private(o, "minStep", c->Format, c->StepValue);
    
    JEncoder_endObject(o);

    return 0;
}
/**
 * 
 * @param o
 * @param name
 * @param format
 * @param value
 * @return 
 */
int ICACHE_FLASH_ATTR marshalValue_private(JEncoder* o, const char* name, CharacteristicFormat format, CharacteristicValue* value)
{
    if(value != NULL) {
        JEncoder_setName(o, name);
        
        switch(format) {
            case FormatString:  
                JEncoder_setString(o, value->String);    
                break;
            case FormatBool:    
                JEncoder_setBoolean(o, value->Bool);     
                break;
            case FormatUInt8:   
                JEncoder_setInt(o, value->UInt8);        
                break;
            case FormatInt8:    
                JEncoder_setInt(o, value->Int8);         
                break;
            case FormatUInt16:  
                JEncoder_setInt(o, value->UInt16);       
                break;
            case FormatInt16:   
                JEncoder_setInt(o, value->Int16);        
                break;
            case FormatUInt32:  
                JEncoder_setLong(o, value->UInt32);      
                break;
            case FormatInt32:   
                JEncoder_setLong(o, value->Int32);       
                break;
            case FormatUInt64:  
                JEncoder_setLong(o, value->UInt64);      
                break;
            case FormatFloat:   
                JEncoder_setDouble(o, value->Float);     
                break;
            case FormatNone:
                break;
        }        
    }

    return 0;
}
/**
 * BufPrint "flush" callback function used indirectly by JEncoder. The function is called when the buffer is full or if committed.
 * 
 * @param o
 * @param sizeRequired
 * @return 
 */
int ICACHE_FLASH_ATTR _BufPrint_flush(BufPrint* o, int sizeRequired)
{
    BufPrint_getBuf(o)[o->cursor] = '\0';
    
    //DTXT("%s\n", BufPrint_getBuf(o));

    BufPrint_erase(o);      // data flushed (just marked erased, data is still in the buffer)
   
    return 0;    // ok
}
