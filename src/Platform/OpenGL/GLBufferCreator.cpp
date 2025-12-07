#include "fspcs.h"
#include "FISH/Renderer/D/BufferData.h"
#include "GLBufferCreator.h"
#include "FISH/Log.h"
#include "glad/glad.h"
#include "OpenGLErrorCheck.h"

namespace FISH {
  BufferGpuHandle GLBufferCreator::create(const BufferData &vertices) {
    BufferGpuHandle HANDLESTRUCT;
    HANDLESTRUCT.HANDLE = new GLBufferHandle();

    GLBufferHandle& GLhandle = *(GLBufferHandle*)HANDLESTRUCT.HANDLE;
    glCreateBuffers(1, &GLhandle.bindId);
    FS_INFO("Create Buffer: {}", GLhandle.bindId);
    switch(vertices.type) {
      case BufferType::VERTEX: createVertex(std::get<0>(vertices.data).first, GLhandle); break;
      case BufferType::INDEX: createIndex(std::get<1>(vertices.data), GLhandle); break;
    }
    return HANDLESTRUCT;
  }
   
  void GLBufferCreator::destory(BufferGpuHandle &handle) {
    if (!handle.HANDLE) return;
    GLBufferHandle& GLhandle = *(GLBufferHandle*)handle.HANDLE;
    if (GLhandle.bindId) {
      FS_INFO("Destory Buffer: {}", GLhandle.bindId);
      glDeleteBuffers(1, &GLhandle.bindId);
      GLhandle.bindId = 0;
      delete handle.HANDLE;
      handle.HANDLE = nullptr;
    }
  }

  void GLBufferCreator::createVertex(const std::vector<float> &vetices, GLBufferHandle &handle) {
    GL_ERRORCALL(glNamedBufferStorage(handle.bindId, vetices.size() * sizeof(float), vetices.data(), GL_DYNAMIC_STORAGE_BIT));
  }

  void GLBufferCreator::createIndex(const std::vector<uint32_t> &vetices, GLBufferHandle &handle) {
    GL_ERRORCALL(glNamedBufferStorage(handle.bindId, vetices.size() * sizeof(uint32_t), vetices.data(), GL_DYNAMIC_STORAGE_BIT));
  }
}