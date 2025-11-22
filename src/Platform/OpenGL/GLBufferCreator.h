#pragma once
#include "FISH/Renderer/GPU/BufferCreator.h"

namespace FISH {
  class GLBufferCreator : public BufferCreator {
  public:
    BufferGpuHandle create(const std::vector<float>& vetices, BufferType bufferType) override;

    void destory(BufferGpuHandle& handle) override;
  };

}