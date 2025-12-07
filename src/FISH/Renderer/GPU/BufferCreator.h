#pragma once

#include "FISH/Base/Ecs/Component.h"

namespace FISH {
  
  //### Buffer Gpu Handle
  struct BufferGpuHandle : public Component {
    BufferHandleI* HANDLE;
    string getComponentName() const override {return "BufferGpuHandle";}
  };

  class API_ BufferCreator {
  public:
    static BufferGpuHandle CreateBuffer(const BufferData& vertices);

    static void Destory(BufferGpuHandle& handle);

  protected:
    virtual BufferGpuHandle create(const BufferData& vertices) = 0;

    virtual void destory(BufferGpuHandle& handle) = 0;
  };
  
  
}