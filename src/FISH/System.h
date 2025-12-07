#pragma once

#include "Layer.h"
#include "FISH/Base/TaskPool.h"
#include "FISH/Base/Ecs/Registry.h"

namespace FISH{

  //---
  //## FISH_System Interface                                               
  //  - Inheritsfrom the Layer class, uses the <OnUpdate> to update in the main thread;
  //  - uses the <AsyncUpdate> to update asynchronously in the other threads;
  //  - All derivative Classes inheriting from FISH_System sharethe same static TaskPool, and have a single Registry
  //  - When you need to <AsyncUpdate> to do any else, overridding the <AsyncUpdate> method and use the <submit> to add a task to the TaskPool    
  //---
  class FISH_System : public Layer {
  public:
    FISH_System();

  protected:
    ~FISH_System() override;

    //异步更新操作
    virtual void AsyncUpdate() {};

    void submit();
  protected:
    //该系统注册表
    Registry mRegistry;
  private:
    //全局线程池
    static TaskPool mTaskPool;

  };
}