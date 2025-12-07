#include "fspcs.h"
#include "FISH/Renderer/D/BufferData.h"
#include "BufferCreator.h"
#include "FISH/Log.h"
#include "Platform/OpenGL/GLBufferCreator.h"
#include "FISH/Renderer/API.h"

namespace FISH {
  BufferGpuHandle BufferCreator::CreateBuffer(const BufferData& vertices) {
    std::unique_ptr<BufferCreator> Creator;
    switch(GetAPI()) {
      case RendererAPI::OpenGL: {
        Creator.reset(new GLBufferCreator()); break;
      }
      default:
        FS_INFO("No API!"); return BufferGpuHandle();

    }
    return Creator->create(vertices);
  }

  void BufferCreator::Destory(BufferGpuHandle &handle) {
    std::unique_ptr<BufferCreator> Creator;
    switch(GetAPI()) {
      case RendererAPI::OpenGL: {
        Creator.reset(new GLBufferCreator()); break;
      }
      default:
        FS_INFO("No API!"); break;
    }
    Creator->destory(handle);
  }
  
}