#include "fspcs.h"
#include "FISH/Log.h"
#include "Platform/OpenGL/glad/glad.h"
#include "OpenGLTexture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "FISH/Renderer/stb_image.h"

namespace FISH {
    GLTexture::GLTexture(const string &path, uint32_t unit) {
        stbi_set_flip_vertically_on_load(1);
        int channels;

        mUnit = unit;
        unsigned char* data = stbi_load(path.c_str(), &mWidth, &mHeight,  &channels, STBI_rgb_alpha);
        
        glGenTextures(1, &mTexture);

        glActiveTexture(GL_TEXTURE0 + mUnit);

        glBindTexture(GL_TEXTURE_2D, mTexture);
        int w = mWidth, h = mHeight;
        for (int level = 0; true; level++) {
            glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            if (w == 1 && h == 1) {
                break;
            }

            w = w > 1 ? w / 2 : 1;
            h = h > 1 ? h / 2 : 1;
        }

        stbi_image_free(data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    }

    GLTexture::GLTexture(uint32_t unit, unsigned char *dataIn, uint32_t widthIn, uint32_t heightIn) {
        stbi_set_flip_vertically_on_load(true);

        mUnit = unit;

        int channels;

        uint32_t dataInsize = 0;
        if (!heightIn) dataInsize = widthIn;
        else dataInsize = widthIn * heightIn * 4;

        unsigned char* data =  stbi_load_from_memory(dataIn, dataInsize, &mWidth, &mHeight, &channels, STBI_rgb_alpha);

        glGenTextures(1, &mTexture);

        glActiveTexture(GL_TEXTURE0 + mUnit);

        glBindTexture(GL_TEXTURE_2D, mTexture);
        int w = mWidth, h = mHeight;
        for (int level = 0; true; level++) {
            glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            if (w == 1 && h == 1) {
                break;
            }

            w = w > 1 ? w / 2 : 1;
            h = h > 1 ? h / 2 : 1;
        }

        stbi_image_free(data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    }

    GLTexture::~GLTexture() {
        destory();
    }

    void GLTexture::bind()  {
        glActiveTexture(GL_TEXTURE0 + mUnit);
        glBindTexture(GL_TEXTURE_2D, mTexture);
    };

    void GLTexture::destory() {
        if (mTexture != 0) {
            glDeleteTextures(1, &mTexture);
        }
    };

}