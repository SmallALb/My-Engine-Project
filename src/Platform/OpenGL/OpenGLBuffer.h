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
        virtual void SetLayout(const BufferLayout& layout) override {mLayout = layout;};
        //ȡ������
        virtual const BufferLayout& GetLayout() const override {return mLayout;};
    private:
        unsigned int mVbo;
        BufferLayout mLayout; 
    };
    //Opengl�±��װ
    class GLIndexBuffer : public IndexBuffer {
    public:
        GLIndexBuffer(unsigned int* indices, uint32_t count);

        virtual ~GLIndexBuffer();

        virtual void bind() const;
        virtual void unbind() const;

        virtual uint32_t GetCount() const {return mCount; } 
    private:
        unsigned int    mVbo{0};
        uint32_t        mCount{0};
    };

}