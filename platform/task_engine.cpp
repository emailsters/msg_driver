#include "task_engine.h"
#include <stdio.h>
#include <unistd.h>
namespace platform
{
void* TaskFunc(void* arg)
{
    TaskEngine* engine = reinterpret_cast<TaskEngine*>(arg);
    if (engine)
    {
        engine->Svc();
    }
    return NULL;
}

TaskEngine::TaskEngine():task_id_(0), component_(NULL),
    running_(false), condition_(&msg_queue_lock_)
{
}

TaskEngine::~TaskEngine()
{
    // free messages had not processed here
    size_t queue_size = msg_queue_.size();
    for (size_t i = 0; i < queue_size; ++i)
    {
        Message* msg = msg_queue_.front();
        msg_queue_.pop();
        delete msg;
    }
}

int TaskEngine::Start()
{
    int ret = pthread_create(&task_id_, NULL, TaskFunc, this);
    if (ret != 0)
    {
        printf("create thread error [%s, %u]\n", __FILE__, __LINE__);
    }
    return ret;
}

int TaskEngine::PutMessage(Message* msg)
{
    LockGuard guard(&msg_queue_lock_);
    if (msg_queue_.size() >= MAX_MESSAGE_COUNT)
    {
        printf("queue is full\n");
        return -1;
    }
    msg_queue_.push(msg);
    condition_.NotifyAll();
    return 0;
}

void TaskEngine::Svc()
{
    running_ = true;
    printf("task %lu start to service\n", pthread_self());
    while (running_)
    {
        const Message* msg = TakeMessage();
        if (msg)
        {
            (void)ProcessMessage(msg);
            delete msg;

            // sleep a while to free cpu
            usleep(200);
        }
    }
}

int TaskEngine::ProcessMessage(const Message* msg)
{
    if (!component_) return -1;
    const MessageEntry* msg_entry = component_->GetMessageEntry();
    bool is_found = false;
    for (int i = 0; msg_entry[i].func; ++i)
    {
        if (msg->msg_id == msg_entry[i].msg_id)
        {
            OnMsgFunc func = msg_entry[i].func;
            (component_->*func)(msg);
            is_found = true;
            break;
        }
    }
    if (!is_found)
    {
        printf("message %d not found\n", msg->dest_cid);
    }
    return 0;
}

const Message* TaskEngine::TakeMessage()
{
    LockGuard guard(&msg_queue_lock_);
    Message* msg = NULL;
    while (msg_queue_.empty() && running_)
    {
        condition_.Wait();
    }
    if (!msg_queue_.empty())
    {
        msg = msg_queue_.front();
        msg_queue_.pop();
    }
    return msg;
}

void TaskEngine::Stop()
{
    running_ = false;
    condition_.NotifyAll();
    pthread_join(task_id_, NULL);
}

void TaskEngine::BindComponent(Component* component)
{
    printf("engine bind component %d\n", component->get_component_id());
    component_ = component;
}
}
