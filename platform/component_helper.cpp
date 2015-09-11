#include "component_helper.h"
#include <stdio.h>
#include <dlfcn.h>
#include "task_engine.h"
#include "common_macros.h"
#include "tinyxml.h"
#include "tools.h"

namespace platform
{
ComponentHelper* ComponentHelper::s_helper_ = NULL;
pthread_once_t ComponentHelper::s_once_ = PTHREAD_ONCE_INIT;
ComponentHelper::ComponentHelper(){}
ComponentHelper::~ComponentHelper(){}

void ComponentHelper::Init()
{
    s_helper_ = new ComponentHelper;
}

ComponentHelper* ComponentHelper::GetInstance()
{
    pthread_once(&s_once_, Init);
    return s_helper_;
}

void ComponentHelper::Release()
{
    delete s_helper_;
    s_helper_ = NULL;
}

int ComponentHelper::LoadAllComponents(const std::string& conf_file)
{
    TiXmlDocument proc_conf_doc(conf_file.c_str());
    bool is_load_ok = proc_conf_doc.LoadFile();
    if (!is_load_ok)
    {
        printf("load proconf.xml failed\n");
        return -1;
    }
    TiXmlElement* root = proc_conf_doc.RootElement();
    for (TiXmlNode* node = root->FirstChild("component");
        node; node = node->NextSibling("component"))
    {
        int ret = 0;
        Component* component = NULL;
        int cid = 0;
        do
        {
            TiXmlElement* element = node->ToElement();
            if (!element)
            {
                ret = -1;
                printf("element is null\n");
                break;
            }
            std::string cname = std::string(element->Attribute("name"));
            element->QueryIntAttribute("cid", &cid);

            std::string share_lib_name = GetProcPath() + "/" + "lib" + cname + ".so";
            if (0 != access(share_lib_name.c_str(), F_OK))
            {
                printf("file %s not exist\n", share_lib_name.c_str());
                ret = -1;
                break;
            }
            component = CreateComponent(share_lib_name);
            if (!component)
            {
                printf("create component failed\n");
                ret = -1;
                break;
            }
        } while(0);

        if (ret != 0)
        {
            ReleaseComponent();
            return -1;
        }

        component->set_component_id(cid);
        components_.insert(std::make_pair(cid, component));
    }
    return 0;
}

void ComponentHelper::ReleaseComponent()
{
    printf("release component\n");
    component_itor itor = components_.begin();
    for (; itor != components_.end(); ++itor)
    {
        delete itor->second;
    }
    components_.clear();

    component_dl_itor dl_itor = component_dl_libs_.begin();
    for (; dl_itor != component_dl_libs_.end(); ++dl_itor)
    {
        dlclose(*dl_itor);
    }
    component_dl_libs_.clear();
}

Component* ComponentHelper::CreateComponent(const std::string& share_lib_name)
{
    void* share_lib_handler = dlopen(share_lib_name.c_str(), RTLD_LAZY);
    if (!share_lib_handler)
    {
        printf("dlopen err:%s.\n",dlerror());  
        return NULL;
    }
    component_dl_libs_.push_back(share_lib_handler);

    CreateComponentObjFunc func = reinterpret_cast<CreateComponentObjFunc>(
        dlsym(share_lib_handler, CREATE_COMPONENT_OBJ_FUNC_SYMBOL));
    Component* component = NULL;
    if (!func)
    {
        printf("dlsym err:%s.\n", dlerror());
        return NULL;
    }
    component = reinterpret_cast<Component*>(func());
    return component;
}

int ComponentHelper::InstallAllComponents()
{
    component_itor itor = components_.begin();
    for (; itor != components_.end(); ++itor)
    {
        TaskEngine* engine = new TaskEngine;
        if (!engine) continue;
        engine->BindComponent(itor->second);
        MsgSchedulerInstance()->Regist(engine);
    }

    if (InitComponents() != 0)
    {
        UninstallAllComponents();
        return -1;
    }

    if (ActivateComponents() != 0)
    {
        UninstallAllComponents();
        return -1;
    }

    MsgSchedulerInstance()->Schedule();
    return 0;
}

int ComponentHelper::InitComponents()
{
    component_itor itor = components_.begin();
    for (; itor != components_.end(); ++itor)
    {
        if(itor->second && itor->second->Init() != 0)
        {
            printf("component %d init failed\n", itor->first);
            return -1;
        }
    }
    return 0;
}

int ComponentHelper::ActivateComponents()
{
    component_itor itor = components_.begin();
    for (; itor != components_.end(); ++itor)
    {
        if(itor->second)
        {
            if (itor->second->Active() != 0)
            {
                printf("component %d active failed\n", itor->first);
                return -1;
            }            
        }
        else
        {
            printf("component is null, active failed\n");
            return -1;
        }
    }
    return 0;
}

int ComponentHelper::UninstallAllComponents()
{
    MsgScheduler* scheduler = MsgScheduler::GetInstance();
    if (!scheduler) return -1;
    component_itor itor = components_.begin();
    for (; itor != components_.end(); ++itor)
    {
        scheduler->UnRegist(itor->first);
        Component* component = itor->second;
        if (component)
        {
            component->Fini();
            delete component;
        }
    }
    components_.clear();
    return 0;
}

Component* ComponentHelper::GetComponent(uint8_t component_id)
{
    component_itor itor = components_.find(component_id);
    if(itor != components_.end())
    {
        return itor->second;
    }
    return NULL;
}
}
