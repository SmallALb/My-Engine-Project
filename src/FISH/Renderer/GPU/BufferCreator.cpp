#include "fspcs.h"
#include "BufferCreator.h"
#include "Platform/OpenGL/GLBufferCreator.h"
#include "FISH/Renderer/API.h"

namespace FISH {
  BufferGpuHandle BufferCreator::CreateBuffer(const BufferData& vertices) {
    std::unique_ptr<BufferCreator> Creator;
    switch(GetAPI()) {
      case RendererAPI::OpenGL: {
        Creator.reset(new GLBufferCreator()); break;
      }
    }
    return Creator->create(vertices);
  }

  void BufferCreator::Destory(BufferGpuHandle &handle) {
    std::unique_ptr<BufferCreator> Creator;
    switch(GetAPI()) {
      case RendererAPI::OpenGL: {
        Creator.reset(new GLBufferCreator()); break;
      }
    }
    Creator->destory(handle);
  }
  
}