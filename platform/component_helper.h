#ifndef _COMPONENT_HELPER_H_
#define _COMPONENT_HELPER_H_
#include <iostream>
#include <map>
#include <list>
#include <pthread.h>
#include "class_macros.h"
#include "component.h"

namespace platform
{
class TaskEngine;

// not thread safe, just use it in main thread
class ComponentHelper
{
DISALLOW_COPY_AND_ASSIGN(ComponentHelper);
public:
    static ComponentHelper* GetInstance();
    void Release();
    int LoadAllComponents(const std::string& proc_path);
    int InstallAllComponents();
    int UninstallAllComponents();
    Component* GetComponent(uint8_t component_id);
private:
    ComponentHelper();
    ~ComponentHelper();
    static void Init();
    int InitComponents();
    int ActivateComponents();
    Component* CreateComponent(const std::string& share_lib_name);
    void ReleaseComponent();
    typedef std::map<uint8_t, Component*>::iterator component_itor;
    typedef std::list<void*>::iterator component_dl_itor;
    std::map<uint8_t, Component*> components_;
    static ComponentHelper* s_helper_;
    static pthread_once_t s_once_;
    std::list<void*> component_dl_libs_;
};
}

#endif
