#include "fspcs.h"
#include "FISH/Log.h"
#include "API.h"
#include "RenderElement.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Texture.h"

namespace FISH {
    std::unordered_map<std::string, TexturePtr> Texture::TextureMap{};
    TexturePtr Texture::NoneTexture = nullptr;

    TexturePtr Texture::CreateTextureFromMemory(const string& path, unsigned char* dataIN, 
            uint32_t WidthIn, uint32_t HeightIn, ChannelType channel, uint32_t unit) {
        switch(GetAPI()) {
            case RendererAPI::OpenGL: {
                if (!TextureMap.contains(path)) TextureMap[path].reset(new GLTexture(unit, channel, dataIN, WidthIn, HeightIn));
                TextureMap[path]->mPath = path;
                return TextureMap[path];
            }
        }    
        FS_CORE_ERROR("找不到对应的API");
        return nullptr;
    }

    TexturePtr Texture::CreateTextureFromPath(const string& path, ChannelType channel, uint32_t unit) {
            switch (GetAPI()) {
                case RendererAPI::OpenGL: {
                    if (!TextureMap.contains(path)) TextureMap[path].reset(new GLTexture(path, channel, unit));
                    TextureMap[path]->mPath = path;
                    return TextureMap[path];
                }
            }
            FS_CORE_ERROR("找不到对应的API");
            return nullptr;
    }

    TexturePtr Texture::CreateNullTexture(const string &path, uint32_t WidthIn, uint32_t HeightIn, ChannelType channel, uint32_t unit) {
        switch (GetAPI()) {
            case RendererAPI::OpenGL: {
                if (!TextureMap.contains(path)) TextureMap[path].reset(new GLTexture(path, WidthIn, HeightIn, channel, unit));
                TextureMap[path]->mPath = path;
                return TextureMap[path];
            }
        }
        FS_CORE_ERROR("找不到对应的API");
        return nullptr;
    }

    void Texture::initDefaultTexture() {
        NoneTexture = CreateTextureFromPath("picture/NoneTexture.jpg");
    }
}