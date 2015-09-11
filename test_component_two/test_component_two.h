#ifndef _test_component_2_h_
#define _test_component_2_h_
#include <stdint.h>
#include "message.h"
#include "component.h"
#include "base_component.h"
namespace platform
{
class Component2 : public BaseComponent
{
    DECLEAR_MESSAGE_MAP();
public:
    Component2(){}
    virtual ~Component2(){}
    int Init();
    int Active();
    int Fini();
    int OnHeihei(const Message* msg);
};
}
#endif
