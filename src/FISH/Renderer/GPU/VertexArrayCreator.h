#pragma once

#include "FISH/Base/Ecs/Component.h"

namespace FISH {
  
  //### VertexArrayGpuHandle
  struct VertexArrayGpuHandle : public Component {
    VertexArrayHandleI* HANDLE;
    uint32_t buffercount{0};
    uint32_t attributecount{0};

    VertexArrayGpuHandle() : HANDLE(nullptr), buffercount(0) {}

    virtual std::string getComponentName() const {return "VertexArrayGpuHandle";};
  };

  class VertexArrayCreator {
  public:
    static VertexArrayGpuHandle CreateVertexArray();

    static void AddBuffer(VertexArrayGpuHandle& handle, uint32_t entity);

    static void Destory(VertexArrayGpuHandle& handle);
    
    static void Bind(VertexArrayGpuHandle& handle);

    static void SetIndex(VertexArrayGpuHandle& handle, uint32_t entity);

    static void Draw(uint32_t beginIndex, uint32_t count, int ElementType);
  protected:
    virtual VertexArrayGpuHandle create() = 0;

    virtual void addbuffer(VertexArrayGpuHandle& handle, uint32_t entity) = 0;

    virtual void destory(VertexArrayGpuHandle& handle) = 0;
    
    virtual void bind(VertexArrayGpuHandle& handle) = 0;

    virtual void renderIndexBuffer(uint32_t beginIndex, uint32_t count, int ElementType) = 0;

    virtual void setIndex(VertexArrayGpuHandle& handle, uint32_t entity) = 0;
  };

}