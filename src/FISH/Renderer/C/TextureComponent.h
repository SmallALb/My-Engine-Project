#pragma once
//纹理组件
#include "FISH/Renderer/D/TextureData.h"
#include "FISH/Base/Ecs/Component.h"

namespace FISH{
  
  //基本纹理结构
  struct TextureBaseData : public Component {
    uint32_t width{0}, height{0};
    //类型
    ChannelType channel {ChannelType::RGBA};
    //纹理加载类型
    TextureLoadType type{TextureLoadType::TEXTURE2D};
    //路径
    TexturePath filePath;
    string fileName;
    
    string getComponentName() const override {return "TextureBaseData";}
  };

  //中间加载结构
  struct TextureLoadState : public Component {
    string getComponentName() const override {return "TextureLoadState";}

    TextureData Imagedata;
    std::function<void(uint32_t)> callback;
  };

}




