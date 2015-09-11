#include "test_component_one.h"
#include <stdio.h>
#include "common_macros.h"
#include <assert.h>

extern "C"
{
platform::Component* CreateComponentObj()
{
    return new platform::Component1;
}
}

namespace platform
{
BEGINE_MESSAGE_MAP(Component1)
    OM_MESSAGE(1, Component1::OnHeihei)
END_MESSAGE_MAP()

int Component1::Init()
{
    printf("Component1 init\n");
    return 0;
}

int Component1::Active()
{
    printf("Component1 active\n");
    SendMsg(2, 1, "bbb", sizeof("bbb"));
    return 0;
}

int Component1::Fini()
{
    printf("Component1 fini\n");
    return 0;
}

int Component1::OnHeihei(const Message* msg)
{
    assert(msg != NULL);
    printf("Component1 recv msg from %d, msg:%s, msg_length:%d\n",
        msg->src_cid, msg->msg, msg->msg_length);
    return 0;
}
}