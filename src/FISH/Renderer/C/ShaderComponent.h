#pragma once

namespace FISH {
  struct ShaderInfo : public Component {
    ShaderType TypeOfShader;
    std::string Name;    
    std::vector<ShaderDataDescribe> Describes;
    std::vector<UniformDescribe> UniformDescribes;
    std::vector<LayoutDescribe> LayoutDescribes;
    std::unordered_map<string, ShaderStructDescribe> ShaderStructs;
    bool isComplite{0};
    size_t Version{0};
    
    std::string getComponentName() const override {
      return "ShaderInfo";
    }

  };


}