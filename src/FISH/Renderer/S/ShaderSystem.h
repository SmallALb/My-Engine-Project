#pragma once


namespace FISH {

  class ShaderSystem : public FISH_System {
    ShaderSystem() {}
  public:
    static ShaderSystem* GetInstance();
    
    uint32_t create(const string& path, ShaderType type);
    
    bool buildShader(uint32_t entity, uint32_t FileEntity);
  private:
    static ShaderSystem* ptr;
    uint32_t mBeginEntity{0};
  };


}