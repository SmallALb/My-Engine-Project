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
        if (!data) {
            FS_CORE_ERROR("Error Texture Path: {0}", path.c_str());
            return;
        }

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
        FS_CORE_INFO("Creating GLTexture: {}x{}, channel: {}", widthIn, heightIn, static_cast<int>(channel));
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        mUnit = unit;
        mWidth = widthIn;
        mHeight = heightIn;
        mEnumChannel = channel;
        mTextureChannel = ChoiceChannel(channel);
        GLint internalFormat = ChoiceInternal(channel);

        // 直接使用传入的原始数据，不要用stbi_load_from_memory
        unsigned char* data = dataIn;

        // 创建纹理
        GL_ERRORCALL(glCreateTextures(GL_TEXTURE_2D, 1, &mTexture));
        
        if (mTexture == 0) {
            FS_CORE_ERROR("Failed to create OpenGL texture object");
            return;
        }

        // 分配存储空间
        GL_ERRORCALL(glTextureStorage2D(mTexture, 1, internalFormat, mWidth, mHeight));
        
        // 检查是否分配成功
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            FS_CORE_ERROR("glTextureStorage2D failed with error: {}", error);
            glDeleteTextures(1, &mTexture);
            mTexture = 0;
            return;
        }

        // 上传纹理数据
        GL_ERRORCALL(glTextureSubImage2D(mTexture, 0, 0, 0, mWidth, mHeight, 
                                        mTextureChannel, GL_UNSIGNED_BYTE, data));

        // 检查上传是否成功
        error = glGetError();
        if (error != GL_NO_ERROR) {
            FS_CORE_ERROR("glTextureSubImage2D failed with error: {}", error);
            glDeleteTextures(1, &mTexture);
            mTexture = 0;
            return;
        }

        // 设置纹理参数
        glTextureParameteri(mTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(mTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(mTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(mTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 生成mipmap
        GL_ERRORCALL(glGenerateTextureMipmap(mTexture));

        // 只有纹理创建成功后才创建bindless handle
        if (mTexture != 0) {
            CreateBindLessHandle();
        } else {
            FS_CORE_ERROR("Cannot create bindless handle for invalid texture");
            return;
        }
        FS_CORE_INFO("Successed Build Texture!");
    }

    GLTexture::GLTexture(const string &path, uint32_t widthIn, int32_t heightIn, ChannelType channel, uint32_t unit) {
        FS_CORE_INFO("Creating GLTexture: {}, {}x{}, channel: {}", path, widthIn, heightIn, static_cast<int>(channel));
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

    unsigned long long GLTexture::getTextureID() const {
        return mTexture;
    }

    inline unsigned long long GLTexture::getHandle() const
    {
        return mTextureHandle;
    }

    TextureInfo GLTexture::getTextureInfo() const {
        TextureInfo info;
        info.width = mWidth;
        info.height = mHeight;
        info.aspectRatio = mHeight > 0 ? (float)mWidth/(float)(mHeight) : 0.0f;
        info.pixelCount = mWidth * mHeight;
        info.channelType = mEnumChannel;

        auto pixelData = readTextureData();
        info.pixels.reserve(info.pixelCount);

        for (uint32_t i = 0; i < info.pixelCount; ++i) {
        uint32_t index = i * getBytesPerPixel();
            PixelInfo color;
            
            switch (mEnumChannel) {
                case ChannelType::Red:
                    color = PixelInfo(pixelData[index], 0, 0, 255);
                    break;
                case ChannelType::RGB:
                    if (index + 2 < pixelData.size()) {
                        color = PixelInfo(pixelData[index], pixelData[index + 1], pixelData[index + 2], 255);
                    }
                    break;
                case ChannelType::RGBA:
                    if (index + 3 < pixelData.size()) {
                        color = PixelInfo(pixelData[index], pixelData[index + 1], pixelData[index + 2], pixelData[index + 3]);
                    }
                    break;
            }
            
            info.pixels.push_back(color);
        }
    

        return info;
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
    PixelInfo GLTexture::getPixelInfo(uint32_t x, uint32_t y) const {
        if (x >= mWidth || y >= mHeight) return PixelInfo();
        auto pixelData = readTextureData();
        uint32_t index = (y * mWidth + x) * getBytesPerPixel();

        if (index >= pixelData.size()) 
            return PixelInfo();

        PixelInfo color;
        switch (mEnumChannel) {
            case ChannelType::Red:
                color = PixelInfo(pixelData[index], 0, 0, 255);
                break;
            case ChannelType::RGB:
                if (index + 2 < pixelData.size()) {
                    color = PixelInfo(pixelData[index], pixelData[index + 1], pixelData[index + 2], 255);
                }
                break;
            case ChannelType::RGBA:
                if (index + 3 < pixelData.size()) {
                    color = PixelInfo(pixelData[index], pixelData[index + 1], pixelData[index + 2], pixelData[index + 3]);
                }
                break;
        }
        return color;   
    }

    std::vector<uint8_t> GLTexture::readTextureData() const {
        std::vector<uint8_t> data(mWidth * mHeight * getBytesPerPixel());
        int currentfbo;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentfbo);

        unsigned int fbo;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
            glReadPixels(0, 0, mWidth, mHeight, mTextureChannel, GL_UNSIGNED_BYTE, data.data());
        glBindFramebuffer(GL_FRAMEBUFFER, currentfbo);
        glDeleteFramebuffers(1, &fbo);

        return data;
        
    }
    int GLTexture::getBytesPerPixel() const {
        switch (mEnumChannel) {
            case ChannelType::Red: return 1;
            case ChannelType::RGB: return 3;
            case ChannelType::RGBA: return 4;
            default: return 4;
        }
    }
}
    