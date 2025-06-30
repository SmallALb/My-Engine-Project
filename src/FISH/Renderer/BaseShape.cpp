#include "fspcs.h"
#include "FISH/Renderer/Buffer.h"
#include "FISH/Renderer/VertexArray.h"
#include "FISH/Renderer/RenderElement.h"
#include "FISH/Renderer/Texture.h"
#include "BaseShape.h"

namespace FISH
{
    Shape::Shape() {
        mTexture = Texture::NoneTexture;
    }

    Shape::~Shape() {

    }

    Shape::Shape(std::shared_ptr<VertexBuffer> vertx, std::shared_ptr<IndexBuffer> index) {
        mVao.reset(VertexArray::Create());
        mVao->AddVertexBuffer(vertx);
        mVao->SetIndexBuffer(index);
    }

    uint32_t Shape::GetIndexCounts() const {return mVao->GetIndexCounts(); }

    void Shape::setTexture(const std::shared_ptr<Texture>& texture) {
        mTexture = texture;
    }

    Shape* Shape::CreateBox(float size) {
        Shape* shape = new Shape();
        float halfSize = size / 2.0f;
        //λ��
        float vertices[] = {
            // ǰ�棨-Z��

            -halfSize, -halfSize, -halfSize,   // v0
            halfSize, -halfSize, -halfSize,    // v1
            halfSize,  halfSize, -halfSize,     // v2
            -halfSize, -halfSize, -halfSize,    // v0 (�ظ�)
            halfSize,  halfSize, -halfSize,     // v2 (�ظ�)
            -halfSize,  halfSize, -halfSize,    // v3

            // ���棨+Z��

            -halfSize, -halfSize,  halfSize,   // v4
            -halfSize,  halfSize,  halfSize,   // v5
            halfSize,  halfSize,  halfSize,    // v6
            -halfSize, -halfSize,  halfSize,   // v4 (�ظ�)
            halfSize,  halfSize,  halfSize,    // v6 (�ظ�)
            halfSize, -halfSize,  halfSize,    // v7

            // ���棨-X��

            -halfSize,  halfSize,  halfSize,   // v8
            -halfSize, -halfSize,  halfSize,   // v9
            -halfSize, -halfSize, -halfSize,   // v10
            -halfSize,  halfSize,  halfSize,   // v8 (�ظ�)
            -halfSize, -halfSize, -halfSize,   // v10 (�ظ�)
            -halfSize,  halfSize, -halfSize,   // v11

            // ���棨+X��

            halfSize,  halfSize, -halfSize,    // v12
            halfSize, -halfSize, -halfSize,    // v13
            halfSize, -halfSize,  halfSize,    // v14
            halfSize,  halfSize, -halfSize,    // v12 (�ظ�)
            halfSize, -halfSize,  halfSize,    // v14 (�ظ�)
            halfSize,  halfSize,  halfSize,    // v15

            // ���棨+Y��

            -halfSize,  halfSize, -halfSize,   // v16
            halfSize,  halfSize, -halfSize,    // v17
            halfSize,  halfSize,  halfSize,    // v18
            -halfSize,  halfSize, -halfSize,   // v16 (�ظ�)
            halfSize,  halfSize,  halfSize,    // v18 (�ظ�)
            -halfSize,  halfSize,  halfSize,   // v19

            // ���棨-Y��

            -halfSize, -halfSize, -halfSize,   // v20
            -halfSize, -halfSize,  halfSize,   // v21
            halfSize, -halfSize,  halfSize,    // v22
            -halfSize, -halfSize, -halfSize,   // v20 (�ظ�)
            halfSize, -halfSize,  halfSize,    // v22 (�ظ�)
            halfSize, -halfSize, -halfSize,    // v23             
        };

        // ����uv��������
        float uvs[] = {
       // ǰ��

            0.0f, 0.0f,   // v0
            1.0f, 0.0f,   // v1
            1.0f, 1.0f,   // v2
            0.0f, 0.0f,   // v0
            1.0f, 1.0f,   // v2
            0.0f, 1.0f,   // v3

            // ����

            0.0f, 0.0f,   // v4
            0.0f, 1.0f,   // v5
            1.0f, 1.0f,   // v6
            0.0f, 0.0f,   // v4
            1.0f, 1.0f,   // v6
            1.0f, 0.0f,   // v7

            // ����

            1.0f, 0.0f,   // v8
            0.0f, 0.0f,   // v9
            0.0f, 1.0f,   // v10
            1.0f, 0.0f,   // v8
            0.0f, 1.0f,   // v10
            1.0f, 1.0f,   // v11

            // ����

            1.0f, 0.0f,   // v12
            0.0f, 0.0f,   // v13
            0.0f, 1.0f,   // v14
            1.0f, 0.0f,   // v12
            0.0f, 1.0f,   // v14
            1.0f, 1.0f,   // v15

            // ����

            0.0f, 1.0f,   // v16
            1.0f, 1.0f,   // v17
            1.0f, 0.0f,   // v18
            0.0f, 1.0f,   // v16
            1.0f, 0.0f,   // v18
            0.0f, 0.0f,   // v19

            // ����

            0.0f, 1.0f,   // v20
            0.0f, 0.0f,   // v21
            1.0f, 0.0f,   // v22
            0.0f, 1.0f,   // v20
            1.0f, 0.0f, 
            1.0f, 1.0f,   // v23
        };

        //����
        float normals[] = {
            // ǰ�棨-Z��

            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,

            // ���棨+Z��

            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,

            // ���棨-X��

            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,

            // ���棨+X��

            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,

            // ���棨+Y��

            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            // ���棨-Y��

            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
        };

        // �����������飨ÿ��4�����㣬ÿ�������ε������������ģ�
        unsigned int indices[] = {
            0, 1, 2, 3, 4, 5,
            6, 7, 8, 9, 10, 11,
            12,13,14,15,16,17,
            18,19,20,21,22,23,
            24,25,26,27,28,29,
            30,31,32,33,34,35
        };

        //��ɫ
        float colors[] = {
            // ��һ�棨��ɫ����ȫ��͸����
            1.0, 0.1, 0.0, 1.0,
            1.0, 0.1, 0.0, 1.0,
            1.0, 0.1, 0.0, 1.0,
            1.0, 0.1, 0.0, 1.0,
            1.0, 0.1, 0.0, 1.0,
            1.0, 0.1, 0.0, 1.0,
        
            // �ڶ��棨��ɫ����΢͸����
            1.0, 0.3, 0.0, 0.8,
            1.0, 0.3, 0.0, 0.8,
            1.0, 0.3, 0.0, 0.8,
            1.0, 0.3, 0.0, 0.8,
            1.0, 0.3, 0.0, 0.8,
            1.0, 0.3, 0.0, 0.8,
        
            // �����棨��ɫ��͸���Ƚ��ͣ�
            1.0, 0.5, 0.0, 0.6,
            1.0, 0.5, 0.0, 0.6,
            1.0, 0.5, 0.0, 0.6,
            1.0, 0.5, 0.0, 0.6,
            1.0, 0.5, 0.0, 0.6,
            1.0, 0.5, 0.0, 0.6,
        
            // �����棨��ɫ������͸����
            1.0, 0.7, 0.0, 0.4,
            1.0, 0.7, 0.0, 0.4,
            1.0, 0.7, 0.0, 0.4,
            1.0, 0.7, 0.0, 0.4,
            1.0, 0.7, 0.0, 0.4,
            1.0, 0.7, 0.0, 0.4,
        
            // �����棨��ɫ���ӽ���ȫ͸����
            1.0, 0.9, 0.0, 0.2,
            1.0, 0.9, 0.0, 0.2,
            1.0, 0.9, 0.0, 0.2,
            1.0, 0.9, 0.0, 0.2,
            1.0, 0.9, 0.0, 0.2,
            1.0, 0.9, 0.0, 0.2,
        
            // �����棨��ɫ����ȫ͸����
            1.0, 1.0, 0.0, 0.1,
            1.0, 1.0, 0.0, 0.1,
            1.0, 1.0, 0.0, 0.1,
            1.0, 1.0, 0.0, 0.1,
            1.0, 1.0, 0.0, 0.1,
            1.0, 1.0, 0.0, 0.1,
        };

        std::shared_ptr<VertexBuffer> Pos, UV, Normal, Color;
        Pos.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
        Color.reset(VertexBuffer::Create(colors, sizeof(colors)));
        UV.reset(VertexBuffer::Create(uvs, sizeof(uvs)));
        Normal.reset(VertexBuffer::Create(normals, sizeof(normals)));
        
        std::shared_ptr<IndexBuffer> index;
        index.reset(IndexBuffer::Create(indices, 36));

        Pos->SetLayout({{ShaderDataType::Float3, VertexType::Position, "pos"}});
        Color->SetLayout({{ShaderDataType::Float4, VertexType::Color, "color"}});
        UV->SetLayout({{ShaderDataType::Float2, VertexType::UV, "uv"}});
        Normal->SetLayout({{ShaderDataType::Float3, VertexType::Normal, "normal"}});

        shape->mVao.reset(VertexArray::Create());

        shape->mVao->AddVertexBuffer(Pos);
        shape->mVao->AddVertexBuffer(Color);
        shape->mVao->AddVertexBuffer(UV);
        shape->mVao->AddVertexBuffer(Normal);                     
        shape->mVao->SetIndexBuffer(index);
        return shape;
    }

    Shape *Shape::CreateSphere(float radius) {
        	Shape* shape = new Shape();

            std::vector<float> positions{};
            std::vector<float> uvs{};
            std::vector<unsigned int> indeices{};
            std::vector<float> normals{};
            std::vector<float> colors;
            int numLatLines = 160;
            int numLongLines = 160;

            for (int i = 0; i <= numLatLines; i++) {
                for (int j = 0; j <= numLongLines; j++) {
                    float phi = i* glm::pi<float>() / numLatLines;
                    float thea = j *2 * glm::pi<float>() / numLongLines;

                    float y = radius * cos(phi);
                    float x = radius * sin(phi) * cos(thea);
                    float z = radius * sin(phi) * sin(thea);

                    positions.push_back(x);
                    positions.push_back(y);
                    positions.push_back(z);

                    float u =  (float)j / (float) numLongLines;
                    float v =  1.0f - (float)i / (float)numLatLines;

                    uvs.push_back(u);
                    uvs.push_back(v);

                    normals.push_back(x);
                    normals.push_back(y);
                    normals.push_back(z);
                }
            }

            for (int i = 0; i < numLatLines; i++) {
                for (int j = 0; j < numLongLines; j++) {
                    int p1 = i * (numLongLines + 1) + j;
                    int p2 = p1 + numLongLines + 1;
                    int p3 = p1 + 1;
                    int p4 = p2 + 1;

                    indeices.push_back(p1);
                    indeices.push_back(p2);
                    indeices.push_back(p3);
                    

                    indeices.push_back(p3);
                    indeices.push_back(p2);
                    indeices.push_back(p4);
                }
            }
                
            for (int i = 0; i <= numLatLines; i++) {
                float t = (float)i / numLatLines; // 0~1
                for (int j = 0; j <= numLongLines; j++) {
                    colors.push_back(1.0f - t); // R
                    colors.push_back(0.5f + 0.5f * t); // G
                    colors.push_back(t); // B
                    colors.push_back(1.0f); // A
                }
            }
            
            std::shared_ptr<VertexBuffer> Pos, UV, Normal, Color;
            Pos.reset(VertexBuffer::Create(positions.data(), positions.size()*sizeof(float)));
            Color.reset(VertexBuffer::Create(colors.data(), colors.size()*sizeof(float)));
            UV.reset(VertexBuffer::Create(uvs.data(), uvs.size()*sizeof(float)));
            Normal.reset(VertexBuffer::Create(normals.data(), normals.size()*sizeof(float)));
            
            std::shared_ptr<IndexBuffer> index;
            index.reset(IndexBuffer::Create(indeices.data(), indeices.size()));

            Pos->SetLayout({{ShaderDataType::Float3, VertexType::Position, "pos"}});
            Color->SetLayout({{ShaderDataType::Float4, VertexType::Color, "color"}});
            UV->SetLayout({{ShaderDataType::Float2, VertexType::UV, "uv"}});
            Normal->SetLayout({{ShaderDataType::Float3, VertexType::Normal, "normal"}});

            shape->mVao.reset(VertexArray::Create());

            shape->mVao->AddVertexBuffer(Pos);
            shape->mVao->AddVertexBuffer(Color);
            shape->mVao->AddVertexBuffer(UV);
            shape->mVao->AddVertexBuffer(Normal);                     
            shape->mVao->SetIndexBuffer(index);
            return shape;
    }

    Shape *Shape::CreatePlan(float width, float height) {
            Shape* shape = new Shape();

            float hafw = width / 2.0;
            float hafh = height / 2.0;


            float positions[] = {
                -hafw, -hafh, 0.0f,
                hafw, -hafh, 0.0f,
                hafw, hafh, 0.0f,
                -hafw, hafh, 0.0f,
            };

            float colors[] = {
                1.0f, 1.0f, 1.0f, 1.0f, // ����
                1.0f, 1.0f, 1.0f, 1.0f, // ����
                1.0f, 1.0f, 1.0f, 1.0f, // ����
                1.0f, 1.0f, 1.0f, 1.0f  // ����
            };

            float normals[] = {
                0.0f, 0.0f, 1.0f, // ����
                0.0f, 0.0f, 1.0f, // ����
                0.0f, 0.0f, 1.0f, // ����
                0.0f, 0.0f, 1.0f  // ����
            };

            float uvs[] = {
                0.0, 0.0,
                1.0, 0.0,
                1.0, 1.0,
                0.0, 1.0
            };

            unsigned int indices[] = {
                0, 1, 2,
                2, 3, 0
            };

            std::shared_ptr<VertexBuffer> Pos, UV, Normal, Color;
            Pos.reset(VertexBuffer::Create(positions, sizeof(positions)));
            Color.reset(VertexBuffer::Create(colors, sizeof(colors)));
            UV.reset(VertexBuffer::Create(uvs, sizeof(uvs)));
            Normal.reset(VertexBuffer::Create(normals, sizeof(normals)));
            
            std::shared_ptr<IndexBuffer> index;
            index.reset(IndexBuffer::Create(indices, 6));

            Pos->SetLayout({{ShaderDataType::Float3, VertexType::Position, "pos"}});
            Color->SetLayout({{ShaderDataType::Float4, VertexType::Color, "color"}});
            UV->SetLayout({{ShaderDataType::Float2, VertexType::UV, "uv"}});
            Normal->SetLayout({{ShaderDataType::Float3, VertexType::Normal, "normal"}});

            shape->mVao.reset(VertexArray::Create());

            shape->mVao->AddVertexBuffer(Pos);
            shape->mVao->AddVertexBuffer(Color);
            shape->mVao->AddVertexBuffer(UV);
            shape->mVao->AddVertexBuffer(Normal);                     
            shape->mVao->SetIndexBuffer(index);
            return shape;
    }

    Shape* Shape::CreateRectangle(float width, float height) {
        Shape* shape = new Shape();

        float halfW = width / 2.0f;
        float halfH = height / 2.0f;

        // ����λ�� (8������)
        float positions[] = {
            // ǰ��
            -halfW, -halfH,  0.0,
            halfW, -halfH,  0.0,
            halfW,  halfH,  0.0,
            -halfW,  halfH,  0.0,
            
            // ����
            -halfW, -halfH, 0.0,
            halfW, -halfH, 0.0,
            halfW,  halfH, 0.0,
            -halfW,  halfH, 0.0
        };

        // ������ɫ (���ж����ɫ)
        float colors[] = {
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f
        };

        // ���� (ÿ����ķ��߷���)
        float normals[] = {
            // ǰ�淨��
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            
            // ���淨��
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            
            // ���淨�� (�����������ظ�ʹ�ö���)
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            
            // ���淨��
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            
            // ���淨��
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            
            // ���淨��
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f
        };

        // �������� (ÿ����ʹ����ͬ��UVӳ��)
        float uvs[] = {
            // ǰ��
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            
            // ����
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            
            // ����
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            
            // ����
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            
            // ����
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            
            // ����
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f
        };

        // ���� (36��������6�����2�������Ρ�3������)
        unsigned int indices[] = {
            // ǰ��
            0, 1, 2,
            2, 3, 0,
            
            // ����
            5, 4, 7,
            7, 6, 5,
            
            // ����
            4, 0, 3,
            3, 7, 4,
            
            // ����
            1, 5, 6,
            6, 2, 1,
            
            // ����
            3, 2, 6,
            6, 7, 3,
            
            // ����
            4, 5, 1,
            1, 0, 4
        };

        // �������㻺����������������
        std::shared_ptr<VertexBuffer> Pos, UV, Normal, Color;
        Pos.reset(VertexBuffer::Create(positions, sizeof(positions)));
        Color.reset(VertexBuffer::Create(colors, sizeof(colors)));
        UV.reset(VertexBuffer::Create(uvs, sizeof(uvs)));
        Normal.reset(VertexBuffer::Create(normals, sizeof(normals)));
        
        std::shared_ptr<IndexBuffer> index;
        index.reset(IndexBuffer::Create(indices, 36));

        // ���ö��㲼��
        Pos->SetLayout({{ShaderDataType::Float3, VertexType::Position, "pos"}});
        Color->SetLayout({{ShaderDataType::Float4, VertexType::Color, "color"}});
        UV->SetLayout({{ShaderDataType::Float2, VertexType::UV, "uv"}});
        Normal->SetLayout({{ShaderDataType::Float3, VertexType::Normal, "normal"}});

        // �����������鲢��ӻ�����
        shape->mVao.reset(VertexArray::Create());
        shape->mVao->AddVertexBuffer(Pos);
        shape->mVao->AddVertexBuffer(Color);
        shape->mVao->AddVertexBuffer(UV);
        shape->mVao->AddVertexBuffer(Normal);                     
        shape->mVao->SetIndexBuffer(index);
        
        return shape;
    }

    Shape* Shape::CreateTriangle2D(float size) {
    Shape* shape = new Shape();
    
    float halfSize = size / 2.0f;
    float vertices[] = {
        -halfSize, -halfSize,  // ����
        halfSize, -halfSize,   // ����
        0.0f, halfSize         // ����
    };

    float colors[] = {
        1.0f, 0.0f, 0.0f, 1.0f,    // ��
        0.0f, 1.0f, 0.0f, 1.0f,    // ��
        0.0f, 0.0f, 1.0f, 1.0f     // ��
    };

    float uvs[] = {
        0.0f, 0.0f,    // ����
        1.0f, 0.0f,    // ����
        0.5f, 1.0f     // ����
    };

    unsigned int indices[] = {0, 1, 2};

    std::shared_ptr<VertexBuffer> Pos, UV, Color;
    Pos.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
    Color.reset(VertexBuffer::Create(colors, sizeof(colors)));
    UV.reset(VertexBuffer::Create(uvs, sizeof(uvs)));
    
    std::shared_ptr<IndexBuffer> index;
    index.reset(IndexBuffer::Create(indices, 3));

    // ֻʹ��2D����
    Pos->SetLayout({{ShaderDataType::Float2, VertexType::Position, "pos"}});
    Color->SetLayout({{ShaderDataType::Float4, VertexType::Color, "color"}});
    UV->SetLayout({{ShaderDataType::Float2, VertexType::UV, "uv"}});

    shape->mVao.reset(VertexArray::Create());
    shape->mVao->AddVertexBuffer(Pos);
    shape->mVao->AddVertexBuffer(Color);
    shape->mVao->AddVertexBuffer(UV);                     
    shape->mVao->SetIndexBuffer(index);
    return shape;
}

Shape* Shape::CreateRectangle2D(float width, float height) {
    Shape* shape = new Shape();
    
    float halfW = width / 2.0f;
    float halfH = height / 2.0f;
    
    float vertices[] = {
        -halfW, -halfH,  // ����
        halfW, -halfH,   // ����
        halfW, halfH,    // ����
        -halfW, halfH    // ����
    };

    float colors[] = {
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };

    float uvs[] = {
        0.0f, 0.0f,    // ����
        1.0f, 0.0f,    // ����
        1.0f, 1.0f,    // ����
        0.0f, 1.0f     // ����
    };

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    std::shared_ptr<VertexBuffer> Pos, UV, Color;
    Pos.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
    Color.reset(VertexBuffer::Create(colors, sizeof(colors)));
    UV.reset(VertexBuffer::Create(uvs, sizeof(uvs)));
    
    std::shared_ptr<IndexBuffer> index;
    index.reset(IndexBuffer::Create(indices, 6));

    Pos->SetLayout({{ShaderDataType::Float2, VertexType::Position, "pos"}});
    Color->SetLayout({{ShaderDataType::Float4, VertexType::Color, "color"}});
    UV->SetLayout({{ShaderDataType::Float2, VertexType::UV, "uv"}});

    shape->mVao.reset(VertexArray::Create());
    shape->mVao->AddVertexBuffer(Pos);
    shape->mVao->AddVertexBuffer(Color);
    shape->mVao->AddVertexBuffer(UV);                     
    shape->mVao->SetIndexBuffer(index);
    return shape;
}

Shape* Shape::CreateCircle2D(float radius, int segments) {
    Shape* shape = new Shape();
    
    std::vector<float> positions;
    std::vector<float> colors;
    std::vector<float> uvs;
    std::vector<unsigned int> indices;
    
    // ���Ķ���
    positions.push_back(0.0f);
    positions.push_back(0.0f);
    
    colors.push_back(1.0f);
    colors.push_back(1.0f);
    colors.push_back(1.0f);
    colors.push_back(1.0f);
    
    uvs.push_back(0.5f);
    uvs.push_back(0.5f);
    
    // ����Բ���ϵĶ���
    for (int i = 0; i <= segments; i++) {
        float theta = 2.0f * glm::pi<float>() * float(i) / float(segments);
        float x = radius * cos(theta);
        float y = radius * sin(theta);
        
        positions.push_back(x);
        positions.push_back(y);
        
        colors.push_back(1.0f);
        colors.push_back(1.0f);
        colors.push_back(1.0f);
        colors.push_back(1.0f);
        
        uvs.push_back((x / radius + 1.0f) / 2.0f);
        uvs.push_back((y / radius + 1.0f) / 2.0f);
    }
    
    // ��������
    for (int i = 1; i <= segments; i++) {
        indices.push_back(0);       // ����
        indices.push_back(i + 1);
        indices.push_back(i);
    }
    // �պ�Բ
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(segments);

    std::shared_ptr<VertexBuffer> Pos, UV, Color;
    Pos.reset(VertexBuffer::Create(positions.data(), positions.size() * sizeof(float)));
    Color.reset(VertexBuffer::Create(colors.data(), colors.size() * sizeof(float)));
    UV.reset(VertexBuffer::Create(uvs.data(), uvs.size() * sizeof(float)));
    
    std::shared_ptr<IndexBuffer> index;
    index.reset(IndexBuffer::Create(indices.data(), indices.size()));

    Pos->SetLayout({{ShaderDataType::Float2, VertexType::Position, "pos"}});
    Color->SetLayout({{ShaderDataType::Float4, VertexType::Color, "color"}});
    UV->SetLayout({{ShaderDataType::Float2, VertexType::UV, "uv"}});

    shape->mVao.reset(VertexArray::Create());
    shape->mVao->AddVertexBuffer(Pos);
    shape->mVao->AddVertexBuffer(Color);
    shape->mVao->AddVertexBuffer(UV);                     
    shape->mVao->SetIndexBuffer(index);
    return shape;
}

Shape* Shape::CreateLine2D(float x1, float y1, float x2, float y2, float thickness) {
    Shape* shape = new Shape();
    
    // �����ߵķ�������
    float dx = x2 - x1;
    float dy = y2 - y1;
    
    // ���㴹ֱ����
    float nx = -dy;
    float ny = dx;
    float len = sqrt(nx*nx + ny*ny);
    nx /= len;
    ny /= len;
    
    // �����ĸ�����
    float halfThickness = thickness / 2.0f;
    float vertices[] = {
        x1 + nx * halfThickness, y1 + ny * halfThickness,  // ����1
        x1 - nx * halfThickness, y1 - ny * halfThickness,  // ����2
        x2 - nx * halfThickness, y2 - ny * halfThickness,  // ����3
        x2 + nx * halfThickness, y2 + ny * halfThickness   // ����4
    };

    float colors[] = {
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };

    float uvs[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    std::shared_ptr<VertexBuffer> Pos, UV, Color;
    Pos.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
    Color.reset(VertexBuffer::Create(colors, sizeof(colors)));
    UV.reset(VertexBuffer::Create(uvs, sizeof(uvs)));
    
    std::shared_ptr<IndexBuffer> index;
    index.reset(IndexBuffer::Create(indices, 6));

    Pos->SetLayout({{ShaderDataType::Float2, VertexType::Position, "pos"}});
    Color->SetLayout({{ShaderDataType::Float4, VertexType::Color, "color"}});
    UV->SetLayout({{ShaderDataType::Float2, VertexType::UV, "uv"}});

    shape->mVao.reset(VertexArray::Create());
    shape->mVao->AddVertexBuffer(Pos);
    shape->mVao->AddVertexBuffer(Color);
    shape->mVao->AddVertexBuffer(UV);                     
    shape->mVao->SetIndexBuffer(index);
    return shape;
}
}