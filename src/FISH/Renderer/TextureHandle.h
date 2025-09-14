#pragma once
#include "RenderElement.h"
#include "FISH/Core.h"

namespace FISH {
    enum class TextureHandleType {
        None,
        Static,
        Dynamic
    };
    //像素信息
    struct PixelInfo {
        uint8_t r, g, b, a;
        PixelInfo(): r(0), g(0), b(0), a(0) {}
        PixelInfo(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_=255):
            r(r_), g(g_), b(b_), a(a_)
        {}
    };

    //纹理信息
    struct TextureInfo {
        uint32_t width, height;
        float aspectRatio;
        uint32_t pixelCount;
        ChannelType channelType;
        std::vector<PixelInfo> pixels;

        TextureInfo(): 
            width(0), height(0), aspectRatio(0.0f), 
            pixelCount(0), channelType(ChannelType::RGBA)
            {}
    };

    class API_  TextureHandle {
    public:
        virtual unsigned long long getHandle() const = 0;

        virtual unsigned long long getTextureID() const = 0;

        virtual TextureHandleType getType() const {return TextureHandleType::None;}

        virtual TextureInfo getTextureInfo() const = 0;

        inline string getPath() const {return mPath;}

    protected:
        string                  mPath;
    };


    using TextureHandlePtr = std::shared_ptr<TextureHandle>;
}