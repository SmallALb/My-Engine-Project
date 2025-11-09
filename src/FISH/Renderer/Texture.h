#pragma once
#include "TextureHandle.h"
#include "FISH/Core.h"

namespace FISH {
    class FrameBuffer;
    using FrameBufferPtr = std::shared_ptr<FrameBuffer>;

    class Texture;
    using TexturePtr = std::shared_ptr<Texture>;

    //纹理接口
    class Texture : public TextureHandle {
        friend class TextureManager;
    public:
        enum class TextureType {
            Texture2D = 0,
            TextureCube
        };


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

        virtual TextureHandleType getType() const {return TextureHandleType::Static;}

        TextureType getTextureType() const {return texturetype;}

        //从内存中读取
        static TexturePtr CreateTextureFromMemory(
            const string& path,
             unsigned char* dataIN, 
             uint32_t WidthIn, uint32_t HeightIn,
             ChannelType channel = ChannelType::RGBA, 
             uint32_t unit = 0
            );
        

        //传入6张图的路径创建立方体贴图
        static TexturePtr CreateCubeTexture(const string& name, const std::array<std::vector<unsigned char>, 6> &paths, uint32_t widthIn, uint32_t heightIn, ChannelType channel = ChannelType::RGBA, uint32_t unit = 0);
        

        //从路径中读取
        static TexturePtr CreateTextureFromPath(const string& path, ChannelType channel = ChannelType::RGBA, uint32_t unit = 0);
        //创建空纹理
        static TexturePtr CreateNullTexture(
            const string& path, uint32_t WidthIn, uint32_t HeightIn,
             ChannelType channel = ChannelType::RGBA, 
             uint32_t unit = 0);

        static TexturePtr CreateFromFrame(const FrameBufferPtr& frame);
             
        static void initDefaultTexture();
    private:
        //纹理内存映射表
        static std::unordered_map<std::string, TexturePtr> TextureMap;
    public:
        //预处理纹理
        static TexturePtr NoneTexture;
    protected:
        //枚举纹理通道
        ChannelType             mEnumChannel;

        TextureType             texturetype{TextureType::Texture2D};
    };

}