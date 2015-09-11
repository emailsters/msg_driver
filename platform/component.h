#ifndef _COMPONENT_H_
#define _COMPONENT_H_
#include <stdint.h>
#include <string>
#include "message.h"

#define CREATE_COMPONENT_OBJ_FUNC_SYMBOL "CreateComponentObj"
/*
extern "C"
{
#define DECLEAR_CREATE_COMPONENT_OBJ_FUNC() \
Component* CreateComponentObj() \
{ \
    return new platform::component_class; \
}
}
*/

namespace platform
{
class Component;
typedef int (Component::*OnMsgFunc)(const Message*);
struct MessageEntry
{
    uint32_t msg_id;
    OnMsgFunc func;
};

typedef Component* (*CreateComponentObjFunc)();

#define DECLEAR_MESSAGE_MAP() \
protected: \
    static const MessageEntry* GetThisMessageEntry(); \
public: \
    const MessageEntry* GetMessageEntry() const

#define BEGINE_MESSAGE_MAP(class_name) \
const MessageEntry* class_name::GetMessageEntry() const \
{ \
    return GetThisMessageEntry(); \
} \
const MessageEntry* class_name::GetThisMessageEntry() \
{ \
    const static MessageEntry s_msg_entries[] = \
    {

#define OM_MESSAGE(msg_id, on_msg_func) \
        {msg_id, static_cast<OnMsgFunc>(&on_msg_func)},

#define END_MESSAGE_MAP() \
        {0, NULL} \
    }; \
    return &s_msg_entries[0]; \
}


class Component
{
public:
    Component(){}
    virtual ~Component(){};
    virtual int Init() = 0;
    virtual int Active() = 0;
    virtual int Fini() = 0;
    virtual uint8_t get_component_id() const = 0;
    virtual void set_component_id(uint8_t component_id) = 0;
    virtual void set_component_name(std::string name) = 0;
    virtual std::string get_component_name() const = 0;
    virtual const MessageEntry* GetMessageEntry() const = 0;
};
}
#endif
