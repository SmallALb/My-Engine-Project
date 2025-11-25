#pragma once


namespace FISH {
  //### Texture Load Type  
  enum class TextureLoadType {
    TEXTURE2D,
    TEXTURECUBE
  };
  //### Texture Path
  //- If texture load type is TEXTURE2D, the path of texture will be a string
  //- If texture load type is TEXTURECUBE, the path of texture will be 6 strings for the paths 
  //- The cube Texture paths input order must be right, left, top, bottom, front and back 
  using TexturePath = std::variant<string, std::array<string, 6>>;
  //### Texture Data In Memory
  //- The layout same as the Texture Path, but the stored type is byte(unsigned char) pointer
  using TextureData = std::variant<uint8_t*, std::array<uint8_t*, 6>>;


}