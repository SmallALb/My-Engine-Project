#pragma once
#include "FISH/Renderer/VertexArray.h"

namespace FISH {

    class GLVertexArray :  public VertexArray{
        public:
            GLVertexArray();

            virtual ~GLVertexArray();
    
            virtual void bind() const override;
            //È¡Ïû°ó¶¨
            virtual void unbind() const override;
    
            virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
    
            virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

            virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override {return mVertexBuffers;}

            virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override {return mIndexBuffer;}
            
            virtual const uint32_t GetIndexCounts() const override {return mIndexBuffer->GetCount();}

            // glDrawElements(GL_TRIANGLES, (mShape->GetIndexCounts()), GL_UNSIGNED_INT, 0);

            virtual void renderIndex(int beginId, int ElementType) override;

        private:
            std::vector<std::shared_ptr<VertexBuffer>> mVertexBuffers;
            std::shared_ptr<IndexBuffer> mIndexBuffer;
            uint32_t idx = 0;
            uint32_t mVao;
        };

}