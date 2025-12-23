#include "fspcs.h"
#include "FISH/Renderer/D/TextureData.h"
#include "FISH/Renderer/RenderElement.h"
#include "GLTextureCreator.h"
#include "FISH/Log.h"
#include "glad/glad.h"
#include "OpenGLErrorCheck.h"

namespace FISH {
  //SubImage
  int ChoiceChannel(ChannelType channel) {
    switch(channel) {
      case ChannelType::Red:  return GL_RED;
      case ChannelType::RGB:  return GL_RGB;
      case ChannelType::RGBA: return GL_RGBA;
    }
    return GL_NONE;
  }
  //Storage
  int ChoiceInternal(ChannelType channel) {
    switch(channel) {
      case ChannelType::Red: return GL_R8;
      case ChannelType::RGB: return GL_RGB8;
      case ChannelType::RGBA: return GL_RGBA8;
    }

    return GL_RGBA8;
  }

  int getBytesPrePixel(int channel) {
    int bytesPerPixel = 0;
    switch (channel) {
        case GL_RED: return 1; break;
        case GL_RGB: return 3; break;
        case GL_RGBA: return 4; break;
    }
    return 1;
  }



  TextureGpuHandle GLTextureCreator::create(const TextureData& Data, uint32_t width, uint32_t height, ChannelType typ, TextureLoadType loadType) {
    FS_CORE_INFO("Create GLTexture: {} x {}", width, height);

    TextureGpuHandle HANDLESTRUCT;
    GLint maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

    FS_CORE_INFO("Max Texture Size: {}", maxTextureSize);
  
    switch (loadType) {
      case TextureLoadType::TEXTURE2D:
        HANDLESTRUCT.alive = create2D(std::get<0>(Data), width, height, typ, HANDLESTRUCT); break;
      case TextureLoadType::TEXTURECUBE:
        HANDLESTRUCT.alive = createCube(std::get<1>(Data), width, height, typ, HANDLESTRUCT); break;
    }

    return HANDLESTRUCT;
  }

  void FISH::GLTextureCreator::destory(TextureGpuHandle& handle) {
    if (!handle.HANDLE) return;
    handle.alive = 0;
    auto& GLhandle = *((GLTextureHandle*)handle.HANDLE);
    if (GLhandle.binId) {
      FS_CORE_INFO("Destory the Texture: {}", GLhandle.binId);
      glDeleteTextures(1, &GLhandle.binId);
      GLhandle.binId = 0;
      delete handle.HANDLE;
      handle.HANDLE = nullptr;
    }
  }

  void GLTextureCreator::bind(TextureGpuHandle &handle) {
    if (!handle.HANDLE) return;
    glActiveTexture(GL_TEXTURE0 + handle.binding);
    glBindTexture(GL_TEXTURE_2D, ((GLTextureHandle*)handle.HANDLE)->binId);
  }

  bool GLTextureCreator::create2D(uint8_t* Data, uint32_t width, uint32_t height, ChannelType typ, TextureGpuHandle &Handle) {
    FS_CORE_INFO("Creating 2D texture: {}x{}", width, height);
    if (!Data) 
      FS_CORE_WARN("Creating a empty Texture!");
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    int textureChannel = ChoiceChannel(typ);
    GLint internalFormat = ChoiceInternal(typ);

    Handle.HANDLE = new GLTextureHandle();
    auto& GLhandle = *((GLTextureHandle*)Handle.HANDLE);
    bool error_tag = 0;
    GL_ERRORCALL(glCreateTextures(GL_TEXTURE_2D, 1, &GLhandle.binId), error_tag);
    GL_ERRORCALL(glTextureStorage2D(GLhandle.binId, 1, internalFormat, width, height), error_tag);

    // 检查是否分配成功
    if (error_tag) {
        glDeleteTextures(1, &GLhandle.binId);
        GLhandle.binId = 0;
        return 0;
    }   


    GL_ERRORCALL(glTextureSubImage2D(GLhandle.binId, 0, 0, 0, width, height, 
                                    textureChannel, GL_UNSIGNED_BYTE, Data), error_tag);
    if (error_tag) {
        glDeleteTextures(1, &GLhandle.binId);
        GLhandle.binId = 0;
        return 0;
    }

    glTextureParameteri(GLhandle.binId, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(GLhandle.binId, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(GLhandle.binId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(GLhandle.binId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    GL_ERRORCALL(glGenerateTextureMipmap(GLhandle.binId), error_tag);
    
    if (error_tag) {
        glDeleteTextures(1, &GLhandle.binId);
        GLhandle.binId = 0;
        return 0;
    }

    FS_CORE_INFO("Texture created: ID={}", GLhandle.binId);
    return 1;
  }
  
  bool GLTextureCreator::createCube(const std::array<uint8_t*, 6> &Data, uint32_t width, uint32_t height, ChannelType typ, TextureGpuHandle &Handle) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    FS_CORE_INFO("Creating Cube texture: {}x{}", width, height);
    bool error_tag = 0;
    Handle.HANDLE = new GLTextureHandle();
    auto& GLhandle = *((GLTextureHandle*)Handle.HANDLE);
    
    //创建纹理
    GL_ERRORCALL(glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &GLhandle.binId), error_tag);
    
    if (error_tag) {
        glDeleteTextures(1, &GLhandle.binId);
        GLhandle.binId = 0;
        return 0;
    }

    for (int i=0; i<6; i++) if (!Data[i]) {
      GL_ERRORCALL(
        glTextureSubImage3D(GLhandle.binId, 0, 0, 0, i, width, height, 1, ChoiceChannel(typ), GL_UNSIGNED_BYTE, Data[i])
        , error_tag
      );

      if (error_tag) {
        glDeleteTextures(1, &GLhandle.binId);
        GLhandle.binId = 0;
        return 0;
      }
    }
    
    glTextureParameteri(GLhandle.binId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(GLhandle.binId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(GLhandle.binId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(GLhandle.binId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(GLhandle.binId, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    FS_CORE_INFO("Texture created: ID={}", GLhandle.binId);

    return 1;
  }
}
