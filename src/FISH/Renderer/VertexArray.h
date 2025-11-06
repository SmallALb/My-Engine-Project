#pragma once

#include <memory>
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
        

        // 顶点编辑功能
        virtual bool UpdateVertexPosition(uint32_t vertexIndex, const glm::vec3& newPosition) = 0;
        virtual bool UpdateVertexColor(uint32_t vertexIndex, const glm::vec4& newColor) = 0;
        virtual bool UpdateVertexUV(uint32_t vertexIndex, const glm::vec2& newUV) = 0;
        virtual bool UpdateVertexNormal(uint32_t vertexIndex, const glm::vec3& newNormal) = 0;
        // 获取顶点信息
        virtual glm::vec3 GetVertexPosition(uint32_t vertexIndex) const = 0;
        virtual glm::vec3 GetVertexNormal(uint32_t vertexIndex) const = 0;
        virtual glm::vec4 GetVertexColor(uint32_t vertexIndex) const = 0;
        virtual glm::vec2 GetVertexUV(uint32_t vertexIndex) const = 0;
        //创建Vao
        static VertexArray* Create();
    };

}