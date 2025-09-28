#pragma once
#include "FISH/Renderer/Buffer.h"

namespace  FISH {
    //Opengl顶点封装
    class GLVertexBuffer : public VertexBuffer {
    public:
        GLVertexBuffer(float* vertices, uint32_t size);

        virtual ~GLVertexBuffer();
        //绑定
        virtual void bind() const  override;
        //解绑
        virtual void unbind() const override;
        //设置该数组的顶点描述
        virtual void SetLayout(const BufferLayout& layout) override {mLayout = layout;}
        //取得描述
        virtual const BufferLayout& GetLayout() const override {return mLayout;}

                //更新整个buffer数据
        virtual void UpdateData(float* vertices, uint32_t size, uint32_t offset) override;
        //更新buffer中第i个数据
        virtual void UpdateVertexData(uint32_t vertexIndex, const void* data, uint32_t dataSize) override;
        //映射到Buffer指针中
        virtual void* MapBuffer() override;
        //取消映射
        virtual void UnmapBuffer() override;

        virtual uint32_t GetVertexCount() const override;
        virtual uint32_t GetVertexSize() const override;
    private:
        unsigned int mVbo;
        BufferLayout mLayout; 
        uint32_t    mSize;
    };
    //Opengl下标封装
    class GLIndexBuffer : public IndexBuffer {
    public:
        GLIndexBuffer(unsigned int* indices, uint32_t count);

        virtual ~GLIndexBuffer();

        virtual void bind() const;
        virtual void unbind() const;

        virtual uint32_t GetCount() const {return mCount; } 

        virtual void UpdateData(unsigned int* indices, uint32_t count, uint32_t offset) override;

    private:
        unsigned int    mVbo{0};
        uint32_t        mCount{0};
    };
    //OpenGL帧缓冲封装
    class GLFrameBuffer : public FrameBuffer {
    public:
        GLFrameBuffer(uint32_t width, uint32_t height);
        virtual ~GLFrameBuffer();

        virtual void bind() const override;

        virtual void unbind() const override;

        virtual uint32_t GetColorAttachmentID() const override {return mColorAttachment;}

        virtual uint32_t GetDepthAttachmentID() const override {return mDepthAttachment;}

        virtual void resize(uint32_t width, uint32_t height) override;
        
        virtual bool isCompllete() const override;

        virtual void updateCleanStatus(int status) override;


        virtual uint32_t GetWidth() const override {return mWidth;}
        virtual uint32_t GetHeight() const override {return mHeight;}

    private:
        void CreateFrameBuffer();

        void Destory();

        void CreateAttachments();
    private:
        uint32_t mRendererID{0};
        uint32_t mColorAttachment{0};
        uint32_t mDepthAttachment{0};
        uint32_t mWidth{0};
        uint32_t mHeight{0};

        bool resizeTag{0};

        mutable int mPrevViewport[4]{0};
    };

}