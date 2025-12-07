#pragma once

#include "FISH/Renderer/Gpu/TextureCreator.h"

namespace FISH {
  struct GLTextureHandle : public TextureHandleI {
    uint32_t binId;
    string getGpuHandleAPI() const override {return "OpenGL Texture Handle";}
  };

  class GLTextureCreator : public TextureCreator {
  public:
    GLTextureCreator() {}

    virtual TextureGpuHandle create(const TextureData& Data, uint32_t width, uint32_t height, ChannelType typ, TextureLoadType loadType) override;

    virtual void destory(TextureGpuHandle& handle) override;

    void bind(TextureGpuHandle& handle) override;

  private:
    void create2D(uint8_t* Data, uint32_t width, uint32_t height, ChannelType typ, TextureGpuHandle& Handle);

    void createCube(const std::array<uint8_t*, 6>& Data, uint32_t width, uint32_t height, ChannelType typ, TextureGpuHandle& Handle);
    

  };

}