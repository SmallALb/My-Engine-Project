#include "fspcs.h"
#include "FISH/Log.h"
#include "API.h"
#include "RenderElement.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Texture.h"

namespace FISH {
    std::unordered_map<std::string, Texture*> Texture::TextureMap{};
    std::shared_ptr<Texture> Texture::NoneTexture = nullptr;

    Texture* Texture::CreateTextureFromMemory(const string& path, unsigned char* dataIN, 
            uint32_t WidthIn, uint32_t HeightIn, ChannelType channel, uint32_t unit) {
        switch(GetAPI()) {
            case RendererAPI::OpenGL:
                return TextureMap.contains(path) ? TextureMap[path] : TextureMap[path] = new GLTexture(unit, channel, dataIN, WidthIn, HeightIn);
        }    
        FS_CORE_ERROR("找不到对应的API");
        return nullptr;
    }

    Texture* Texture::CreateTextureFromPath(const string& path, ChannelType channel, uint32_t unit) {
            switch (GetAPI()) {
                case RendererAPI::OpenGL:
                    return TextureMap.contains(path) ? TextureMap[path] : TextureMap[path] = new GLTexture(path, channel, unit);
            }
            FS_CORE_ERROR("找不到对应的API");
            return nullptr;
    }

    Texture *Texture::CreateNullTexture(const string &path, uint32_t WidthIn, uint32_t HeightIn, ChannelType channel, uint32_t unit) {
        switch (GetAPI()) {
            case RendererAPI::OpenGL:
                return TextureMap.contains(path) ? TextureMap[path] : TextureMap[path] = new GLTexture(path, WidthIn, HeightIn, channel, unit);
        }
        FS_CORE_ERROR("找不到对应的API");
        return nullptr;
    }

    void Texture::initDefaultTexture() {
        NoneTexture.reset(CreateTextureFromPath("picture/NoneTexture.jpg"));
    }
}