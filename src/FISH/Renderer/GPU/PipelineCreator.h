#pragma once

#include "FISH/Base/Ecs/Component.h"

namespace FISH {
  struct PipelineGpuHandle : public Component {
    PipelineHandleI* HANDLE;   
    string getComponentName() const override {return "PipelineGpuHandle";}
  };

  class API_ PipelineCreator {
  public:
    static PipelineGpuHandle Create(uint32_t Mask, const std::vector<PipelineRenderSettings>& Features);

    static void Destory(PipelineGpuHandle& handle);
  
    static void BuildShaderProgram(PipelineGpuHandle& handle, std::span<uint32_t> view);
  protected:
    virtual PipelineGpuHandle create(uint32_t Mask, const std::vector<PipelineRenderSettings>& Features) = 0;
    
    virtual void destory(PipelineGpuHandle& handle) = 0;

    virtual void buildShaderProgram(PipelineGpuHandle& handle, std::span<uint32_t> view) = 0;
  
  };

}