#include "fspcs.h"
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
        create2D(std::get<0>(Data), width, height, typ, HANDLESTRUCT); break;
      case TextureLoadType::TEXTURECUBE:
        createCube(std::get<1>(Data), width, height, typ, HANDLESTRUCT); break;
    }

    return HANDLESTRUCT;
  }

  void FISH::GLTextureCreator::destory(TextureGpuHandle& handle) {
    if (handle.textureId) {
      FS_CORE_INFO("Destory the Texture: {}", handle.textureId);
      glDeleteTextures(1, &handle.textureId);
      handle.textureId = 0;
    }
  }

  void GLTextureCreator::bind(TextureGpuHandle &handle) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, handle.textureId);
  }

  void GLTextureCreator::create2D(uint8_t* Data, uint32_t width, uint32_t height, ChannelType typ, TextureGpuHandle &Handle) {
    FS_CORE_INFO("Creating 2D texture: {}x{}", width, height);
    if (!Data) FS_CORE_WARN("Creating a empty Texture!");
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    int textureChannel = ChoiceChannel(typ);
    GLint internalFormat = ChoiceInternal(typ);
    
    GL_ERRORCALL(glCreateTextures(GL_TEXTURE_2D, 1, &Handle.textureId));
    GL_ERRORCALL(glTextureStorage2D(Handle.textureId, 1, internalFormat, width, height));

    // 检查是否分配成功
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        FS_CORE_ERROR("glTextureStorage2D failed with error: {}", error);
        glDeleteTextures(1, &Handle.textureId);
        Handle.textureId = 0;
        return;
    }   


    GL_ERRORCALL(glTextureSubImage2D(Handle.textureId, 0, 0, 0, width, height, 
                                    textureChannel, GL_UNSIGNED_BYTE, Data));

    // 检查上传是否成功
    error = glGetError();
    if (error != GL_NO_ERROR) {
        FS_CORE_ERROR("glTextureSubImage2D failed with error: {}", error);
        glDeleteTextures(1, &Handle.textureId);
        Handle.textureId = 0;
        return;
    }
    
    glTextureParameteri(Handle.textureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(Handle.textureId, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(Handle.textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(Handle.textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    GL_ERRORCALL(glGenerateTextureMipmap(Handle.textureId));
    
    FS_CORE_INFO("Pure DSA texture created: ID={}", Handle.textureId);
  }
  
  void GLTextureCreator::createCube(const std::array<uint8_t*, 6> &Data, uint32_t width, uint32_t height, ChannelType typ, TextureGpuHandle &Handle) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    //创建纹理
    GL_ERRORCALL(glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &Handle.textureId));
    
    for (int i=0; i<6; i++) if (!Data[i]) {
      GL_ERRORCALL(
        glTextureSubImage3D(Handle.textureId, 0, 0, 0, i, width, height, 1, ChoiceChannel(typ), GL_UNSIGNED_BYTE, Data[i])
      );
    }
    
    glTextureParameteri(Handle.textureId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(Handle.textureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(Handle.textureId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(Handle.textureId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(Handle.textureId, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
  }
}
