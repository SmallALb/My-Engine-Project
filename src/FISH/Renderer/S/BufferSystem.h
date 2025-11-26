#pragma once 

#include "FISH/System.h"
#include "FISH/Renderer/C/BufferComponent.h"

namespace FISH {
  //### BufferSystem
  // - only create the VertexBuffer or IndexBuffer
  class BufferSystem : public FISH_System {
  public:
    uint32_t createBuffer(const std::vector<BufferData>& vertices);
  
    void destoryBuffer(uint32_t entity, size_t id);
    
    void destoryEntity(uint32_t entity);
  private:
    void buildVertexBuffer(uint32_t entity, const BufferData& vertices, size_t id);

    void buildIndexBuffer(uint32_t entity, const BufferData& vertices, size_t id);

  };


}