#pragma once

#include "FISH/Base/Ecs/Component.h"
#include "../D/BufferData.h"

namespace FISH {
  struct BufferGpuHandle;

  class API_ BufferCreator {
  public:
    static BufferGpuHandle CreateVertexBuffer(const std::vector<float>& vetices, BufferType bufferType);

    static void Destory(BufferGpuHandle& handle);
  protected:
    virtual BufferGpuHandle create(const std::vector<float>& vetices, BufferType bufferType) = 0;

    virtual void destory(BufferGpuHandle& handle) = 0;
  };
  
  //缓冲区句柄
  struct BufferGpuHandle : public Component {
    union {
      float bindId;
    };
    BufferType bufferType;
    ~BufferGpuHandle();

    string getComponentName() const {return "BufferGpuHandle";}
  };
}