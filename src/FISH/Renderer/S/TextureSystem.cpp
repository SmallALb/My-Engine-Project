#include "fspcs.h"
#include "FISH/System.h"
#include "FISH/Log.h"
#include "FISH/Renderer/RenderElement.h"
#include "../D/TextureData.h"
#include "../C/TextureComponent.h"
#include "TextureSystem.h"
#include "FISH/Renderer/GPU/TextureCreator.h"
//aother
#include "FISH/Renderer/stb_image.h"
#include "FISH/ImGui/ImGuiLayer.h"
#include "FISH/Debugger.h"

namespace FISH {  
  TextureSystem* TextureSystem::ptr{nullptr};

  TextureSystem::~TextureSystem() {
    FS_INFO("Try destruct TextureSystem");
  }

  void FISH::TextureSystem::OnAttach()
  {
    FS_PROFILE_THREAD("Texture Build");
  }
  void TextureSystem::OnImGuiRender() {
    
  }

  void TextureSystem::OnDetach() {
    FS_INFO("destruct TextureSystem");
    for (auto& [entity, path] : mEntityMap) destoryEntity(entity);
  }

  void TextureSystem::OnUpdate(float dt) {
    FS_PROFILE_FUNCTION();
    {

      FS_PROFILE_SCOPE("Onupdate");

      std::queue<uint32_t> Q;
      {
        //Gets all the already uploaded Texture
        std::lock_guard<std::mutex> lock(mUpLoadQueMutex);
        if (mUploadQue.empty()) return;
        std::swap(Q, mUploadQue);
      }
      //creates Gpu handle for every Texture
      while (!Q.empty()){
        std::function<void(uint32_t)> func;
        auto entity = Q.front(); Q.pop();
        FS_CORE_INFO("Upload Texture entity {}", entity);
        {
          
          auto& [w, h, c, t, fp, fn] = mRegistry.get<TextureBaseData>(entity);
          auto& [d, call] = mRegistry.get<TextureLoadState>(entity);
          mRegistry.add<TextureGpuHandle>(entity, std::move(TextureCreator::CreateHandle(d,w,h,c,t)));
          func = call;
        }
        if (func) func(entity);
        //erases loadState after a successful build;
        {
          
          auto& loadState = mRegistry.get<TextureLoadState>(entity);
          auto& [w, h, c, t, fp, fn] = mRegistry.get<TextureBaseData>(entity);
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
          
          mRegistry.erase<TextureLoadState>(entity);
        }
      }
    }
  }

  void TextureSystem::create(const TexturePath &path, TextureLoadType typ, const std::function<void(uint32_t)> &func) {
    FS_PROFILE_FUNCTION();

    uint32_t entity;    
    entity = mRegistry.create([](uint32_t entity, Registry& Reger) {
      if (!Reger.has_entity(entity))
      TextureCreator::DestoryHandle(Reger.get<TextureGpuHandle>(entity));
      FS_CORE_INFO("sueccesed to destory Texture entity: {}", entity);
    });
    
    {
      FS_PROFILE_SCOPE("get for every");
      //Gets filename
      string filename;
      {
        FS_PROFILE_SCOPE("Get Name");
        switch(typ) {
          case TextureLoadType::TEXTURE2D: {
            auto p = std::get<0>(path);
            size_t pos = p.find_last_of("/\\");
            filename = pos != std::string::npos ? p.substr(pos+1) : p;
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
      }
      FS_INFO("Current TextureSystem TextureName:{}", filename);
      mNameToEntity[filename] = entity;
      FS_CORE_INFO("Load Texture: {}", filename);
      {
        FS_PROFILE_SCOPE("Init Data");
                
        mRegistry.add<TextureBaseData>(entity);
        //Inits the attribute of the base Texture Data
        auto& attribution = mRegistry.get<TextureBaseData>(entity);
        attribution.type = typ;
        attribution.fileName = filename;
        attribution.filePath = path;

        //Inits the LoadState Data
        mRegistry.add<TextureLoadState>(entity);
        auto& state = mRegistry.get<TextureLoadState>(entity);
        state.callback = func;
      }
      {
        FS_PROFILE_SCOPE("push Que");
        mLoadQue.push(entity);
      }
        //submit
      submit();
      mAsyncCondition.notify_one();
    }
  }

  void TextureSystem::setBinding(uint32_t entity, uint32_t binding) {
    auto& handle =  mRegistry.get<TextureGpuHandle>(entity);
    handle.binding = binding;
  }

  uint32_t TextureSystem::getTextureEntity(string path) {
    return mNameToEntity[path];
  }

  TextureGpuHandle& TextureSystem::getTextureHandle(uint32_t entity) {
    auto& handle =  mRegistry.get<TextureGpuHandle>(entity);
    return handle;
  }


  void TextureSystem::destoryEntity(uint32_t entity) {
     
    if (!mEntityMap.contains(entity)) return;
    auto& handle = mRegistry.get<TextureGpuHandle>(entity);
    mRegistry.destory(entity);
    mNameToEntity.erase(mEntityMap[entity]);
    mEntityMap.erase(entity);
  }

  void TextureSystem::bindHandle(uint32_t entity) {
    if (mRegistry.has<TextureGpuHandle>(entity)) 
      TextureCreator::BindTexture(mRegistry.get<TextureGpuHandle>(entity));
  }

  void TextureSystem::AsyncUpdate() {
    FS_CORE_INFO("TextureSystem AsyncUpdate");
    //取出需要处理的实体
    uint32_t res; mLoadQue.pop(res);
    auto& entity = res;
    FS_CORE_INFO("Done! get Texture Load Task!");
    //上锁取信息
    TexturePath filepath;
    TextureLoadType typ;
    int w, h, c;
    {
      FS_CORE_INFO("Get Texture Input Data: {}", entity);
             
      auto& basedata = mRegistry.get<TextureBaseData>(entity);
      filepath = basedata.filePath;
      typ = basedata.type;
    }

    TextureData Data;
    //异步加载数据
    switch(typ) {
      case TextureLoadType::TEXTURE2D: {
        auto data = load2D(std::get<0>(filepath), w, h, c);
        if (!data) {
          FS_CORE_ERROR("Error Image Data Load!");
            
          mRegistry.destory(entity);
          return;
        }
        Data = data; break;
      }
      case TextureLoadType::TEXTURECUBE: {
        auto data = loadCube(std::get<1>(filepath), w, h, c);
        if (data.empty()) {
          FS_CORE_ERROR("Error Image Data Load!");
          
          mRegistry.destory(entity);
          return;
        }
        Data = std::move(data); break;
      }
    } 

    //上锁再次灌入数据
    {
      FS_CORE_INFO("Input TextureData In: {}", entity);
             
      auto& basedata = mRegistry.get<TextureBaseData>(entity);
      auto& loaddata = mRegistry.get<TextureLoadState>(entity);

      basedata.width = w; basedata.height = h; basedata.channel = StbChannelToEnumChannel(c);
      loaddata.Imagedata = std::move(Data);
    }

    //上传上载队列
    {
      FS_CORE_INFO("Upload Task In Que: {}", entity);
      std::lock_guard<std::mutex> lock(mUpLoadQueMutex);
      mUploadQue.push(entity);
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