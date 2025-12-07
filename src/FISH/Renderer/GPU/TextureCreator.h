#pragma once

#include "FISH/Base/Ecs/Component.h"

namespace FISH {  

  //### Texture Gpu Handle
  //- the texture binding method was decided by API
  struct TextureGpuHandle : public Component {
    TextureHandleI* HANDLE;
    uint32_t binding{0};
    std::string getComponentName() const override {
      return "TextureGpuHandle";
    };
  };

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




  
}