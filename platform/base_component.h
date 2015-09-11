#ifndef _BASE_COMPONENT_H_
#define _BASE_COMPONENT_H_
#include <stdint.h>
#include "component.h"
#include "message.h"
namespace platform
{
class BaseComponent : public Component
{
public:
    BaseComponent();
    virtual ~BaseComponent();
    virtual int Init() = 0;
    virtual int Fini() = 0;
    virtual int Active() = 0;
    virtual uint8_t get_component_id() const
    {
        return component_id_;
    };
    
    virtual void set_component_id(uint8_t component_id)
    {
        component_id_ = component_id;
    }
    
    virtual std::string get_component_name() const
    {
        return name_;
    }
    
    virtual void set_component_name(std::string name)
    {
        name_ = name;
    }

    virtual const MessageEntry* GetMessageEntry() const = 0;
    virtual void SendMsg(uint8_t dest_cid, uint16_t msg_id, const char* msg, 
        uint32_t msg_len);
    
protected:
    uint8_t component_id_;
    std::string name_;
};
} // namespace platform
#endif
