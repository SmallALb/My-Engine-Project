#include "fspcs.h"
#include "glad/glad.h"
#include "OpenGLErrorCheck.h"
#include "OpenGLBuffer.h"
#include "FISH/Log.h"

namespace FISH {
    /////////////////////////////////////
        //VERTEX
    /////////////////////////////////////

    GLVertexBuffer::GLVertexBuffer(float *vertices, uint32_t size): mSize(size) {
        GL_ERRORCALL(glCreateBuffers(1, &mVbo));
        GL_ERRORCALL(glBindBuffer(GL_ARRAY_BUFFER, mVbo));
        GL_ERRORCALL(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW));
    }

    GLVertexBuffer::~GLVertexBuffer() {
        glDeleteBuffers(1, &mVbo);
    }

    void GLVertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    }

    void GLVertexBuffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }

    void GLVertexBuffer::UpdateData(float *vertices, uint32_t size, uint32_t offset) {
        bind();
        if (offset + size > mSize) FS_CORE_ERROR("Buffer update out of bounds! offset: {0}, dataSize: {1}, Bounds:{2}", offset, size, mSize);
        
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
    }

    void GLVertexBuffer::UpdateVertexData(uint32_t vertexIndex, const void *data, uint32_t dataSize) {
        uint32_t offset = vertexIndex * GetVertexSize();
        if (offset + dataSize > mSize) FS_CORE_ERROR("Buffer update out of bounds! offset: {0}, dataSize: {1}, Bounds:{2}", offset, dataSize, mSize);
        bind();
        glBufferSubData(GL_ARRAY_BUFFER, offset, dataSize, data);
    }

    void *GLVertexBuffer::MapBuffer() {
        bind();
        return glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    }

    void GLVertexBuffer::UnmapBuffer() {
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    uint32_t GLVertexBuffer::GetVertexCount() const {
        if (mLayout.GetStride() == 0) return 0;
        return mSize / mLayout.GetStride();
    }

    uint32_t GLVertexBuffer::GetVertexSize() const {
        return mLayout.GetStride();
    }

    /////////////////////////////////////
    // INDEX
    /////////////////////////////////////
    GLIndexBuffer::GLIndexBuffer(unsigned int *indices, uint32_t count) : mCount(count) {
        glCreateBuffers(1, &mVbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    GLIndexBuffer::~GLIndexBuffer() {
        glDeleteBuffers(1, &mVbo);
    }


    void GLIndexBuffer::bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVbo);
    }

    void GLIndexBuffer::unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    }

    void GLIndexBuffer::UpdateData(unsigned int *indices, uint32_t count, uint32_t offset) {
        if ((offset + count) * 4 > mCount * 4) FS_CORE_ERROR("Buffer update out of bounds! offset: {0}, dataSize: {1}, Bounds:{2}", offset*4, count*4, mCount * 4);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset*4, count*4, indices);
    }
    GLFrameBuffer::GLFrameBuffer(uint32_t width, uint32_t height): mWidth(width), mHeight(height) {
        CreateFrameBuffer();
        CreateAttachments();
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            FS_CORE_ERROR("Framebuffer is not complete! Status: 0x{0:x}", status);
            switch(status) {
                case GL_FRAMEBUFFER_UNDEFINED: FS_CORE_ERROR("Framebuffer undefined"); break;
                case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: FS_CORE_ERROR("Incomplete attachment"); break;
                case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: FS_CORE_ERROR("Missing attachment"); break;
                case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: FS_CORE_ERROR("Incomplete draw buffer"); break;
                case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: FS_CORE_ERROR("Incomplete read buffer"); break;
                case GL_FRAMEBUFFER_UNSUPPORTED: FS_CORE_ERROR("Unsupported"); break;
                case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: FS_CORE_ERROR("Incomplete multisample"); break;
                case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: FS_CORE_ERROR("Incomplete layer targets"); break;
            }
        }
        unbind();
    }

    GLFrameBuffer::~GLFrameBuffer() {
        Destory();
    }
    void GLFrameBuffer::bind() const {
        if (!resizeTag) glGetIntegerv(GL_VIEWPORT, mPrevViewport);
        glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);
        glViewport(0, 0, mWidth, mHeight);
    }
    void GLFrameBuffer::unbind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(mPrevViewport[0], mPrevViewport[1], mPrevViewport[2], mPrevViewport[3]);
    }

    void GLFrameBuffer::resize(uint32_t width, uint32_t height) {
        if (width == mWidth && height == mHeight) return;
        mWidth = width;
        mHeight = height;
        resizeTag = 1;
        if (mColorAttachment) {
            glDeleteTextures(1, &mColorAttachment);
        }
        if (mDepthAttachment) {
            glDeleteRenderbuffers(1, &mDepthAttachment);
        }
        CreateAttachments();
        unbind();
        resizeTag = 0;
        
    }
    bool GLFrameBuffer::isCompllete() const {
        return false;
    }
    void GLFrameBuffer::updateCleanStatus(int status) {
        glClear(status);
    }

    void GLFrameBuffer::CreateFrameBuffer()
    {
        glGenFramebuffers(1, &mRendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);
    }

    void GLFrameBuffer::Destory() {
        if (mColorAttachment) {
            glDeleteTextures(1, &mColorAttachment);
            mColorAttachment = 0;
        }
        if (mDepthAttachment) {
            glDeleteTextures(1, &mDepthAttachment);
            mDepthAttachment = 0;
        }
        if (mRendererID) {
            glDeleteFramebuffers(1, &mRendererID);
            mRendererID = 0;
        }
    }

    void GLFrameBuffer::CreateAttachments() {
        bind();
        glGenTextures(1, &mColorAttachment);
        glBindTexture(GL_TEXTURE_2D, mColorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment, 0);
        
        // 创建深度附件
        glGenRenderbuffers(1, &mDepthAttachment);
        glBindRenderbuffer(GL_RENDERBUFFER, mDepthAttachment);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthAttachment);
        
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
}