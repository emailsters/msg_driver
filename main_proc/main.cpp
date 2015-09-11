#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "base_component.h"
#include "task_engine.h"
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "msg_scheduler.h"
#include "common_macros.h"

using namespace platform;
int ParseArgs(int argc, char const* argv[])
{
    for (int i = 0; i < argc; ++i)
    {
        // todo: ./procmain -mode daemon -conf_file file
    }
    return 0;
}

void SignalHandle(int signo)
{
    printf("catch signal %d\n", signo);
    ReleaseWhenExit();
    signal(SIGTERM, SIG_DFL);
    signal(SIGINT, SIG_DFL);
    printf("exit\n");
    exit(0);
}

int main(int argc, char const *argv[])
{
    if (ParseArgs(argc, argv) != 0)
    {
        printf("parse args error\n");
        return -1;
    }

    struct sigaction sig;  
    sig.sa_handler = SignalHandle;  
    sigemptyset(&sig.sa_mask);  
    sig.sa_flags = 0;  
    sigaction(SIGINT, &sig, NULL);
    sigaction(SIGTERM, &sig, NULL);
    
    SystemInit();
    ControllerInstance()->Run();
    
    while (1)
    {
        sleep(1000000);
    }
    
    return 0;
}
