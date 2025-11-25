#pragma once

#include "FISH/Base/Ecs/Component.h"
#include "FISH/Renderer/D/TextureData.h"

namespace FISH {
  //forward declaraction of TextureGpuHandle Component
  struct TextureGpuHandle;

  //### Texture Creator interface
  //- The detials of texture creatment was decided by API; 
  class API_ TextureCreator {
  public:
    static TextureGpuHandle CreateHandle(const TextureData& Data, uint32_t width, uint32_t height, ChannelType typ, TextureLoadType loadType);

    static void BindTexture(TextureGpuHandle& handle);

    static void DestoryHandle(TextureGpuHandle& handle);
  protected:
    virtual TextureGpuHandle create(const TextureData& Data, uint32_t width, uint32_t height, ChannelType typ, TextureLoadType loadType) = 0;

    virtual void destory(TextureGpuHandle& handle) = 0;

    virtual void bind(TextureGpuHandle& handle) = 0;

  };


  //### Texture Gpu Handle
  //- the texture binding method was decided by API
  struct TextureGpuHandle : public Component {
    union {
      struct {
        uint32_t textureId;
      };
      
    };
    std::string getComponentName() const override {
      return "TextureGpuHandle";
    };
  };

  
}