#include "fspcs.h"
#include "FISH/Base/SparseMap.h"
#include "FISH/Renderer/D/ShaderData.h"
#include "GLShaderCreator.h"
#include "FISH/Log.h"
#include "glad/glad.h"
#include "OpenGLErrorCheck.h"

namespace FISH {
  static int getTypeToGLType(ShaderType type) {
    switch(type) {
      case ShaderType::VERTEX: return GL_VERTEX_SHADER;
      case ShaderType::FRAGMENT: return GL_FRAGMENT_SHADER;
      case ShaderType::TESSCONTROL: return GL_TESS_CONTROL_SHADER;
      case ShaderType::TESSEVALUATION: return GL_TESS_EVALUATION_SHADER;
      case ShaderType::COMPUTE: return GL_COMPUTE_SHADER;
    }
    return GL_VERTEX_SHADER;
  }

    bool checkShaderErrors(uint32_t target, std::string type) {
        int success = 0;
        char infoLog[1024];
    
        if (type == "COMPILE") {
          glGetShaderiv(target, GL_COMPILE_STATUS, &success);
          if (!success) {
            glGetShaderInfoLog(target, 1024, NULL, infoLog);
            //std::cout << "Error: SHADER COMPILE ERROR" << "\n" << infoLog << std::endl;
            FS_CORE_ERROR("Error: SHADER COMPILE ERROR: {0}",  infoLog);
            return true;
          }
        }
        else if (type == "LINK") {
          glGetProgramiv(target, GL_LINK_STATUS, &success);
          if (!success) {
            glGetProgramInfoLog(target, 1024, NULL, infoLog);
            //std::cout << "Error: SHADER LINK ERROR " << "\n" << infoLog << std::endl;
            FS_CORE_ERROR("Error: SHADER LINK ERROR: {0}",  infoLog);
            return true;
          }
        }
        else {
          //std::cout << "Error: Check shader errors Type is wrong" << std::endl;
          FS_CORE_ERROR("Error: Check shader errors Type is wrong");
          return true;
        }
        return false;
    }


  ShaderGpuHandle GLShaderCreator::create(const string &contex, ShaderType type) {
    ShaderGpuHandle HANDLESTRUCT;
    HANDLESTRUCT.HANDLE = new GLShaderHandle();
    auto& GLhandle = *static_cast<GLShaderHandle*>(HANDLESTRUCT.HANDLE);
    int glShaderType = getTypeToGLType(type);
    
    GLhandle.bindId = glCreateShader(glShaderType);

    const char* ctx = contex.c_str();
    glShaderSource(GLhandle.bindId, 1, &ctx, NULL);

    glCompileShader(GLhandle.bindId);
    
    bool isError = checkShaderErrors(GLhandle.bindId, "COMPILE");
    if (isError) {
      glDeleteShader(GLhandle.bindId);
      delete HANDLESTRUCT.HANDLE;
      HANDLESTRUCT.HANDLE = nullptr;
      FS_CORE_ERROR("failed To Make a Shader! ");
    }
    else {
      FS_CORE_INFO("successed To Make a Shader: bindID {}", GLhandle.bindId);
      HANDLESTRUCT.alive = 1;
    }
    
    return HANDLESTRUCT;
  }

  void GLShaderCreator::destory(ShaderGpuHandle &handle) {
    if (!handle.HANDLE) return;
    handle.alive = 0;
    auto& GLhandle = *static_cast<GLShaderHandle*>(handle.HANDLE);
    if (!GLhandle.bindId) return;
    glDeleteShader(GLhandle.bindId);
    delete handle.HANDLE;
    handle.HANDLE = nullptr;
  }
}