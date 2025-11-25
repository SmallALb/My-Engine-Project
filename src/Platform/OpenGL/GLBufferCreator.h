#pragma once
#include "FISH/Renderer/GPU/BufferCreator.h"

namespace FISH {
  class GLBufferCreator : public BufferCreator {
  public:
    BufferGpuHandle create(const std::vector<float>& vertices, BufferType bufferType) override;

    void destory(BufferGpuHandle& handle) override;
    
    void bind(BufferGpuHandle& handle) override; 

  private:
    void createVertex(const std::vector<float>& vertices, BufferGpuHandle& handle);

    void createIndex(const std::vector<float>& vertices, BufferGpuHandle& handle);

  };

}