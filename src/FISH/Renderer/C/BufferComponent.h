#pragma once
#include "FISH/Base/Ecs/Component.h"
#include "../D/BufferData.h"

namespace FISH {
  struct VertexBuffer : public Component {
    string getComponentName() const override {return "VertexBuffer";}
    uint32_t size;
    uint32_t offset;
    InputDataType inputdataType;
  };

  struct IndexBuffer : public Component {
    string getComponentName() const override {return "IndexBuffer";}
    uint32_t count;
  };

}