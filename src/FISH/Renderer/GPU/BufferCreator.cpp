#include "fspcs.h"
#include "BufferCreator.h"
#include "Platform/OpenGL/GLBufferCreator.h"
#include "FISH/Renderer/API.h"

namespace FISH {
  BufferGpuHandle BufferCreator::CreateVertexBuffer(const std::vector<float> &vertices, BufferType bufferType) {
    std::unique_ptr<BufferCreator> Creator;
    switch(GetAPI()) {
      case RendererAPI::OpenGL: {
        Creator.reset(new GLBufferCreator()); break;
      }
    }
    return Creator->create(vertices, bufferType);
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

  void BufferCreator::BindVertex(BufferGpuHandle &handle) {
    std::unique_ptr<BufferCreator> Creator;
    switch(GetAPI()) {
      case RendererAPI::OpenGL: {
        Creator.reset(new GLBufferCreator()); break;
      }
    }
    Creator->bind(handle);
  }