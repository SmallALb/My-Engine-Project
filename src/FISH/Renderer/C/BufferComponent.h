#pragma once
#include "FISH/Base/Ecs/Component.h"
#include "../D/BufferData.h"

namespace FISH {
  struct BufferSize : public Component {
    uint32_t stride{0};
    string getComponentName() const override {return "BufferSize";}
  };

  struct VertexBuffer : public Component {
    string getComponentName() const override {return "VertexBuffer";}
    uint32_t offset{0};
    InputDataType inputdataType;
  };

  struct IndexBuffer : public Component {
    string getComponentName() const override {return "IndexBuffer";}
    uint32_t count;
  };

}