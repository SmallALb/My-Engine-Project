#include "fspcs.h"
#include "GLBufferCreator.h"
#include "FISH/Log.h"
#include "glad/glad.h"
#include "OpenGLErrorCheck.h"

namespace FISH {
  BufferGpuHandle GLBufferCreator::create(const BufferData &vertices) {
    BufferGpuHandle HANDLESTRUCT;
    glCreateBuffers(1, &HANDLESTRUCT.bindId);

    switch(vertices.type) {
      case BufferType::VERTEX: createVertex(std::get<0>(vertices.data).first, HANDLESTRUCT); break;
      case BufferType::INDEX: createIndex(std::get<1>(vertices.data), HANDLESTRUCT); break;
    }

    return HANDLESTRUCT;
  }
   
  void GLBufferCreator::destory(BufferGpuHandle &handle) {
    glDeleteBuffers(1, &handle.bindId);
  }

  void GLBufferCreator::createVertex(const std::vector<float> &vetices, BufferGpuHandle &handle) {
    GL_ERRORCALL(glNamedBufferStorage(handle.bindId, vetices.size() * sizeof(float), vetices.data(), GL_DYNAMIC_STORAGE_BIT));
  }

  void GLBufferCreator::createIndex(const std::vector<uint32_t> &vetices, BufferGpuHandle &handle) {
    GL_ERRORCALL(glNamedBufferStorage(handle.bindId, vetices.size() * sizeof(uint32_t), vetices.data(), GL_DYNAMIC_STORAGE_BIT));
  }
}