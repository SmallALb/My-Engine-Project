#pragma once
#include "FISH/System.h"
#include "FISH/Renderer/C/TextureComponent.h"

namespace FISH {
  struct TextureGpuHandle;
  //纹理系统
  class TextureSystem : public FISH_System {
    TextureSystem() {};
  public:

    ~TextureSystem();
    
    void OnAttach() override;

    void OnImGuiRender() override;

    void OnDetach() override;

    //给Application类在主线程中运行的代码
    void OnUpdate(float dt) override;

    void create(const std::vector<TexturePath>& paths, TextureLoadType typ, const std::function<void(uint32_t)>& func);

    uint32_t getTextureEntity(string path);

    TextureGpuHandle& getTextureHandle(uint32_t entity, size_t index);

    void destoryTexture(uint32_t entity, size_t index);

    void bindHandle(uint32_t entity, size_t index);
    
    //单例
    static TextureSystem* GetInstance();
  private:
    void AsyncUpdate() override;

  private:
    uint8_t* load2D(const string& path, int& w, int& h, int& c);

    std::array<uint8_t*, 6> loadCube(const std::array<string, 6>& path, int& w, int& h, int& c);


  private:
    //路径-实体映射表
    std::unordered_map<string, uint32_t> mNameToEntity;
    sparse_map<uint32_t, string> mEntityMap;
    std::queue<std::pair<uint32_t, size_t>> mLoadQue, mUploadQue;
    mutable std::mutex mLoadQueMutex, mUpLoadQueMutex;
    std::condition_variable mAsyncCondition;


    static TextureSystem* ptr;

  };
}