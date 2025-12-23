#include "fspcs.h"
#include "FISH/Renderer/D/PipelineData.h"
#include "GLPipelineCreator.h"
#include "FISH/Log.h"
#include "glad/glad.h"
#include "OpenGLErrorCheck.h"

#include "FISH/Renderer/D/ShaderData.h"
#include "FISH/Renderer/C/ShaderComponent.h"
#include "FISH/Renderer/S/ShaderSystem.h"

#include "FISH/Base/SparseMap.h"
#include "GLShaderCreator.h"

namespace FISH {
  static GLenum ToGLCullMode(CullMode mode) {
    switch(mode) {
      case CullMode::NONE: return GL_NONE;
      case CullMode::FRONT: return GL_FRONT;
      case CullMode::BACK: return GL_BACK;
      case CullMode::FONT_AND_BACK: return GL_FRONT_AND_BACK;
    }
    return GL_NONE;

  }

  static GLenum ToPolygonMode(PolygonMode mode) {
    switch(mode) {
      case PolygonMode::FILL: return GL_FILL;
      case PolygonMode::LINE: return GL_LINE;
      case PolygonMode::POINT: return GL_POINT;
    }
    return GL_POINT;
  }

  static GLenum ToFrontFace(FrontFace mode) {
    switch (mode) {
      case FrontFace::CW: return GL_CW;
      case FrontFace::CCW: return GL_CCW;
    }
    return GL_CCW;
  }

  static GLenum ToCompareOp(CompareOp mode) {
    switch(mode) {
      case CompareOp::NEVER: return GL_NEVER;
      case CompareOp::ALWAYS: return GL_ALWAYS;
      case CompareOp::EQUAL: return GL_EQUAL;
      case CompareOp::GREATER: return GL_GREATER;
      case CompareOp::GREATER_OR_EQUAL: return GL_GEQUAL;
      case CompareOp::LESS: return GL_LESS;
      case CompareOp::LESS_OR_EQUAL : return GL_LEQUAL;
      case CompareOp::NOT_EQUAL: return GL_NOTEQUAL;
    }
    return GL_LEQUAL;
  }
  
  static GLenum ToBlendFactor(BlendFactor factor) {
    switch(factor) {
      case BlendFactor::ZERO: return GL_ZERO;
      case BlendFactor::ONE: return GL_ONE;
      case BlendFactor::SRC_COLOR: return GL_SRC1_COLOR;
      case BlendFactor::ONE_MINUS_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
      case BlendFactor::DST_COLOR: return GL_DST_COLOR;
      case BlendFactor::ONE_MINUS_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
      case BlendFactor::SRC_ALPHA: return GL_SRC_ALPHA;
      case BlendFactor::ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
      case BlendFactor::DST_ALPHA: return GL_DST_ALPHA;
      case BlendFactor::ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
      case BlendFactor::CONSTANT_COLOR: return GL_CONSTANT_COLOR;
      case BlendFactor::ONE_MINUS_CONSTANT_COLOR: return GL_ONE_MINUS_CONSTANT_COLOR;
      case BlendFactor::CONSTANT_ALPHA: return GL_CONSTANT_ALPHA;
      case BlendFactor::ONE_MINUS_CONSTANT_ALPHA: return GL_ONE_MINUS_CONSTANT_ALPHA;
      case BlendFactor::SRC_ALPHA_SATURATE: return GL_SRC_ALPHA_SATURATE;
    }
    return GL_ONE;
  }


    static GLenum ConvertBlendOp(BlendOp op) {
      switch (op) {
        case BlendOp::ADD: return GL_FUNC_ADD;
        case BlendOp::SUBTRACT: return GL_FUNC_SUBTRACT;
        case BlendOp::REVERSE_SUBTRACT: return GL_FUNC_REVERSE_SUBTRACT;
        case BlendOp::MIN: return GL_MIN;
        case BlendOp::MAX: return GL_MAX;
        default: return GL_FUNC_ADD;
      }
    }

    static GLenum ConvertStencilOp(StencilOp op) {
        switch (op) {
            case StencilOp::KEEP: return GL_KEEP;
            case StencilOp::ZERO: return GL_ZERO;
            case StencilOp::REPLACE: return GL_REPLACE;
            case StencilOp::INCREMENT_AND_CLAMP: return GL_INCR;
            case StencilOp::DECREMENT_AND_CLAMP: return GL_DECR;
            case StencilOp::INVERT: return GL_INVERT;
            case StencilOp::INCREMENT_AND_WRAP: return GL_INCR_WRAP;
            case StencilOp::DECREMENT_AND_WRAP: return GL_DECR_WRAP;
            default: return GL_KEEP;
        }
    }


  PipelineGpuHandle GLPipelineCreator::create(uint32_t Mask, const std::vector<PipelineRenderSettings> &Features) {
    PipelineGpuHandle handle;
    handle.HANDLE = new GLPipelineHandle();
    return handle;
  }


  void GLPipelineCreator::destory(PipelineGpuHandle &handle) {
    if (!handle.HANDLE) return;
    auto GLhandle = static_cast<GLPipelineHandle*>(handle.HANDLE);
    if (GLhandle->mShaderProgameID) {
      glDeleteProgram(GLhandle->mShaderProgameID);
      FS_CORE_INFO("Success To Destory Pipeline Program!");
      GLhandle->mShaderProgameID = 0;
    } 
  }

  void GLPipelineCreator::buildShaderProgram(PipelineGpuHandle & handle, std::span<uint32_t> view) {
    auto GLhandle = static_cast<GLPipelineHandle*>(handle.HANDLE);
    GLhandle->mShaderProgameID = glCreateProgram();
    auto& shadersystem = *ShaderSystem::GetInstance();
    
    for (auto shaderEntity : view) {
      glAttachShader(
        GLhandle->mShaderProgameID,
        static_cast<GLShaderHandle*>(shadersystem.getShaderGpuHandle(shaderEntity))->bindId
      );
    }
    glLinkProgram(GLhandle->mShaderProgameID);
    bool error_tag = checkShaderErrors(GLhandle->mShaderProgameID, "LINK");
    if (error_tag) {
      glDeleteProgram(GLhandle->mShaderProgameID);
      delete handle.HANDLE;
      handle.HANDLE = nullptr;
      FS_CORE_ERROR("failed To Make a Shader! ");
      return;
    }

    handle.alive = 1;
  }
}