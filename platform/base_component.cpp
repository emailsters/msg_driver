#include "base_component.h"
#include <stdio.h>
#include <string.h>
#include "common_macros.h"

namespace platform
{
BaseComponent::BaseComponent():component_id_(0), name_(""){}
BaseComponent::~BaseComponent(){}

void BaseComponent::SendMsg(uint8_t dest_cid, uint16_t msg_id, const char* msg, 
    uint32_t msg_len)
{
    Message* messge = new Message;
    if (messge)
    {
        messge->dest_cid = dest_cid;
        messge->src_cid = component_id_;
        messge->msg_id = msg_id;
        messge->msg[0] = 0;
        messge->msg_length = msg_len;
        if (msg)
        {
            memcpy(messge->msg, msg, MAX_MSG_LENGTH);
            messge->msg[MAX_MSG_LENGTH - 1] = 0;
        }
        MsgSchedulerInstance()->SendMsg(messge);
    }
}
} // namespace platform
