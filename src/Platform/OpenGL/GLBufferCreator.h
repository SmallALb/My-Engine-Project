#pragma once
#include "FISH/Renderer/GPU/BufferCreator.h"

namespace FISH {
  struct GLBufferHandle : public BufferHandleI {
    uint32_t bindId;
    virtual string getGpuHandleAPI() const {return "OpenGL Buffer Handle";};
  };

  class GLBufferCreator : public BufferCreator {
  public:
    BufferGpuHandle create(const BufferData& vertices) override;

    void destory(BufferGpuHandle& handle) override;

  private:
    bool createVertex(const std::vector<float>& vertices, GLBufferHandle& handle);

    bool createIndex(const std::vector<uint32_t>& vertices, GLBufferHandle& handle);

  };

}