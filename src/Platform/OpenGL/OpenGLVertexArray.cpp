#include "fspcs.h"
#include "Platform/OpenGL/glad/glad.h"
#include "OpenGLErrorCheck.h"
#include "OpenGLVertexArray.h"
#include "FISH/Log.h"

namespace FISH {
    
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:         return GL_FLOAT;
            case ShaderDataType::Float2:        return GL_FLOAT;
            case ShaderDataType::Float3:        return GL_FLOAT;
            case ShaderDataType::Float4:        return GL_FLOAT;
            case ShaderDataType::Int:           return GL_INT;
            case ShaderDataType::Int2:          return GL_INT;
            case ShaderDataType::Int3:          return GL_INT;
            case ShaderDataType::Int4:          return GL_INT;
            case ShaderDataType::Bool:          return GL_BOOL;
            case ShaderDataType::Mat3:          return GL_FLOAT;
            case ShaderDataType::Mat4:          return GL_FLOAT;

        }
        return 0;
    }

    GLVertexArray::GLVertexArray() {
        GL_ERRORCALL(glCreateVertexArrays(1, &mVao));
    }

    GLVertexArray::~GLVertexArray() {
        glDeleteVertexArrays(1, &mVao);
    }

    void GLVertexArray::bind() const {
        glBindVertexArray(mVao);
    };
    //取消绑定
    void GLVertexArray::unbind() const {
        glBindVertexArray(0);
    };

    void GLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
        if (!vertexBuffer->GetLayout().size()) FS_CORE_ERROR("failed: 该顶点数据没有分布属性");
        bind();
        vertexBuffer->bind();
        
        for (const auto& e : vertexBuffer->GetLayout()) {

             GL_ERRORCALL(glEnableVertexAttribArray(idx));
             GL_ERRORCALL(glVertexAttribPointer(idx++, 
                e.GetComponentCount(), 
                ShaderDataTypeToOpenGLBaseType(e.Type),
                e.Normalized ?  GL_TRUE : GL_FALSE,
                vertexBuffer->GetLayout().GetStride(),
                (const void*)e.Offset));
        }

        mVertexBuffers.push_back(vertexBuffer);
        unbind();
    };

    void GLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
        bind();
        indexBuffer->bind();

        mIndexBuffer = indexBuffer;

        unbind();
    }
    void GLVertexArray::renderIndex(int beginId, int ElementType) {
        bind();
        glDrawElements(ElementType, (mIndexBuffer->GetCount()), GL_UNSIGNED_INT, (const void*)beginId);
        unbind();
    };
}