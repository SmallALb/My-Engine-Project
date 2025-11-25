#pragma once 

#include "FISH/System.h"
#include "FISH/Renderer/C/BufferComponent.h"

namespace FISH {
  //### BufferSystem
  class BufferSystem : public FISH_System {
  public:
    void OnAttach() override;

    void OnUpdate(float dt) override;

    void OnDetach() override;

    void AsyncUpdate() override;

    uint32_t createFromVertices(const std::vector<float>& vertices, BufferType type);

  };


}