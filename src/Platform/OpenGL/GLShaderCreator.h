#pragma once

#include "FISH/Renderer/GPU/ShaderCreator.h"

namespace FISH {
  struct GLShaderHandle : public ShaderHandleI {
    uint32_t bindId;
    virtual string getGpuHandleAPI() const { return "OpenGL Shader Handle!"; }
  };


  class GLShaderCreator : public ShaderCreator {
  public:
    ShaderGpuHandle create(const string& contex, ShaderType type) override;

    void destory(ShaderGpuHandle& handle) override;
    
  };

}