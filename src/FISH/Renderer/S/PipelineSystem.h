#pragma once

namespace FISH {
  class PipelineSystem : public FISH_System {
  public:
    void addShader(uint32_t entity, const std::vector<uint32_t>& Shaders);
    
    void addVertexArray(uint32_t entity, uint32_t arrayEntity);

    uint32_t cerate(cosnt std::vector<RenderCommand>& vectoor);
  };

}