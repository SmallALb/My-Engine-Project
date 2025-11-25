#include "fspcs.h"
#include "GLBufferCreator.h"
#include "FISH/Log.h"
#include "glad/glad.h"
#include "OpenGLErrorCheck.h"

namespace FISH {
  BufferGpuHandle GLBufferCreator::create(const std::vector<float> &vertices, BufferType bufferType) {
    BufferGpuHandle HANDLESTRUCT;
    glCreateBuffers(1, &HANDLESTRUCT.bindId);

    switch(bufferType) {
      case BufferType::VERTEX: createVertex(vertices, HANDLESTRUCT); break;
      case BufferType::INDEX: createIndex(vertices, HANDLESTRUCT); break;
    }
  }
   
  void GLBufferCreator::destory(BufferGpuHandle &handle) {
  }

  void GLBufferCreator::bind(BufferGpuHandle &handle) {

  }

  void GLBufferCreator::createVertex(const std::vector<float> &vetices, BufferGpuHandle &handle) {
    GL_ERRORCALL(glNamedBufferStorage(handle.bindId, vetices.size() * sizeof(float), vetices.data(), GL_DYNAMIC_STORAGE_BIT));
  }

  void GLBufferCreator::createIndex(const std::vector<float> &vetices, BufferGpuHandle &handle) {
    GL_ERRORCALL(glNamedBufferStorage(handle.bindId, vetices.size() * sizeof(uint32_t), vetices.data(), GL_DYNAMIC_STORAGE_BIT));
  }
}