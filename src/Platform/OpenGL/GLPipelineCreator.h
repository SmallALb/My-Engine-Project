#include "FISH/Renderer/GPU/PipelineCreator.h"

namespace FISH {
  struct GLPipelineHandle : public PipelineHandleI {
    uint32_t mShaderProgameID{0};
    
    string getGpuHandleAPI() const {return "OpenGl Pipeline Handle";} 
  };

  class GLPipelineCreator : public PipelineCreator {
  public:
    virtual PipelineGpuHandle create(uint32_t Mask, const std::vector<PipelineRenderSettings> &Features) override;
    
    virtual void destory(PipelineGpuHandle& handle) override;

    virtual void buildShaderProgram(PipelineGpuHandle& handle, std::span<uint32_t> view) override;
  
  };

}