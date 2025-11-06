#include "fspcs.h"
#include "Platform/OpenGL/glad/glad.h"
#include "OpenGLErrorCheck.h"
#include "OpenGLBuffer.h"
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
    //È¡Ïû°ó¶¨
    void GLVertexArray::unbind() const {
        glBindVertexArray(0);
    };

    void GLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
        if (!vertexBuffer->GetLayout().size()) FS_CORE_ERROR("failed: No Layout in this Buffer");
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
    }
    bool GLVertexArray::UpdateVertexPosition(uint32_t vertexIndex, const glm::vec3 &newPosition) {
        auto posBuffer = FindVertexBufferByType(VertexType::Position);
        if (!posBuffer) return false;
        posBuffer->UpdateVertexData(vertexIndex, &newPosition, sizeof(glm::vec3));
        return true;
    }
    bool GLVertexArray::UpdateVertexColor(uint32_t vertexIndex, const glm::vec4 &newColor) {
        auto colorBuffer = FindVertexBufferByType(VertexType::Color);
        if (!colorBuffer) return false;
        colorBuffer->UpdateVertexData(vertexIndex, &newColor, sizeof(glm::vec4));
        return true;
    }

    bool GLVertexArray::UpdateVertexUV(uint32_t vertexIndex, const glm::vec2 &newUV) {
        auto uvBuffer = FindVertexBufferByType(VertexType::UV);
        if (!uvBuffer) return false;
        uvBuffer->UpdateVertexData(vertexIndex, &newUV, sizeof(glm::vec2));
        return true;
    }

    bool GLVertexArray::UpdateVertexNormal(uint32_t vertexIndex, const glm::vec3 &newNormal) {
        auto normalBuffer = FindVertexBufferByType(VertexType::Normal);
        if (!normalBuffer) return false;
        normalBuffer->UpdateVertexData(vertexIndex, &newNormal, sizeof(glm::vec3));
        return true;
    }

    glm::vec3 GLVertexArray::GetVertexPosition(uint32_t vertexIndex) const {
        glm::vec3 position(0.0f);
        auto posBuffer = FindVertexBufferByType(VertexType::Position);
        if (posBuffer) {
            void* data = posBuffer->MapBuffer();
            if (data) {
                uint32_t offset = GetVertexOffsetInBuffer(VertexType::Position, vertexIndex);
                position = *reinterpret_cast<glm::vec3*>(static_cast<char*>(data) + offset);
                posBuffer->UnmapBuffer();
            }
        }
        return position;
    }

    glm::vec4 GLVertexArray::GetVertexColor(uint32_t vertexIndex) const {
        glm::vec4 color(0.0f);
        auto colorBuffer = FindVertexBufferByType(VertexType::Color);
        if (colorBuffer) {
            void* data = colorBuffer->MapBuffer();
            if (data) {
                uint32_t offset = GetVertexOffsetInBuffer(VertexType::Color, vertexIndex);
                color = *reinterpret_cast<glm::vec4*>(static_cast<char*>(data) + offset);
                colorBuffer->UnmapBuffer();
            }
        }
        return color;
    }

    glm::vec2 GLVertexArray::GetVertexUV(uint32_t vertexIndex) const {
        glm::vec2 uv(0.0f);
        auto uvBuffer = FindVertexBufferByType(VertexType::UV);
        if (uvBuffer) {
            void* data = uvBuffer->MapBuffer();
            if (data) {
                uint32_t offset = GetVertexOffsetInBuffer(VertexType::UV, vertexIndex);
                uv = *reinterpret_cast<glm::vec4*>(static_cast<char*>(data) + offset);
                uvBuffer->UnmapBuffer();
            }
        }
        return uv;
    }

    glm::vec3 GLVertexArray::GetVertexNormal(uint32_t vertexIndex) const {
        glm::vec3 normal(0.0);
        auto normalBuffer = FindVertexBufferByType(VertexType::Normal);
        if (normalBuffer) {
            void* data = normalBuffer->MapBuffer();
            if (data) {
                uint32_t offset = GetVertexOffsetInBuffer(VertexType::Normal, vertexIndex);
                normal = *reinterpret_cast<glm::vec3*>(static_cast<char*>(data) + offset);
                normalBuffer->UnmapBuffer();
            }
        }
        return normal;
    }

    std::shared_ptr<VertexBuffer> GLVertexArray::FindVertexBufferByType(VertexType type) const {
        for (const auto& buffer : mVertexBuffers) for (const auto& e : buffer->GetLayout()) if (e.ElementType == type){
            return buffer;
        }
        return nullptr;
    }

    uint32_t GLVertexArray::GetVertexOffsetInBuffer(VertexType type, uint32_t vertexIndex) const {
        for (const auto& buffer : mVertexBuffers) for (const auto& e : buffer->GetLayout()) if (e.ElementType == type){
            return vertexIndex * buffer->GetVertexSize() + e.Offset;
        }
        return 0;
    };
}