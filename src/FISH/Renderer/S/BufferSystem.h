#pragma once 

#include "FISH/System.h"

namespace FISH {

  //### BufferSystem
  // - only create the VertexBuffer or IndexBuffer
  class BufferSystem : public FISH_System {
    BufferSystem() {}
  public:
    void OnDetach();

    uint32_t createBuffer(const BufferData& vertices);
      
    void destoryEntity(uint32_t entity);

    bool hasBufferEntity(uint32_t entity);

    BufferHandleI* getBufferGpuHandle(uint32_t entity);

    BufferType getBufferType(uint32_t entity) const;

    const std::vector<std::pair<InputDataType, bool>>& getBufferLayout(uint32_t entity) const;

    static BufferSystem* GetInstance();

  private:
    static BufferSystem* ptr;
  };


}