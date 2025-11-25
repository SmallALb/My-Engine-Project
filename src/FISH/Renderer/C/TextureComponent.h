#pragma once

#include "FISH/Renderer/D/TextureData.h"
#include "FISH/Base/Ecs/Component.h"

namespace FISH{
  
  //### Base Texture Data Component
  struct TextureBaseData : public Component {
    uint32_t width{0}, height{0};
    ChannelType channel {ChannelType::RGBA};
    TextureLoadType type{TextureLoadType::TEXTURE2D};
    TexturePath filePath;
    string fileName;
    
    string getComponentName() const override {return "TextureBaseData";}
  };

  //### Async Update Load State Component
  //- When asynchronously Update will read the image data in memory, then send it to the next update
  struct TextureLoadState : public Component {
    string getComponentName() const override {return "TextureLoadState";}
    TextureData Imagedata;
    std::function<void(uint32_t)> callback;
  };

}




