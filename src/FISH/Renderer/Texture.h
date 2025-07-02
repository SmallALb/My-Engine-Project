#pragma once
#include "TextureHandle.h"
#include "FISH/Core.h"

namespace FISH {
    //纹理接口
    class Texture : public TextureHandle {
    public:
        virtual ~Texture() = default;


        virtual bool isValid() const = 0;

        //销毁
        virtual void destory() = 0;

        //获得宽度
        virtual uint32_t getWidth() const = 0;

        //获得高度
        virtual uint32_t getHeight() const = 0;
        //更新纹理部分
        virtual void updateSubTex(int x, int y, int width, int height, unsigned char* data) = 0;

        //设置unit(Opengl)
        virtual void setUnit(uint32_t unit) {};

        //从内存中读取
        static Texture* CreateTextureFromMemory(
            const string& path,
             unsigned char* dataIN, 
             uint32_t WidthIn, uint32_t HeightIn,
             ChannelType channel = ChannelType::RGBA, 
             uint32_t unit = 0
            );
        
        //从路径中读取
        static Texture* CreateTextureFromPath(const string& path, ChannelType channel = ChannelType::RGBA, uint32_t unit = 0);
        //创建空纹理
        static Texture* CreateNullTexture(
            const string& path, uint32_t WidthIn, uint32_t HeightIn,
             ChannelType channel = ChannelType::RGBA, 
             uint32_t unit = 0);
             
        static void initDefaultTexture();
    private:
        //纹理内存映射表
        static std::unordered_map<std::string, Texture*> TextureMap;
    public:
        //预处理纹理
        static std::shared_ptr<Texture> NoneTexture;
    protected:
        //枚举纹理通道
        ChannelType             mEnumChannel;
            
    };
}