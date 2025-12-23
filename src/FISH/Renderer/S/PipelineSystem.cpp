#include "fspcs.h"
#include "FISH/System.h"
#include "../D/PipelineData.h"
#include "../C/PipelineComponent.h"
#include "PipelineSystem.h"

#include "../D/ShaderData.h"
#include "../C/ShaderComponent.h"
#include "ShaderSystem.h"

namespace FISH {
  void PipelineSystem::setShader(uint32_t entity, const std::vector<uint32_t> &Shaders) {
    
  }

  void PipelineSystem::setVertexArray(uint32_t entity, uint32_t arrayEntity) {
    
  }

  uint32_t PipelineSystem::create() {
    uint32_t entity = mRegistry.create();
    mRegistry.add<PipelineInfo>(entity);
        
    return entity;
  }

  void PipelineSystem::exctue(uint32_t entity) {
    
  }

  void PipelineSystem::buildUpShaderPrograme(uint32_t entity) {
  
  }

}