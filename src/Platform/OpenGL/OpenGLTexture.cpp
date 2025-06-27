#include "fspcs.h"
#include "FISH/Log.h"
#include "Platform/OpenGL/glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "FISH/Renderer/stb_image.h"
#include "OpenGLErrorCheck.h"
#include "FISH/Renderer/RenderElement.h"
#include "OpenGLTexture.h"

namespace FISH {
    GLTexture::GLTexture(const string &path, ChannelType channel, uint32_t unit) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        stbi_set_flip_vertically_on_load(1);
        int channels;

        mUnit = unit;
        unsigned char* data = stbi_load(path.c_str(), &mWidth, &mHeight,  &channels, choiceStbChannel(channel));
        mEnumChannel = channel;
        mTextureChannel = ChoiceChannel(channel);
        GLint internalFormat = ChoiceInternal(channel);
        // 使用DSA函数创建纹理
        GL_ERRORCALL(glCreateTextures(GL_TEXTURE_2D, 1, &mTexture));
        
        // 分配存储并设置参数
        GL_ERRORCALL(glTextureStorage2D(mTexture, 1, internalFormat, mWidth, mHeight));
        GL_ERRORCALL(glTextureSubImage2D(mTexture, 0, 0, 0, mWidth, mHeight, 
                                        mTextureChannel, GL_UNSIGNED_BYTE, data));

        stbi_image_free(data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        GL_ERRORCALL(glGenerateMipmap(GL_TEXTURE_2D));

        CreateBindLessHandle();

    }

    GLTexture::GLTexture(uint32_t unit, ChannelType channel, unsigned char *dataIn, uint32_t widthIn, uint32_t heightIn) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        stbi_set_flip_vertically_on_load(true);

        mUnit = unit;

        int channels;
        //枚举通道
        mEnumChannel = channel;
        //选择对应的GL类型
        mTextureChannel = ChoiceChannel(channel);
        int internalFormat = ChoiceInternal(channel);
        //计算size
        size_t dataInsize = 0;
        if (!heightIn) dataInsize = widthIn;
        else dataInsize = accumulateDataSize(widthIn, heightIn);
        unsigned char* data;
        if (channel != ChannelType::Red) {
            data =  stbi_load_from_memory(dataIn, dataInsize, &mWidth, &mHeight, &channels, choiceStbChannel(channel));
        }
        else {
            mWidth = widthIn, mHeight = heightIn;
            data = dataIn;
        }



        GL_ERRORCALL(glCreateTextures(GL_TEXTURE_2D, 1, &mTexture));
        
        // 分配存储并设置参数
        GL_ERRORCALL(glTextureStorage2D(mTexture, 1, internalFormat, mWidth, mHeight));
        GL_ERRORCALL(glTextureSubImage2D(mTexture, 0, 0, 0, mWidth, mHeight, 
                                        mTextureChannel, GL_UNSIGNED_BYTE, data));
        
        

        if (channel != ChannelType::Red) stbi_image_free(data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GL_ERRORCALL(glGenerateMipmap(GL_TEXTURE_2D));

        CreateBindLessHandle();
    }

    GLTexture::GLTexture(const string &path, uint32_t widthIn, int32_t heightIn, ChannelType channel, uint32_t unit) {
        mWidth = widthIn, mHeight = heightIn;
        mTextureChannel = ChoiceChannel(channel);
        GLint internalFormat = ChoiceInternal(channel);
        mEnumChannel = channel;

        GL_ERRORCALL(glCreateTextures(GL_TEXTURE_2D, 1, &mTexture));
        
        // 分配存储并设置参数
        GL_ERRORCALL(glTextureStorage2D(mTexture, 1, internalFormat, mWidth, mHeight));
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

        GL_ERRORCALL(glGenerateMipmap(GL_TEXTURE_2D));

        CreateBindLessHandle();

    }

    GLTexture::~GLTexture() {
        destory();
    }

    inline unsigned long long GLTexture::getHandle() const
    {
        return mTextureHandle;
    }

    void GLTexture::updateSubTex(int x, int y, int width, int height, unsigned char *data) {
        stbi_set_flip_vertically_on_load(true);

        size_t dataInsize =0;
        int channels;
        //计算size
        if (!height) dataInsize = width;
        else dataInsize = accumulateDataSize(width, height);

        unsigned char* readData = nullptr;
        if (mEnumChannel != ChannelType::Red) {
            readData =  stbi_load_from_memory(data, dataInsize, &mWidth, &mHeight, &channels, mTextureChannel);
        }
        else readData = data;

        if (width <= 0 || height <= 0) {
            FS_CORE_WARN("Invalid texture update size: {0}x{1}", width, height);
            return;
        }
        
        if (x < 0 || y < 0 || (x + width) > mWidth || (y + height) > mHeight) {
            FS_CORE_WARN("Texture update out of bounds: ({0},{1}) size {2}x{3} on {4}x{5} texture",
                        x, y, width, height, mWidth, mHeight);
            return;
        }

        if (!readData) {
            FS_CORE_ERROR("Texture update data is null");
            return;
        }

        GL_ERRORCALL(glTextureSubImage2D(mTexture, 0, x, y, width, height, mTextureChannel, GL_UNSIGNED_BYTE, readData));
    
        
    }

    void GLTexture::destory() {
        if (mTextureHandle) {
            // 使句柄非常驻
            glMakeTextureHandleNonResidentARB(mTextureHandle);
            mTextureHandle = 0;
        }
        
        if (mTexture != 0) {
            glDeleteTextures(1, &mTexture);
            mTexture = 0;
        }
    };

    int GLTexture::ChoiceChannel(ChannelType channel) {
        switch(channel) {
            case ChannelType::Red:  return GL_RED;
            case ChannelType::RGB:  return GL_RGB;
            case ChannelType::RGBA: return GL_RGBA;
        }
        return GL_NONE;
    }

    int GLTexture::ChoiceInternal(ChannelType channel) {
        switch(channel) {
            case ChannelType::Red: return GL_R8;
            case ChannelType::RGB: return GL_RGB8;
            case ChannelType::RGBA: return GL_RGBA8;
        }

        return GL_RGBA8;


    }

    void GLTexture::CreateBindLessHandle() {
            if (!GLAD_GL_ARB_bindless_texture) {
                FS_CORE_ERROR("Bindless texture extension not supported!");
                return;
            }

            mTextureHandle = GL_ERRORCALL(glGetTextureHandleARB(mTexture));
            if (!mTextureHandle) {
                FS_CORE_ERROR("Failed to create bindless texture handle");
                return;
            }
            
            glMakeTextureHandleResidentARB(mTextureHandle);

            FS_CORE_INFO("[Texture Debug] Handle: {}, GL Status: {}", mTextureHandle, glIsTextureHandleResidentARB(mTextureHandle) ? "Resident" : "Not Resident");
    }
    size_t GLTexture::accumulateDataSize(int width, int height) const {
        switch (mTextureChannel) {
            case GL_RED:   return width * height * 1;  // 单通道
            case GL_RGB:   return width * height * 3;  // 三通道
            case GL_RGBA:  return width * height * 4;  // 四通道
            default:
                FS_CORE_ERROR("Unsupported texture channel format: {}", mTextureChannel);
                return 0;
        }
        
    }
}
    