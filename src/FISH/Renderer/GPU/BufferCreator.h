#pragma once

#include "FISH/Base/Ecs/Component.h"
#include "../D/BufferData.h"

namespace FISH {
  struct BufferGpuHandle;

  class API_ BufferCreator {
  public:
    static BufferGpuHandle CreateVertexBuffer(const std::vector<float>& vertices, BufferType bufferType);

    static void Destory(BufferGpuHandle& handle);

    static void BindVertex(BufferGpuHandle& handle);
  protected:
    virtual BufferGpuHandle create(const std::vector<float>& vertices, BufferType bufferType) = 0;

    virtual void destory(BufferGpuHandle& handle) = 0;

    virtual void bind(BufferGpuHandle& handle) = 0;
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