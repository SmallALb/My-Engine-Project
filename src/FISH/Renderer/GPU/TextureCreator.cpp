#include "fspcs.h"
#include "FISH/Renderer/RenderElement.h"
#include "../D/TextureData.h"
#include "TextureCreator.h"
#include "FISH/Log.h"
#include "Platform/OpenGL/GLTextureCreator.h"
#include "FISH/Renderer/API.h"

namespace FISH {

  TextureGpuHandle TextureCreator::CreateHandle(const TextureData & Data, uint32_t width, uint32_t height, ChannelType typ, TextureLoadType loadType) {
    std::unique_ptr<TextureCreator> Creator;
    switch (GetAPI()) {
      case RendererAPI::OpenGL:
        Creator.reset(new GLTextureCreator()); break;
      default: 
        FS_INFO("No API!"); return TextureGpuHandle();
    }

    return Creator->create(Data, width, height, typ, loadType);
  }

  void TextureCreator::BindTexture(TextureGpuHandle & handle) {
    std::unique_ptr<TextureCreator> Creator;
    switch (GetAPI()) {
      case RendererAPI::OpenGL:
        Creator.reset(new GLTextureCreator()); break;
      default: 
        FS_INFO("No API!"); break;

    }
    Creator->bind(handle);
  }

  void TextureCreator::DestoryHandle(TextureGpuHandle & handle) {
    std::unique_ptr<TextureCreator> Creator;
    switch (GetAPI()) {
      case RendererAPI::OpenGL:
        Creator.reset(new GLTextureCreator()); break;
      default:
        FS_INFO("No API!"); break;
    }
    Creator->destory(handle);
  }
}