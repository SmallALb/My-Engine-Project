#pragma once

namespace FISH {
  class PipelineSystem : public FISH_System {
  public:
    void setShader(uint32_t entity, const std::vector<uint32_t>& Shaders);
    
    void setVertexArray(uint32_t entity, uint32_t arrayEntity);
    
    uint32_t create();

    void buildUpShaderPrograme(uint32_t entity);
  
    void exctue(uint32_t entity);
  };

}