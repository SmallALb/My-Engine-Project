#pragma once

#include <memory>
#include "Buffer.h"
#include "FISH/Core.h"

namespace FISH {
    //VAO封装
    class API_ VertexArray {
    public:
        virtual ~VertexArray() {}
        //绑定使用该Vao
        virtual void bind() const = 0;
        //取消绑定
        virtual void unbind() const = 0;
        //添加顶点层（灌入数据）
        virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)  = 0;
        //设置下标层信息
        virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)  = 0;
        //获取所有顶点层信息
        virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
        //获取下标层信息
        virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;
        //获取下标计数
        virtual const uint32_t GetIndexCounts() const = 0;
        //按顶点下标绘制图形
        virtual void renderIndex(int beginId, int ElementType) = 0;

        //创建Vao
        static VertexArray* Create();
    };

}