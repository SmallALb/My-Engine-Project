#pragma once

#include "FISH/Base/Ecs/Component.h"
#include "../D/BufferData.h"

namespace FISH {
  struct BufferGpuHandle;

  class API_ BufferCreator {
  public:
    static BufferGpuHandle CreateBuffer(const BufferData& vertices);

    static void Destory(BufferGpuHandle& handle);

  protected:
    virtual BufferGpuHandle create(const BufferData& vertices) = 0;

    virtual void destory(BufferGpuHandle& handle) = 0;
  };
  
  //缓冲区句柄
  struct BufferGpuHandle : public Component {
    union {
      uint32_t bindId;
    };
    BufferType bufferType;


    string getComponentName() const {return "BufferGpuHandle";}
  };
}