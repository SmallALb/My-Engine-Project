#include "fspcs.h"
#include "FISH/Log.h"
#include "FISH/Renderer/RenderElement.h"
#include "TextureSystem.h"
#include "FISH/Renderer/GPU/TextureCreator.h"
#include "FISH/Renderer/stb_image.h"
#include "FISH/ImGui/ImGuiLayer.h"

namespace FISH {  
  TextureSystem* TextureSystem::ptr{nullptr};

  TextureSystem::~TextureSystem() {
    FS_INFO("destrutc TextureSystem");
    OnDetach();
  }

  void FISH::TextureSystem::OnAttach()
  {
  }
  void TextureSystem::OnImGuiRender() {
    
  }

  void TextureSystem::OnDetach() {
    std::lock_guard<std::mutex> lock(mRegistryMutex);
    for (auto [entity, name] : mEntityMap) {
      if (mRegistry.has<TextureGpuHandle>(entity)) 
        for (size_t i=0; i< mRegistry.size<TextureGpuHandle>(entity); i++) {
          auto& handle = mRegistry.get<TextureGpuHandle>(entity, i);
          TextureCreator::DestoryHandle(handle);
        }
    }
  }

  void TextureSystem::OnUpdate(float dt)
  {
    //此处灌入Gpu数据
    std::queue<std::pair<uint32_t, size_t>> Q;
    {
      std::lock_guard<std::mutex> lock(mUpLoadQueMutex);
      if (mUploadQue.empty()) return;
      std::swap(Q, mUploadQue);
    }
    //加载纹理
    while (!Q.empty()){

      std::function<void(uint32_t)> func;
      auto [entity, i] = Q.front(); Q.pop();
      FS_CORE_INFO("Upload Texture entity {}, index: {}", entity, i);
      {
        std::lock_guard<std::mutex> lock(mRegistryMutex);
        auto& [w, h, c, t, fp, fn] = mRegistry.get<TextureBaseData>(entity, i);
        auto& [d, call] = mRegistry.get<TextureLoadState>(entity, i);
        mRegistry.add<TextureGpuHandle>(entity, std::move(TextureCreator::CreateHandle(d,w,h,c,t)));
        func = call;
      }
      if (func) func(entity);
      //加载完函数删掉中间加载组件
      {
        std::lock_guard<std::mutex> lock(mRegistryMutex);
        auto& loadState = mRegistry.get<TextureLoadState>(entity, i);
        auto& [w, h, c, t, fp, fn] = mRegistry.get<TextureBaseData>(entity, i);
        switch(t) {
          case TextureLoadType::TEXTURE2D: {
            stbi_image_free(std::get<0>(loadState.Imagedata));
            break;
          }
          case TextureLoadType::TEXTURECUBE: {
            for (int i=0; i<6; i++) stbi_image_free(std::get<1>(loadState.Imagedata)[i]);
            break;
          }
        }
        
        mRegistry.erase<TextureLoadState>(entity, i);
      }
    }
  }

  void TextureSystem::create(const std::vector<TexturePath> &paths, TextureLoadType typ, const std::function<void(uint32_t)> &func) {
    auto entity = mRegistry.create();
    for (size_t i=0; i< paths.size(); i++) {
      auto& path = paths[i];
      //获取名字
      string filename;
      switch(typ) {
        case TextureLoadType::TEXTURE2D: {
          auto p = std::get<0>(path);
          filename = p.substr(p.find_last_of("/\\")+1);
          mEntityMap[entity] = filename;
          break;
        }
        case TextureLoadType::TEXTURECUBE: {
          auto p = std::get<1>(path);
          filename = p[0].substr(p[0].find_last_of("/\\")+1);
          mEntityMap[entity] = filename;
          break;
        }
      }
      mNameToEntity[filename] = entity;
      FS_CORE_INFO("Load Texture: {}", filename);
      //设置数据
      {
        std::lock_guard<std::mutex> lock(mRegistryMutex);      
        mRegistry.add<TextureBaseData>(entity);
        //设置属性组件
        auto& attribution = mRegistry.get<TextureBaseData>(entity, i);
        attribution.type = typ;
        attribution.fileName = filename;
        attribution.filePath = path;

        //设置中间数据组件
        mRegistry.add<TextureLoadState>(entity);
        auto& state = mRegistry.get<TextureLoadState>(entity, i);
        state.callback = func;
      }
      //上锁加入队列
      {
        std::lock_guard<std::mutex> lock(mLoadQueMutex);
        mLoadQue.push({entity, i});
      }
      //唤起系统全局任务池
      submit();
      mAsyncCondition.notify_one();
    }
  }
  

  uint32_t TextureSystem::getTextureEntity(string path)
  {
    return mNameToEntity[path];
  }

  TextureGpuHandle& TextureSystem::getTextureHandle(uint32_t entity, size_t index) {
    std::lock_guard<std::mutex> lock(mRegistryMutex);
    auto& handle =  mRegistry.get<TextureGpuHandle>(entity, index);
    return handle;
  }

  void TextureSystem::destoryTexture(uint32_t entity, size_t index) {
      std::lock_guard<std::mutex> lock(mRegistryMutex);
      auto& handle = mRegistry.get<TextureGpuHandle>(entity, index);
      TextureCreator::DestoryHandle(handle);
  }

  void TextureSystem::bindHandle(uint32_t entity, size_t index) {
    std::lock_guard<std::mutex> lock(mRegistryMutex);
    if (mRegistry.has<TextureGpuHandle>(entity)) TextureCreator::BindTexture(mRegistry.get<TextureGpuHandle>(entity, index));
  }

  void TextureSystem::AsyncUpdate() {
    FS_CORE_INFO("TextureSystem AsyncUpdate");
    //等待执行
    std::unique_lock<std::mutex> lock(mLoadQueMutex);
    mAsyncCondition.wait(lock, [this]() {return !mLoadQue.empty();});
    //取出需要处理的实体
    auto [entity, i] = mLoadQue.front(); mLoadQue.pop();
    FS_CORE_INFO("Done! get Texture Load Task!");
    //上锁取信息
    TexturePath filepath;
    TextureLoadType typ;
    int w, h, c;
    {
      FS_CORE_INFO("Get Texture Input Data: {}", entity);
      std::lock_guard<std::mutex> lock(mRegistryMutex);      
      auto& basedata = mRegistry.get<TextureBaseData>(entity, i);
      filepath = basedata.filePath;
      typ = basedata.type;
    }

    TextureData Data;
    //异步加载数据
    switch(typ) {
      case TextureLoadType::TEXTURE2D:
        {
          auto data = load2D(std::get<0>(filepath), w, h, c);
          if (!data) {
            FS_CORE_ERROR("Error Image Data Load!");
            std::lock_guard<std::mutex> lock(mRegistryMutex);
            mRegistry.destory(entity);
            return;
          }
          Data = data; break;
        }
      case TextureLoadType::TEXTURECUBE:
        {
          auto data = loadCube(std::get<1>(filepath), w, h, c);
          if (data.empty()) {
            FS_CORE_ERROR("Error Image Data Load!");
            std::lock_guard<std::mutex> lock(mRegistryMutex);
            mRegistry.destory(entity);
            return;
          }
          Data = std::move(data); break;
        }
    } 

    //上锁再次灌入数据
    {
      FS_CORE_INFO("Input TextureData In: {}", entity);
      std::lock_guard<std::mutex> lock(mRegistryMutex);      
      auto& basedata = mRegistry.get<TextureBaseData>(entity, i);
      auto& loaddata = mRegistry.get<TextureLoadState>(entity, i);

      basedata.width = w; basedata.height = h; basedata.channel = StbChannelToEnumChannel(c);
      loaddata.Imagedata = std::move(Data);
    }

    //上传上载队列
    {
      FS_CORE_INFO("Upload Task In Que: {}", entity);
      std::lock_guard<std::mutex> lock(mUpLoadQueMutex);
      mUploadQue.push({entity, i});
    }
  }

  TextureSystem *TextureSystem::GetInstance() {
    if (!ptr) ptr = new TextureSystem();
    return ptr;
  }

  uint8_t* TextureSystem::load2D(const string &path, int &w, int &h, int &c) {
    stbi_set_flip_vertically_on_load(true);
    FS_CORE_INFO("Get Memory From Texture Path: {}", path);
    auto sData = stbi_load(path.c_str(), &w, &h, &c, 0);
    if (!sData) {
      FS_CORE_ERROR("Get Memory From Texture Path Failed: {}", path);
      return {};
    }
    else {
      FS_CORE_INFO("Get Memory From Texture_{} width:{}, height:{}, channel:{}", path, w, h, c);

    }
    FS_CORE_INFO("Get Memory From Texture Path Done: {}", path);
    return sData;
  }

  std::array<uint8_t*, 6> TextureSystem::loadCube(const std::array<string, 6>& path, int& w, int& h, int& c) {
    std::array<uint8_t*, 6> Data;
    for (int i=0; i<6; i++) {
      auto data = stbi_load(path[i].c_str(), &w, &h, &c, 0);
      if (!data) {
          FS_CORE_ERROR("Failed to load Texture: {0}", path[i]);
          return {};
      }
    }
    return Data;
  }
}