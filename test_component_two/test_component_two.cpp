#include "test_component_two.h"
#include <stdio.h>
#include <assert.h>
#include "common_macros.h"

extern "C"
{
platform::Component* CreateComponentObj()
{
    return new platform::Component2;
}
}

namespace platform
{
BEGINE_MESSAGE_MAP(Component2)
    OM_MESSAGE(1, Component2::OnHeihei)
END_MESSAGE_MAP()

int Component2::Init()
{
    printf("Component2 init\n");
    return 0;
}

int Component2::Active()
{
    printf("Component2 active\n");
    return 0;
}

int Component2::Fini()
{
    printf("Component2 fini\n");
    return 0;
}

int Component2::OnHeihei(const Message* msg)
{
    assert(msg != NULL);
    printf("Component2 recv, src_id:%d, dest_id:%d, msg_id:%d\n", 
        msg->src_cid, msg->dest_cid, msg->msg_length);
    SendMsg(1, 1, "aaa", sizeof("aaa"));
    return 0;
}
}
