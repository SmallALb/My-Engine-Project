#include "fspcs.h"
#include "BufferCreator.h"

namespace FISH {
  BufferGpuHandle BufferCreator::CreateVertexBuffer(const std::vector<float> &vetices, BufferType bufferType) {
    return BufferGpuHandle();
  }

  void BufferCreator::Destory(BufferGpuHandle &handle) {
  }

  BufferGpuHandle::~BufferGpuHandle() {
    BufferCreator::Destory(*this);
  }
}