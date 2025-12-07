#pragma once

#include "FISH/Base/Ecs/Component.h"

namespace FISH {
  struct VertexArrayC : public Component {
    std::vector<uint32_t> bufferEntitys;
    uint32_t IndexEntity;
    std::string getComponentName() const override {return "VertexArray";};
  };

}
