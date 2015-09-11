#ifndef _COMPONENT_ENGINE_H_
#define _COMPONENT_ENGINE_H_
#include <queue>
#include <pthread.h>
#include "component.h"
#include "message.h"
#include "locker.h"

namespace platform
{
#define MAX_MESSAGE_COUNT (1024 * 256)
class TaskEngine
{
public:
    TaskEngine();
    ~TaskEngine();
    void BindComponent(Component* component);
    int Start();
    void Stop();
    int PutMessage(Message* msg);
    void Svc();
    uint8_t ComponentId() const
    {
        return component_->get_component_id();
    }

private:
    const Message* TakeMessage();
    int ProcessMessage(const Message* msg);
    std::queue<Message*> msg_queue_;
    pthread_t task_id_;
    MutexLock msg_queue_lock_;
    Component* component_;
    bool running_;
    Condition condition_;
};
}

#endif
