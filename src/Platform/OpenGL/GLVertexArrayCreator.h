#pragma once

#include "FISH/Renderer/GPU/VertexArrayCreator.h"

namespace FISH {
  struct GLVertexArrayHandle : public VertexArrayHandleI {
    uint32_t bindId;
    string getGpuHandleAPI() const override {return "OpenGL VertexArray Handle";}
  };

  class GLVertexArrayCreator : public VertexArrayCreator {
  public:
    GLVertexArrayCreator() {}

    virtual VertexArrayGpuHandle create() override;

    virtual void addbuffer(VertexArrayGpuHandle& handle, uint32_t entity) override;

    virtual void destory(VertexArrayGpuHandle& handle) override;

    virtual void bind(VertexArrayGpuHandle& handle) override;
    
    virtual void renderIndexBuffer(uint32_t beginIndex, uint32_t count, int ElementType) override;
  
    virtual void setIndex(VertexArrayGpuHandle& handle, uint32_t entity) override;
  };


}