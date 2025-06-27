#pragma once
#include "FISH/Renderer/Texture.h"

namespace FISH {
    //Opengl的纹理贴图
    class GLTexture : public Texture {
    public:
        GLTexture(const string& path, ChannelType channel, uint32_t unit);

        GLTexture(uint32_t unit, ChannelType channel, unsigned char* dataIn, uint32_t widthIn, uint32_t heightIn);

        GLTexture(const string& path, uint32_t widthIn, int32_t heightIn, ChannelType channel, uint32_t unit);

        ~GLTexture() override;
        
        inline virtual uint32_t getWidth() const override {return mWidth;}

        inline virtual uint32_t getHeight() const override {return mHeight;}

        inline virtual void setUnit(uint32_t unit) override {mUnit = unit;}

        inline virtual unsigned long long getHandle() const;

        virtual void updateSubTex(int x, int y, int width, int height, unsigned char* data);

        virtual bool isValid() const override { return !mTexture; }

        virtual void destory() override;
    private:
        //选择创建GL纹理的通道
        int ChoiceChannel(ChannelType channel);
        //根据通道选择写入类型
        int ChoiceInternal(ChannelType channel);
        //创建无绑定纹理的句柄
        void CreateBindLessHandle();
        //计算数据大小
        size_t accumulateDataSize(int width, int height) const;
    private:
        uint32_t                mTexture{0};
        unsigned long long      mTextureHandle{0};
        int                     mWidth{0};
        int                     mHeight{0};
        uint32_t                mUnit{0};
        int                     mTextureChannel{0};
    };

}