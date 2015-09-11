#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_
#include "class_macros.h"
#include <pthread.h>

namespace platform
{
// not thread safe, just use it in main thread
class Controller
{
DISALLOW_COPY_AND_ASSIGN(Controller);
public:
    static Controller* GetInstance();
    void Release();
    int Run();
    int Suspend();
private:
    Controller(){}
    ~Controller(){}
    static void Init();
    static Controller* s_controller_;
    static pthread_once_t s_once_;
};
}
#endif
