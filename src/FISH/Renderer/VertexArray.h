#pragma once

#include <memory>
#include "Buffer.h"
#include "FISH/Core.h"

namespace FISH {
    //VAO��װ
    class API_ VertexArray {
    public:
        virtual ~VertexArray() {}
        //��ʹ�ø�Vao
        virtual void bind() const = 0;
        //ȡ����
        virtual void unbind() const = 0;
        //��Ӷ���㣨�������ݣ�
        virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)  = 0;
        //�����±����Ϣ
        virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)  = 0;
        //��ȡ���ж������Ϣ
        virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
        //��ȡ�±����Ϣ
        virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;
        //��ȡ�±����
        virtual const uint32_t GetIndexCounts() const = 0;
        //�������±����ͼ��
        virtual void renderIndex(int beginId, int ElementType) = 0;

        //����Vao
        static VertexArray* Create();
    };

}