#pragma once
#include "FISH/Core.h"

namespace FISH {
    //���������
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

    //��������
    enum class VertexType {
        None = 0,
        Normal,
        Position,
        Color,
        UV,
    };
    //���ض�Ӧ���͵Ĵ�С
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

    //�ֲ���������
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

        //��ȡ���͵ļ���
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

    //Vbo��Ϣ��¼��
    class API_ BufferLayout {
    public:
        BufferLayout() {}
        //����Vbo��Ϣ����
        BufferLayout(const std::initializer_list<BufferElement>& elements) : mElements(elements) {
            CalculateOffsetsAndStride();
        }

        //��ȡ���е�vbo��Ϣ
        inline const std::vector<BufferElement>& GetElements() const {return mElements; }

        std::vector<BufferElement>::iterator begin() {return mElements.begin();}
        std::vector<BufferElement>::iterator end() {return mElements.end();}

        std::vector<BufferElement>::const_iterator begin() const {return mElements.begin();}
        std::vector<BufferElement>::const_iterator end() const {return mElements.end();}
        inline size_t size() const {return mElements.size(); }
        //��ȡƫ��
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

    //���㻺����
    class API_ VertexBuffer {
    public:
        virtual ~VertexBuffer() {}
        //��
        virtual void bind() const = 0;
        //ȡ����
        virtual void unbind() const = 0;
        //����Buffer��
        virtual void SetLayout(const BufferLayout& layout) = 0;
        //��ȡ��ǰ�Ļ����
        virtual const BufferLayout& GetLayout() const = 0;
        //��������buffer
        static VertexBuffer* Create(float* vertices, uint32_t size);
    };

    //�����±껺����
    class API_ IndexBuffer {
    public:
        virtual ~IndexBuffer() {}

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual unsigned int GetCount() const = 0;

        static IndexBuffer* Create(unsigned int* indices, uint32_t size);
    };

     

}