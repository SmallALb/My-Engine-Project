#pragma once
#include "FISH/Renderer/VertexArray.h"

namespace FISH {

    class GLVertexArray :  public VertexArray{
        public:
            GLVertexArray();

            virtual ~GLVertexArray();
    
            virtual void bind() const override;
            //取消绑定
            virtual void unbind() const override;
    
            virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
    
            virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

            virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override {return mVertexBuffers;}

            virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override {return mIndexBuffer;}
            
            virtual const uint32_t GetIndexCounts() const override {return mIndexBuffer->GetCount();}

            // glDrawElements(GL_TRIANGLES, (mShape->GetIndexCounts()), GL_UNSIGNED_INT, 0);

            virtual void renderIndex(int beginId, int ElementType) override;

                    // 顶点编辑功能
            virtual bool UpdateVertexPosition(uint32_t vertexIndex, const glm::vec3& newPosition) override;
            virtual bool UpdateVertexColor(uint32_t vertexIndex, const glm::vec4& newColor) override;
            virtual bool UpdateVertexUV(uint32_t vertexIndex, const glm::vec2& newUV) override;
            virtual bool UpdateVertexNormal(uint32_t vertexIndex, const glm::vec3& newNormal) override;

            // 获取顶点信息
            virtual glm::vec3 GetVertexPosition(uint32_t vertexIndex) const override;
            virtual glm::vec4 GetVertexColor(uint32_t vertexIndex) const override;
            virtual glm::vec2 GetVertexUV(uint32_t vertexIndex) const override;
            virtual glm::vec3 GetVertexNormal(uint32_t vertexIndex) const override;

        private:
            std::shared_ptr<VertexBuffer> FindVertexBufferByType(VertexType type) const;
            uint32_t GetVertexOffsetInBuffer(VertexType type, uint32_t vertexIndex) const;
        private:
            std::vector<std::shared_ptr<VertexBuffer>> mVertexBuffers;
            std::shared_ptr<IndexBuffer> mIndexBuffer;
            uint32_t idx = 0;
            uint32_t mVao;
        };

}