#ifndef _test_component_1_h_
#define _test_component_1_h_
#include <stdint.h>
#include "message.h"
#include "component.h"
#include "base_component.h"
namespace platform
{
class Component1 : public BaseComponent
{
    DECLEAR_MESSAGE_MAP();
public:
    Component1(){}
    virtual ~Component1(){}
    int Init();
    int Active();
    int Fini();
    int OnHeihei(const Message* msg);
};
}
#endif
