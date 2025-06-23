#pragma once
#include "FISH/Core.h"

namespace FISH {
    //纹理接口
    class API_ Texture {
    public:
        virtual ~Texture() = default;

        //绑定纹理
        virtual void bind() {};

        //销毁
        virtual void destory() = 0;

        //获得宽度
        virtual uint32_t getWidth() const = 0;

        //获得高度
        virtual uint32_t getHeight() const = 0;

        virtual void updateSubTex(int x, int y, int width, int height, void* data) = 0;

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

        static void initDefaultTexture();
    private:
        //纹理内存映射表
        static std::unordered_map<std::string, Texture*> TextureMap;
    public:
        //预处理纹理
        static std::shared_ptr<Texture> NoneTexture;
    };
}