#include "controller.h"
#include <stdio.h>
#include "common_macros.h"
#include "tools.h"
namespace platform
{
Controller* Controller::s_controller_ = NULL;
pthread_once_t Controller::s_once_ = PTHREAD_ONCE_INIT;
void Controller::Init()
{
    s_controller_ = new Controller;
}

Controller* Controller::GetInstance()
{
    pthread_once(&s_once_, &Controller::Init);
    return s_controller_;
}

void Controller::Release()
{
    delete s_controller_;
    s_controller_ = NULL;
}

int Controller::Run()
{
    if (ComponentHelperInstance()->LoadAllComponents("../conf/proconf.xml")
        != 0)
    {
        printf("load components failed\n");
        return -1;
    }

    if (ComponentHelperInstance()->InstallAllComponents() != 0)
    {
        printf("install components failed\n");
        return -1;
    }
    return 0;
}

int Controller::Suspend()
{
    if (ComponentHelperInstance()->UninstallAllComponents() != 0)
    {
        printf("uninstall components failed\n");
        return -1;
    }
    return 0;
}
}
