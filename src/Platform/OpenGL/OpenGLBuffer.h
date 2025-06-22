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
        virtual void SetLayout(const BufferLayout& layout) override {mLayout = layout;};
        //取得描述
        virtual const BufferLayout& GetLayout() const override {return mLayout;};
    private:
        unsigned int mVbo;
        BufferLayout mLayout; 
    };
    //Opengl下标封装
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