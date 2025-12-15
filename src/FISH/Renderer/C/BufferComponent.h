#pragma once


namespace FISH {
  struct BufferTypeC : public Component {
    BufferTypeC() {}
    BufferTypeC(BufferType t): Type(t) {}

    BufferType Type {BufferType::VERTEX};
    std::string getComponentName() const override {
      return "BufferType";
    }
  };

  struct BufferLayoutC : public Component {
    std::vector<BufferData::Vlayout> TypeLayout;
    
    std::string getComponentName() const override {
      return "BufferLayout";
    }
  };
    
}