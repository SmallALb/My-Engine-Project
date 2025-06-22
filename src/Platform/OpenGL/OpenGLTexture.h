#pragma once
#include "FISH/Renderer/Texture.h"

namespace FISH {
    //Opengl的纹理贴图
    class GLTexture : public Texture {
    public:
        GLTexture(const string& path, uint32_t unit);

        GLTexture(uint32_t unit, unsigned char* dataIn, uint32_t widthIn, uint32_t heightIn);

        ~GLTexture() override;
        
        inline virtual uint32_t getWidth() const override {return mWidth;}

        inline virtual uint32_t getHeight() const override {return mHeight;}

        inline virtual void setUnit(uint32_t unit) override {mUnit = unit;}

        virtual void bind() override;

        virtual void destory() override;
    private:
        uint32_t    mTexture{0};
        int         mWidth{0};
        int         mHeight{0};
        uint32_t    mUnit{0};
    };

}