#ifndef _COMMON_MACROS_H_
#define _COMMON_MACROS_H_
#include "msg_scheduler.h"
#include "controller.h"
#include "component_helper.h"
#include "msg_scheduler.h"

namespace platform
{
inline ComponentHelper* ComponentHelperInstance()
{
    return ComponentHelper::GetInstance();
}

inline Controller* ControllerInstance()
{
    return Controller::GetInstance();
}

inline MsgScheduler* MsgSchedulerInstance()
{
    return MsgScheduler::GetInstance();
}

inline void SystemInit()
{
	ComponentHelperInstance();
	MsgSchedulerInstance();
	ControllerInstance();
}

inline void ReleaseWhenExit()
{
    ControllerInstance()->Suspend();
    ControllerInstance()->Release();
    MsgSchedulerInstance()->Release();
    ComponentHelperInstance()->Release();
}
}
#endif
