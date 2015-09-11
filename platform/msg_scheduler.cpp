#include "msg_scheduler.h"
#include <stdio.h>
#include <unistd.h>
namespace platform
{
MsgScheduler::MsgScheduler(){
    printf("constuct\n");
}
MsgScheduler::~MsgScheduler(){
    printf("destruct\n");
}
MsgScheduler* MsgScheduler::s_scheduler_ = NULL;
pthread_once_t MsgScheduler::s_once_ = PTHREAD_ONCE_INIT;

void MsgScheduler::Init()
{
    s_scheduler_ = new MsgScheduler;
}

MsgScheduler* MsgScheduler::GetInstance()
{
    pthread_once(&s_once_, &MsgScheduler::Init);
    return s_scheduler_;
}

void MsgScheduler::Release()
{
    delete s_scheduler_;
    s_scheduler_ = NULL;
}

int MsgScheduler::Regist(TaskEngine* engine)
{
    printf("component %d regist\n", engine->ComponentId());
    engine_itor itor = engines_.find(engine->ComponentId());
    if (itor != engines_.end()) return -1;
    engines_.insert(std::make_pair(engine->ComponentId(),
        engine));
    printf("engine count:%zu\n", engines_.size());
    return 0;
}

void MsgScheduler::UnRegist(uint8_t component_id)
{
    printf("component %d unregist\n", component_id);
    engine_itor itor = engines_.find(component_id);
    if (itor == engines_.end()) return;
    itor->second->Stop();

    // free engine memory here
    delete (itor->second);
    engines_.erase(itor);
}

void MsgScheduler::Schedule()
{
    engine_itor itor = engines_.begin();
    for (; itor != engines_.end(); ++itor)
    {
        if(itor->second) itor->second->Start();
    }
}

void MsgScheduler::SendMsg(const Message* msg)
{
    if (msg)
    {
        printf("%zu engines.send msg from %d to %d msg:%s\n", 
            engines_.size(), msg->src_cid, msg->dest_cid, msg->msg);
        msg_dispatch_lock_.Lock();
        engine_itor itor = engines_.find(msg->dest_cid);
        if (itor == engines_.end())
        {
            printf("not find\n");
            delete msg;
            msg_dispatch_lock_.UnLock();
            return;
        }
        itor->second->PutMessage(const_cast<Message*>(msg));
        msg_dispatch_lock_.UnLock();
    }   
}
}
