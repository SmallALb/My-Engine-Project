#pragma once
#include "FISH/System.h"
#include "FISH/Base/LockFreeQue.h"

namespace FISH {
  struct TextureGpuHandle;
  //## TextureSystem
  //- One Texture entity can map multi TextureHandle;
  class TextureSystem final : public FISH_System {
    TextureSystem() {};
  public:

    virtual ~TextureSystem() override;
    
    virtual void OnAttach() override;

    virtual void OnImGuiRender() override;

    virtual void OnDetach() override;

    virtual void OnUpdate(float dt) override;

    void create(const TexturePath& paths, TextureLoadType typ, const std::function<void(uint32_t)>& func);

    void setBinding(uint32_t entity, uint32_t binding);

    uint32_t getTextureEntity(string path);

    TextureGpuHandle& getTextureHandle(uint32_t entity);

    void destoryEntity(uint32_t entity);

    void bindHandle(uint32_t entity);
    
    //单例
    static TextureSystem* GetInstance();
  private:
    void AsyncUpdate() override;

  private:
    uint8_t* load2D(const string& path, int& w, int& h, int& c);

    std::array<uint8_t*, 6> loadCube(const std::array<string, 6>& path, int& w, int& h, int& c);


  private:
    //Name to Entity
    std::unordered_map<string, uint32_t> mNameToEntity;
    //Entity to TextureName
    sparse_map<uint32_t, string> mEntityMap;
    std::queue<uint32_t> mUploadQue;
    LockFreeQue<uint32_t> mLoadQue;
    mutable std::mutex mUpLoadQueMutex;
    std::condition_variable mAsyncCondition;

    static TextureSystem* ptr;

  };
}