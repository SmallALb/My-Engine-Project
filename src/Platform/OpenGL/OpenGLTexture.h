#pragma once
#include "FISH/Renderer/Texture.h"

namespace FISH {
    //Opengl的纹理贴图
    class GLTexture : public Texture {
    public:
        GLTexture(const string& path, ChannelType channel, uint32_t unit);

        GLTexture(uint32_t unit, ChannelType channel, unsigned char* dataIn, uint32_t widthIn, uint32_t heightIn);

        ~GLTexture() override;
        
        inline virtual uint32_t getWidth() const override {return mWidth;}

        inline virtual uint32_t getHeight() const override {return mHeight;}

        inline virtual void setUnit(uint32_t unit) override {mUnit = unit;}

        virtual void updateSubTex(int x, int y, int width, int height, void* data);

        virtual void bind() override;

        virtual void destory() override;
    private:
        //选择创建GL纹理的通道
        int ChoiceChannel(ChannelType channel);
    private:
        uint32_t    mTexture{0};
        int         mWidth{0};
        int         mHeight{0};
        uint32_t    mUnit{0};
        int         mTextureChannel{0};
    };

}