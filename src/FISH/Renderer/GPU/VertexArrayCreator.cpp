#include "fspcs.h"
#include "../D/VertexArrayData.h"
#include "VertexArrayCreator.h"
#include "FISH/Log.h"
#include "../API.h"
#include "Platform/OpenGL/GLVertexArrayCreator.h"


namespace FISH {
  VertexArrayGpuHandle VertexArrayCreator::CreateVertexArray() {
    std::unique_ptr<VertexArrayCreator> Creator;
    switch (GetAPI()) {
      case RendererAPI::OpenGL:
        Creator.reset(new GLVertexArrayCreator()); break;
      default:
        FS_INFO("No API!"); return VertexArrayGpuHandle();
    }
    
    return Creator->create();
  }
  void VertexArrayCreator::AddBuffer(VertexArrayGpuHandle &handle, uint32_t entity) {
    std::unique_ptr<VertexArrayCreator> Creator;
    switch (GetAPI()) {
      case RendererAPI::OpenGL:
        Creator.reset(new GLVertexArrayCreator()); break;
      default:
        FS_INFO("No API!"); break;
    }
    Creator->addbuffer(handle, entity);
  }
  
  void VertexArrayCreator::Destory(VertexArrayGpuHandle &handle) {
  std::unique_ptr<VertexArrayCreator> Creator;
    switch (GetAPI()) {
      case RendererAPI::OpenGL:
        Creator.reset(new GLVertexArrayCreator()); break;
      default:
        FS_INFO("No API!"); break;
    }
    Creator->destory(handle);
  }

  void VertexArrayCreator::Bind(VertexArrayGpuHandle &handle) {
    std::unique_ptr<VertexArrayCreator> Creator;
    switch (GetAPI()) {
      case RendererAPI::OpenGL:
        Creator.reset(new GLVertexArrayCreator()); break;
      default:
        FS_INFO("No API!"); break;
    }
    Creator->bind(handle);
  }

  void VertexArrayCreator::SetIndex(VertexArrayGpuHandle &handle, uint32_t entity) {
    std::unique_ptr<VertexArrayCreator> Creator;
    switch (GetAPI()) {
      case RendererAPI::OpenGL:
        Creator.reset(new GLVertexArrayCreator()); break;
      default:
        FS_INFO("No API!"); break;
    }
    Creator->setIndex(handle, entity);
  }

  void VertexArrayCreator::Draw(uint32_t beginIndex, uint32_t count, int ElementType) {
    std::unique_ptr<VertexArrayCreator> Creator;
    switch (GetAPI()) {
      case RendererAPI::OpenGL:
        Creator.reset(new GLVertexArrayCreator()); break;
      default:
        FS_INFO("No API!"); break;
    }
    Creator->renderIndexBuffer(beginIndex, count, ElementType);
  }
}