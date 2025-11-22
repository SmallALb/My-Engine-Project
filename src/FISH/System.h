#pragma once

#include "Layer.h"
#include "FISH/Base/TaskPool.h"
#include "FISH/Base/Ecs/Registry.h"

namespace FISH{

  //---
  //## 系统类：                                               
  //  - 继承于层级类，利用Layer的OnUpdate进行主线程操作
  //  - 利用AsyncUpdate进行线程池中的异步操作
  //  - 所有的派生系统共用一个异步线程池                       
  //---
  class FISH_System : public Layer {
  public:
    FISH_System();

  protected:
    ~FISH_System();

    //异步更新操作
    virtual void AsyncUpdate() {};

    void submit();
  protected:
    //该系统注册表
    Registry mRegistry;
    mutable std::mutex mRegistryMutex;
  private:
    //全局线程池
    static TaskPool mTaskPool;

  };
}