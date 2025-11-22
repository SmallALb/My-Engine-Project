#pragma once

//纹理加载类型
namespace FISH {
  
  enum class TextureLoadType {
    TEXTURE2D,
    TEXTURECUBE
  };
  //纹理路径
  using TexturePath = std::variant<string, std::array<string, 6>>;
  //纹理在内存中的数据
  using TextureData = std::variant<uint8_t*, std::array<uint8_t*, 6>>;


}