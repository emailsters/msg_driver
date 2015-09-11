#ifndef _MSG_SCHEDULER_H_
#define _MSG_SCHEDULER_H_
#include <map>
#include <pthread.h>
#include "class_macros.h"
#include "task_engine.h"
#include "locker.h"

namespace platform
{
class MsgScheduler
{
DISALLOW_COPY_AND_ASSIGN(MsgScheduler);
public:
    static MsgScheduler* GetInstance();
    void Release();
    int Regist(TaskEngine* engine);
    void UnRegist(uint8_t component_id);
    void SendMsg(const Message* msg);
    void Schedule();

private:
    MsgScheduler();
    ~MsgScheduler();
    static void Init();

    typedef std::map<uint8_t, TaskEngine*>::iterator engine_itor;
    static MsgScheduler* s_scheduler_;
    std::map<uint8_t, TaskEngine*> engines_;
    MutexLock msg_dispatch_lock_;
    static pthread_once_t s_once_;
};
} // namespace platform
#endif
