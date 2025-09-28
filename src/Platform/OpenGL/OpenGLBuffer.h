#pragma once
#include "FISH/Renderer/Buffer.h"

namespace  FISH {
    //Opengl�����װ
    class GLVertexBuffer : public VertexBuffer {
    public:
        GLVertexBuffer(float* vertices, uint32_t size);

        virtual ~GLVertexBuffer();
        //��
        virtual void bind() const  override;
        //���
        virtual void unbind() const override;
        //���ø�����Ķ�������
        virtual void SetLayout(const BufferLayout& layout) override {mLayout = layout;}
        //ȡ������
        virtual const BufferLayout& GetLayout() const override {return mLayout;}

                //��������buffer����
        virtual void UpdateData(float* vertices, uint32_t size, uint32_t offset) override;
        //����buffer�е�i������
        virtual void UpdateVertexData(uint32_t vertexIndex, const void* data, uint32_t dataSize) override;
        //ӳ�䵽Bufferָ����
        virtual void* MapBuffer() override;
        //ȡ��ӳ��
        virtual void UnmapBuffer() override;

        virtual uint32_t GetVertexCount() const override;
        virtual uint32_t GetVertexSize() const override;
    private:
        unsigned int mVbo;
        BufferLayout mLayout; 
        uint32_t    mSize;
    };
    //Opengl�±��װ
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
    //OpenGL֡�����װ
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