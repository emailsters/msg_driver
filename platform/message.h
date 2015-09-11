#ifndef _MESSAGE_H_
#define _MESSAGE_H_
namespace platform
{
#define MAX_MSG_LENGTH (1024)
struct Message
{
    uint8_t src_cid;
    uint8_t dest_cid;
    uint32_t msg_id;
    char msg[MAX_MSG_LENGTH];
    uint32_t msg_length;
};

enum MessagesDefine
{
    ON_RECEIVE_FROM_WORLD = 560
};
}
#endif
