#pragma once
#include "FISH/Core.h"

namespace FISH {
    //传入的类型
    enum class ShaderDataType {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool
    };

    //顶点类型
    enum class VertexType {
        None = 0,
        Normal,
        Position,
        Color,
        UV,
    };
    //返回对应类型的大小
    static uint32_t ShaderDataTypeSize(ShaderDataType type) {
        switch(type) {
            case ShaderDataType::Float:     return 4;
            case ShaderDataType::Float2:    return 4 * 2;
            case ShaderDataType::Float3:    return 4 * 3;
            case ShaderDataType::Float4:    return 4 * 4;
            case ShaderDataType::Mat3:      return 4 * 3 * 3;
            case ShaderDataType::Mat4:      return 4 * 4 * 4;
            case ShaderDataType::Int:       return 4;
            case ShaderDataType::Int2:      return 4 * 2;
            case ShaderDataType::Int3:      return 4 * 3;
            case ShaderDataType::Int4:      return 4 * 4;
            case ShaderDataType::Bool:      return 1;
            case ShaderDataType::None:      return 0;
            default:
                break;
        }
        return 0;
    }

    //分布属性描述
    struct BufferElement {
        ShaderDataType Type;
        uint32_t Size;
        uint32_t Offset;
        bool Normalized;
        std::string Name;
        VertexType ElementType;

        BufferElement() {}

        BufferElement(ShaderDataType type, const VertexType& typ, const std::string& name = "element", bool normalized = 0): 
            Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized), Name(name), ElementType(typ)
        {}

        //获取类型的计数
        uint32_t GetComponentCount() const {
            switch(Type) {
                case ShaderDataType::Float:     return 1;
                case ShaderDataType::Float2:    return 2;
                case ShaderDataType::Float3:    return 3;
                case ShaderDataType::Float4:    return 4;
                case ShaderDataType::Int:       return 1;
                case ShaderDataType::Int2:      return 2;
                case ShaderDataType::Int3:      return 3;
                case ShaderDataType::Int4:      return 4;
                case ShaderDataType::Bool:      return 1;
                case ShaderDataType::Mat3:      return 3 * 3;
                case ShaderDataType::Mat4:      return 4 * 4;
                default:
                    break;
            }
            return 0;
        }
        
    };

    //Vbo信息记录层
    class API_ BufferLayout {
    public:
        BufferLayout() {}
        //传入Vbo信息集合
        BufferLayout(const std::initializer_list<BufferElement>& elements) : mElements(elements) {
            CalculateOffsetsAndStride();
        }

        //获取所有的vbo信息
        inline const std::vector<BufferElement>& GetElements() const {return mElements; }

        std::vector<BufferElement>::iterator begin() {return mElements.begin();}
        std::vector<BufferElement>::iterator end() {return mElements.end();}

        std::vector<BufferElement>::const_iterator begin() const {return mElements.begin();}
        std::vector<BufferElement>::const_iterator end() const {return mElements.end();}
        inline size_t size() const {return mElements.size(); }
        //获取偏移
        inline uint32_t GetStride() const {return mStride;}

        BufferElement& operator [] (const uint32_t& idx) {return mElements[idx];}
    private:
        void CalculateOffsetsAndStride() {
            uint32_t offset = 0; mStride = 0;
            for (auto& e : mElements) {
                e.Offset = offset;
                offset += e.Size;
            }
            mStride = offset;
        }
    private:
        std::vector<BufferElement> mElements;
        uint32_t mStride = 0;
    };

    //顶点缓冲区
    class API_ VertexBuffer {
    public:
        virtual ~VertexBuffer() {}
        //绑定
        virtual void bind() const = 0;
        //取消绑定
        virtual void unbind() const = 0;
        //设置Buffer层
        virtual void SetLayout(const BufferLayout& layout) = 0;
        //获取当前的缓冲层
        virtual const BufferLayout& GetLayout() const = 0;
        //创建顶点buffer
        static VertexBuffer* Create(float* vertices, uint32_t size);
        //更新整个buffer数据
        virtual void UpdateData(float* vertices, uint32_t size, uint32_t offset = 0) = 0;
        //更新buffer中第i个数据
        virtual void UpdateVertexData(uint32_t vertexIndex, const void* data, uint32_t dataSize) = 0;
        //映射到Buffer指针中
        virtual void* MapBuffer() = 0;
        //取消映射
        virtual void UnmapBuffer() = 0;

        virtual uint32_t GetVertexCount() const = 0;
        virtual uint32_t GetVertexSize() const = 0;
    };

    //绘制下标缓冲区
    class API_ IndexBuffer {
    public:
        virtual ~IndexBuffer() {}

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual unsigned int GetCount() const = 0;

        virtual void UpdateData(unsigned int* indices, uint32_t count, uint32_t offset = 0) = 0;

        static IndexBuffer* Create(unsigned int* indices, uint32_t size);
    };

    class API_ FrameBuffer {
    public:
        virtual void bind() const = 0;

        virtual void unbind() const = 0;

        virtual uint32_t GetColorAttachmentID() const = 0;

        virtual uint32_t GetDepthAttachmentID() const = 0;

        virtual void resize(uint32_t width, uint32_t height) = 0;
        
        virtual bool isCompllete() const = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        virtual void updateCleanStatus(int status) = 0;

        static FrameBuffer* Create(uint32_t width, uint32_t height);

    };

    using VertexBufferPtr = std::shared_ptr<VertexBuffer>;
    using IndexBufferPtr = std::shared_ptr<IndexBuffer>;
    using FrameBufferPtr = std::shared_ptr<FrameBuffer>;


}