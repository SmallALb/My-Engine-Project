#pragma once

#include "FISH/Base/Ecs/Component.h"

namespace FISH {
  struct ShaderGpuHandle: Component {
    ShaderHandleI* HANDLE;
    virtual std::string getComponentName() const {return "ShaderGpuHandle";};
  };
  
  
  class API_ ShaderCreator {
  public:
    static ShaderGpuHandle CreateShader(const string& contex, ShaderType type);

    static void Destory(ShaderGpuHandle& handle);
  protected:
    virtual ShaderGpuHandle create(const string& contex, ShaderType type) = 0;

    virtual void destory(ShaderGpuHandle& handle) = 0;
    
  };


}