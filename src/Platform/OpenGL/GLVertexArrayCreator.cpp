#include "fspcs.h"
#include "FISH/Renderer/D/VertexArrayData.h"
#include "GLVertexArrayCreator.h"
#include "FISH/Log.h"
#include "glad/glad.h"
#include "OpenGLErrorCheck.h"

#include "FISH/Renderer/D/BufferData.h"
#include "FISH/Renderer/C/BufferComponent.h"
#include "FISH/System.h"
#include "FISH/Renderer/S/BufferSystem.h"
#include "GLBufferCreator.h"
namespace FISH {
  
  static size_t getTypeDataTypeSize(InputDataType typ) {
    switch (typ) {
      case InputDataType::Float:     return 4;
      case InputDataType::Float2:    return 4 * 2;
      case InputDataType::Float3:    return 4 * 3;
      case InputDataType::Float4:    return 4 * 4;
      case InputDataType::Mat3:      return 4 * 3 * 3;
      case InputDataType::Mat4:      return 4 * 4 * 4;
      case InputDataType::Int:       return 4;
      case InputDataType::Int2:      return 4 * 2;
      case InputDataType::Int3:      return 4 * 3;
      case InputDataType::Int4:      return 4 * 4;
      case InputDataType::Bool:      return 1;
      case InputDataType::None:      return 0;
      default:
          break;
    }
    return 0;
  }


  static size_t getTypeDataTypeCount(InputDataType typ) {
    switch (typ) {
      case InputDataType::Float:     return 1;
      case InputDataType::Float2:    return 2;
      case InputDataType::Float3:    return 3;
      case InputDataType::Float4:    return 4;
      case InputDataType::Mat3:      return 3 * 3;
      case InputDataType::Mat4:      return 4 * 4;
      case InputDataType::Int:       return 1;
      case InputDataType::Int2:      return 2;
      case InputDataType::Int3:      return 3;
      case InputDataType::Int4:      return 4;
      case InputDataType::Bool:      return 1;
      case InputDataType::None:      return 0;
      default:
          break;
    }
    return 0;
  }



  static GLenum getDataTypeToOpenGLBaseType(InputDataType type) {
    switch (type) {
        case InputDataType::Float:         return GL_FLOAT;
        case InputDataType::Float2:        return GL_FLOAT;
        case InputDataType::Float3:        return GL_FLOAT;
        case InputDataType::Float4:        return GL_FLOAT;
        case InputDataType::Int:           return GL_INT;
        case InputDataType::Int2:          return GL_INT;
        case InputDataType::Int3:          return GL_INT;
        case InputDataType::Int4:          return GL_INT;
        case InputDataType::Bool:          return GL_UNSIGNED_BYTE;
        case InputDataType::Mat3:          return GL_FLOAT;
        case InputDataType::Mat4:          return GL_FLOAT;
        default:
            FS_CORE_ERROR("Invalid InputDataType: {}", static_cast<int>(type));
            return GL_FLOAT; // 默认返回 GL_FLOAT
    }
    return 0;
  }


  VertexArrayGpuHandle GLVertexArrayCreator::create() {
    VertexArrayGpuHandle HANDLESTRUCT;
    HANDLESTRUCT.HANDLE = new GLVertexArrayHandle();
    auto& GLhandle = *(GLVertexArrayHandle*)HANDLESTRUCT.HANDLE;
    GL_ERRORCALL(glCreateVertexArrays(1, &GLhandle.bindId), HANDLESTRUCT.alive);
    HANDLESTRUCT.alive = !HANDLESTRUCT.alive;
    FS_INFO("Create bindid: {}, GLHandle: {}", GLhandle.bindId, (size_t)HANDLESTRUCT.HANDLE);
    return HANDLESTRUCT;
  }

  void GLVertexArrayCreator::addbuffer(VertexArrayGpuHandle &handle, uint32_t entity) {
        FS_CORE_INFO("[GLVertexArrayCreator::addbuffer] START - Adding buffer entity {} to VAO", entity);
    FS_CORE_INFO("[GLVertexArrayCreator::addbuffer] VAO handle address: {}, HANDLE pointer: {}", 
                 (void*)&handle, (void*)handle.HANDLE);

    auto& VBOSystem = *BufferSystem::GetInstance();
    if (!VBOSystem.hasBufferEntity(entity) || VBOSystem.getBufferType(entity) != BufferType::VERTEX) {
      FS_CORE_ERROR("Not exitence of buffer entity");
      return;
    } 

    if (!handle.HANDLE) {
      FS_CORE_ERROR("NULL Handle inputed in GLVertexArray");
      return;
    }

    auto vbo = VBOSystem.getBufferGpuHandle(entity);
    if (!vbo) {
      FS_INFO("NULL VBOHandle inputed in GLVertexArray!");
      return;
    }

    auto& GLVaohandle = *static_cast<GLVertexArrayHandle*>(handle.HANDLE);
    auto& GLVbohandle = *static_cast<GLBufferHandle*>(vbo);
    
    size_t stride = 0;
    for (const auto& [datatype, normalize] : VBOSystem.getBufferLayout(entity))
      stride += getTypeDataTypeSize(datatype);


    
    glVertexArrayVertexBuffer(GLVaohandle.bindId, handle.buffercount, GLVbohandle.bindId, 0, stride);

    
    size_t offset = 0;
    for (const auto& [datatype, normalize] : VBOSystem.getBufferLayout(entity)) {
      glVertexArrayAttribBinding(GLVaohandle.bindId, handle.attributecount, handle.buffercount);
      
      glVertexArrayAttribFormat(GLVaohandle.bindId, handle.attributecount, 
        getTypeDataTypeCount(datatype), getDataTypeToOpenGLBaseType(datatype), 
        (normalize ? GL_TRUE : GL_FALSE), offset);
      
      glEnableVertexArrayAttrib(GLVaohandle.bindId, handle.attributecount);
      
      offset += getTypeDataTypeSize(datatype);
      handle.attributecount++;
    }

    handle.buffercount++;
  }
  
  void GLVertexArrayCreator::destory(VertexArrayGpuHandle &handle) {
    if (!handle.HANDLE) return;
    handle.alive = 0;
    auto& GLhandle = *(GLVertexArrayHandle*)handle.HANDLE;
    if (GLhandle.bindId) {
      glDeleteVertexArrays(1, &GLhandle.bindId);
      FS_CORE_INFO("Destory VertexArray Handle: {}", GLhandle.bindId);
      GLhandle.bindId = 0;
      delete handle.HANDLE;
      handle.HANDLE = nullptr;
    }
  }
  
  void GLVertexArrayCreator::bind(VertexArrayGpuHandle &handle) {
    auto& GLhandle = *(GLVertexArrayHandle*)handle.HANDLE;
    bool error_tag = 0;
    GL_ERRORCALL(glBindVertexArray(GLhandle.bindId), error_tag);
  }

  void GLVertexArrayCreator::renderIndexBuffer(uint32_t beginIndex, uint32_t count, int ElementType) {
    bool error_tag = 0;
    GL_ERRORCALL(glDrawElements(ElementType, count, GL_UNSIGNED_INT, (const void*)beginIndex), error_tag);
  }

  void GLVertexArrayCreator::setIndex(VertexArrayGpuHandle &handle, uint32_t entity) {
    auto& GLhandle = *(GLVertexArrayHandle*)handle.HANDLE;
    auto& VBOSystem = *BufferSystem::GetInstance();
    auto& GLVbohandle = *static_cast<GLBufferHandle*>(VBOSystem.getBufferGpuHandle(entity));
    glVertexArrayElementBuffer(GLhandle.bindId, GLVbohandle.bindId);
  }
}