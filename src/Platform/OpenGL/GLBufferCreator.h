#pragma once
#include "FISH/Renderer/GPU/BufferCreator.h"

namespace FISH {
  class GLBufferCreator : public BufferCreator {
  public:
    BufferGpuHandle create(const BufferData& vertices) override;

    void destory(BufferGpuHandle& handle) override;

  private:
    void createVertex(const std::vector<float>& vertices, BufferGpuHandle& handle);

    void createIndex(const std::vector<uint32_t>& vertices, BufferGpuHandle& handle);

  };

}