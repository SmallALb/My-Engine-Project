#include "fspcs.h"
#include "FISH/Renderer/Buffer.h"
#include "FISH/Timer.h"
#include "FISH/Renderer/VertexArray.h"
#include "FISH/Renderer/RenderElement.h"
#include "FISH/Renderer/Texture.h"
#include "FISH/Renderer/Animation.h"
#include "BaseShape.h"

namespace FISH
{
    Shape::Shape() {
        mVao.reset(VertexArray::Create());
    }

    Shape::~Shape() {

    }

    Shape::Shape(std::shared_ptr<VertexBuffer> vertx, std::shared_ptr<IndexBuffer> index) {
        mVao.reset(VertexArray::Create());
        mVao->AddVertexBuffer(vertx);
        mVao->SetIndexBuffer(index);
    }

    Shape * Shape::CreateCone(float height, float radius, int subdivisions) {
        Shape* shape = new Shape();
        shape->mType = ShapeType::Cone;

        std::vector<float> positions;
        std::vector<float> colors;
        std::vector<float> uvs;
        std::vector<float> normals;
        std::vector<unsigned int> indices;

        float halfHeight = height / 2.0f;
        subdivisions = (std::max)(3, subdivisions);

        // 生成底面顶点
        // 底面中心顶点
        positions.push_back(0.0f);
        positions.push_back(-halfHeight);
        positions.push_back(0.0f);
        
        colors.push_back(0.7f);
        colors.push_back(0.7f);
        colors.push_back(0.7f);
        colors.push_back(1.0f);
        
        uvs.push_back(0.5f);
        uvs.push_back(0.5f);
        
        normals.push_back(0.0f);
        normals.push_back(-1.0f);
        normals.push_back(0.0f);

            // 底面圆周顶点
        for (int i = 0; i <= subdivisions; i++) {
            float theta = 2.0f * glm::pi<float>() * float(i) / float(subdivisions);
            float x = radius * cos(theta);
            float z = radius * sin(theta);
            
            positions.push_back(x);
            positions.push_back(-halfHeight);
            positions.push_back(z);
            
            colors.push_back(0.7f);
            colors.push_back(0.7f);
            colors.push_back(0.7f);
            colors.push_back(1.0f);
            
            uvs.push_back(0.5f + 0.5f * cos(theta));
            uvs.push_back(0.5f + 0.5f * sin(theta));
            
            normals.push_back(0.0f);
            normals.push_back(-1.0f);
            normals.push_back(0.0f);

        }

            // 生成顶点索引
        int bottomCenterIndex = 0;
        int bottomRingStartIndex = 1;
        int tipIndex = bottomRingStartIndex + (subdivisions + 1); // 顶点索引
        
        // 添加锥尖顶点
        positions.push_back(0.0f);
        positions.push_back(halfHeight);
        positions.push_back(0.0f);
        
        colors.push_back(1.0f);
        colors.push_back(1.0f);
        colors.push_back(1.0f);
        colors.push_back(1.0f);
        
        uvs.push_back(0.5f);
        uvs.push_back(0.5f);
        
        // 锥尖法线需要特殊计算，这里先设为向上
        normals.push_back(0.0f);
        normals.push_back(1.0f);
        normals.push_back(0.0f);

        // 添加侧面圆周顶点（与底面圆周顶点位置相同，但法线不同）
        int sideRingStartIndex = tipIndex + 1;
        for (int i = 0; i <= subdivisions; i++) {
            float theta = 2.0f * glm::pi<float>() * float(i) / float(subdivisions);
            float x = radius * cos(theta);
            float z = radius * sin(theta);
            
            positions.push_back(x);
            positions.push_back(-halfHeight);
            positions.push_back(z);
            
            colors.push_back(0.8f);
            colors.push_back(0.8f);
            colors.push_back(0.8f);
            colors.push_back(1.0f);
            
            uvs.push_back((float)i / subdivisions);
            uvs.push_back(0.0f);
            
            // 计算侧面法线（垂直于锥面）
            glm::vec3 sideNormal = glm::normalize(glm::vec3(x, 0.0f, z));
                normals.push_back(sideNormal.x);
                normals.push_back(sideNormal.y);
                normals.push_back(sideNormal.z);
        }

        // 生成底面三角形（顺时针从底面看）
        for (int i = 0; i < subdivisions; i++) {
            indices.push_back(bottomCenterIndex);
            indices.push_back(bottomRingStartIndex + i + 1);
            indices.push_back(bottomRingStartIndex + i);
        }

        // 生成侧面三角形
        for (int i = 0; i < subdivisions; i++) {
            indices.push_back(tipIndex);
            indices.push_back(sideRingStartIndex + i);
            indices.push_back(sideRingStartIndex + i + 1);
        }

        // 创建顶点缓冲区和索引缓冲区
        std::shared_ptr<VertexBuffer> Pos, UV, Normal, Color;
        Pos.reset(VertexBuffer::Create(positions.data(), positions.size() * sizeof(float)));
        Color.reset(VertexBuffer::Create(colors.data(), colors.size() * sizeof(float)));
        UV.reset(VertexBuffer::Create(uvs.data(), uvs.size() * sizeof(float)));
        Normal.reset(VertexBuffer::Create(normals.data(), normals.size() * sizeof(float)));
        
        std::shared_ptr<IndexBuffer> index;
        index.reset(IndexBuffer::Create(indices.data(), indices.size()));

        // 设置顶点布局
        Pos->SetLayout({{ShaderDataType::Float3, VertexType::Position, "pos"}});
        Color->SetLayout({{ShaderDataType::Float4, VertexType::Color, "color"}});
        UV->SetLayout({{ShaderDataType::Float2, VertexType::UV, "uv"}});
        Normal->SetLayout({{ShaderDataType::Float3, VertexType::Normal, "normal"}});

        // 创建顶点数组并添加缓冲区
        shape->mVao.reset(VertexArray::Create());
        shape->mVao->AddVertexBuffer(Pos);
        shape->mVao->AddVertexBuffer(Color);
        shape->mVao->AddVertexBuffer(UV);
        shape->mVao->AddVertexBuffer(Normal);                     
        shape->mVao->SetIndexBuffer(index);
        
        return shape;

    }

    uint32_t Shape::GetIndexCounts() const {return mVao->GetIndexCounts(); }

    std::unique_ptr<VertexArray> &Shape::getVertexArrary() {
        return mVao;
    }

    Shape* Shape::CreateBox(float size, int subdivisions) {
        Shape* shape = new Shape();
        shape->mType = ShapeType::Box;
        float halfSize = size / 2.0f;
        
        subdivisions = (std::max)(subdivisions, 1);

        std::vector<float> vertices;
        std::vector<float> colors;
        std::vector<float> uvs;
        std::vector<float> normals;
        std::vector<unsigned int> indices;

        int countInPreFace = (subdivisions + 1) * (subdivisions + 1);
        int currentVertices = countInPreFace * 6;
        int currentIndices = subdivisions * subdivisions * 6 * 6;

        for (int face = 0; face < 6; face++) {
            glm::vec3 normal, tangent, bitangent;
            bool reverseOrd = 0;
            switch (face) {
                case 0: normal = glm::vec3(0, 0, -1); tangent = glm::vec3(1, 0, 0); bitangent = glm::vec3(0, 1, 0); break; // 前
                case 1: normal = glm::vec3(0, 0, 1);  tangent = glm::vec3(-1, 0, 0); bitangent = glm::vec3(0, 1, 0); reverseOrd = 1; break; // 后
                case 2: normal = glm::vec3(-1, 0, 0); tangent = glm::vec3(0, 0, 1); bitangent = glm::vec3(0, 1, 0); break; // 左
                case 3: normal = glm::vec3(1, 0, 0);  tangent = glm::vec3(0, 0, -1); bitangent = glm::vec3(0, 1, 0); reverseOrd = 1; break; // 右
                case 4: normal = glm::vec3(0, 1, 0);  tangent = glm::vec3(1, 0, 0); bitangent = glm::vec3(0, 0, 1); break; // 上
                case 5: normal = glm::vec3(0, -1, 0); tangent = glm::vec3(1, 0, 0); bitangent = glm::vec3(0, 0, -1); reverseOrd = 1; break; // 下
            }
            for (int i=0; i<= subdivisions; i++) for (int j = 0; j<= subdivisions; j++) {
                float u = (float)j / subdivisions;
                float v = (float)i / subdivisions;

                glm::vec3 pos = -halfSize * normal 
                    + (u * 2.0f -1.0f) * halfSize * tangent
                    + (v * 2.0f -1.0f) * halfSize * bitangent;
                vertices.push_back(pos.x);
                vertices.push_back(pos.y);
                vertices.push_back(pos.z);

                uvs.push_back(u);
                uvs.push_back(v);


                colors.push_back(1.0f - (float)face / 6.0f);
                colors.push_back(0.1f + (float)face / 6.0f);
                colors.push_back(0.0f);
                colors.push_back(1.0f);

                normals.push_back(normal.x);
                normals.push_back(normal.y);
                normals.push_back(normal.z);

            }

            int baseIndex = face * countInPreFace;
            for (int i=0; i< subdivisions; i++) for (int j=0; j<subdivisions; j++) {
                int i0 = baseIndex + i * (subdivisions + 1) + j; //0
                int i1 = i0 + 1; //1
                int i2 = i0 + (subdivisions + 1); //3
                int i3 = i2 + 1; //4 

                if (!reverseOrd) {
                    // 标准顺时针顺序
                    indices.push_back(i0);
                    indices.push_back(i1);
                    indices.push_back(i2);
                    
                    indices.push_back(i1);
                    indices.push_back(i3);
                    indices.push_back(i2);
                } else {
                    // 反转顺序（仍然是顺时针，但三角形划分不同）
                    indices.push_back(i0);
                    indices.push_back(i2);
                    indices.push_back(i1);
                    
                    indices.push_back(i1);
                    indices.push_back(i2);
                    indices.push_back(i3);
                }

            }
        }

        std::shared_ptr<VertexBuffer> Pos, UV, Normal, Color;
        Pos.reset(VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float)));
        Color.reset(VertexBuffer::Create(colors.data(), colors.size() * sizeof(float)));
        UV.reset(VertexBuffer::Create(uvs.data(), uvs.size() * sizeof(float)));
        Normal.reset(VertexBuffer::Create(normals.data(), normals.size() * sizeof(float)));
        
        std::shared_ptr<IndexBuffer> index;
        index.reset(IndexBuffer::Create(indices.data(), indices.size()));

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

    Shape *Shape::CreateSphere(float radius, int subdivisions) {
        	Shape* shape = new Shape();
            shape->mType = ShapeType::Sphere;
            std::vector<float> positions{};
            std::vector<float> uvs{};
            std::vector<unsigned int> indeices{};
            std::vector<float> normals{};
            std::vector<float> colors;
            int numLatLines = subdivisions;
            int numLongLines = subdivisions;

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
            shape->mType = ShapeType::Plan;
            float hafw = width / 2.0;
            float hafh = height / 2.0;


            float positions[] = {
                -hafw, -hafh, 0.0f,
                hafw, -hafh, 0.0f,
                hafw, hafh, 0.0f,
                -hafw, hafh, 0.0f,
            };

            float colors[] = {
                1.0f, 1.0f, 1.0f, 1.0f, // 左下
                1.0f, 1.0f, 1.0f, 1.0f, // 右下
                1.0f, 1.0f, 1.0f, 1.0f, // 右上
                1.0f, 1.0f, 1.0f, 1.0f  // 左上
            };

            float normals[] = {
                0.0f, 0.0f, 1.0f, // 左下
                0.0f, 0.0f, 1.0f, // 右下
                0.0f, 0.0f, 1.0f, // 右上
                0.0f, 0.0f, 1.0f  // 左上
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
        shape->mType = ShapeType::Plan;
        float halfW = width / 2.0f;
        float halfH = height / 2.0f;

        // 顶点位置 (8个顶点)
        float positions[] = {
            // 前面
            -halfW, -halfH,  0.0,
            halfW, -halfH,  0.0,
            halfW,  halfH,  0.0,
            -halfW,  halfH,  0.0,
            
            // 后面
            -halfW, -halfH, 0.0,
            halfW, -halfH, 0.0,
            halfW,  halfH, 0.0,
            -halfW,  halfH, 0.0
        };

        // 顶点颜色 (所有顶点白色)
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

        // 法线 (每个面的法线方向)
        float normals[] = {
            // 前面法线
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            
            // 后面法线
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            
            // 左面法线 (将在索引中重复使用顶点)
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            
            // 右面法线
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            
            // 上面法线
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            
            // 下面法线
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f
        };

        // 纹理坐标 (每个面使用相同的UV映射)
        float uvs[] = {
            // 前面
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            
            // 后面
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            
            // 左面
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            
            // 右面
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            
            // 上面
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            
            // 下面
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f
        };

        // 索引 (36个索引，6个面×2个三角形×3个顶点)
        unsigned int indices[] = {
            // 前面
            0, 1, 2,
            2, 3, 0,
            
            // 后面
            5, 4, 7,
            7, 6, 5,
            
            // 左面
            4, 0, 3,
            3, 7, 4,
            
            // 右面
            1, 5, 6,
            6, 2, 1,
            
            // 上面
            3, 2, 6,
            6, 7, 3,
            
            // 下面
            4, 5, 1,
            1, 0, 4
        };

        // 创建顶点缓冲区和索引缓冲区
        std::shared_ptr<VertexBuffer> Pos, UV, Normal, Color;
        Pos.reset(VertexBuffer::Create(positions, sizeof(positions)));
        Color.reset(VertexBuffer::Create(colors, sizeof(colors)));
        UV.reset(VertexBuffer::Create(uvs, sizeof(uvs)));
        Normal.reset(VertexBuffer::Create(normals, sizeof(normals)));
        
        std::shared_ptr<IndexBuffer> index;
        index.reset(IndexBuffer::Create(indices, 36));

        // 设置顶点布局
        Pos->SetLayout({{ShaderDataType::Float3, VertexType::Position, "pos"}});
        Color->SetLayout({{ShaderDataType::Float4, VertexType::Color, "color"}});
        UV->SetLayout({{ShaderDataType::Float2, VertexType::UV, "uv"}});
        Normal->SetLayout({{ShaderDataType::Float3, VertexType::Normal, "normal"}});

        // 创建顶点数组并添加缓冲区
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
    shape->mType = ShapeType::Triangle2D;
    float halfSize = size / 2.0f;
    float vertices[] = {
        -halfSize, -halfSize,  // 左下
        halfSize, -halfSize,   // 右下
        0.0f, halfSize         // 上中
    };

    float colors[] = {
        1.0f, 0.0f, 0.0f, 1.0f,    // 红
        0.0f, 1.0f, 0.0f, 1.0f,    // 绿
        0.0f, 0.0f, 1.0f, 1.0f     // 蓝
    };

    float uvs[] = {
        0.0f, 0.0f,    // 左下
        1.0f, 0.0f,    // 右下
        0.5f, 1.0f     // 上中
    };

    unsigned int indices[] = {0, 1, 2};

    std::shared_ptr<VertexBuffer> Pos, UV, Color;
    Pos.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
    Color.reset(VertexBuffer::Create(colors, sizeof(colors)));
    UV.reset(VertexBuffer::Create(uvs, sizeof(uvs)));
    
    std::shared_ptr<IndexBuffer> index;
    index.reset(IndexBuffer::Create(indices, 3));

    // 只使用2D坐标
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
    shape->mType = ShapeType::Plan;    
    float halfW = width / 2.0f;
    float halfH = height / 2.0f;
    
    float vertices[] = {
        -halfW, -halfH,  // 左下
        halfW, -halfH,   // 右下
        halfW, halfH,    // 右上
        -halfW, halfH    // 左上
    };

    float colors[] = {
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };

    float uvs[] = {
        0.0f, 0.0f,    // 左下
        1.0f, 0.0f,    // 右下
        1.0f, 1.0f,    // 右上
        0.0f, 1.0f     // 左上
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
    shape->mType = ShapeType::Circle2D;
    std::vector<float> positions;
    std::vector<float> colors;
    std::vector<float> uvs;
    std::vector<unsigned int> indices;
    
    // 中心顶点
    positions.push_back(0.0f);
    positions.push_back(0.0f);
    
    colors.push_back(1.0f);
    colors.push_back(1.0f);
    colors.push_back(1.0f);
    colors.push_back(1.0f);
    
    uvs.push_back(0.5f);
    uvs.push_back(0.5f);
    
    // 生成圆周上的顶点
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
    
    // 生成索引
    for (int i = 1; i <= segments; i++) {
        indices.push_back(0);       // 中心
        indices.push_back(i + 1);
        indices.push_back(i);
    }
    // 闭合圆
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
    shape->mType = ShapeType::Line2D;
    // 计算线的方向向量
    float dx = x2 - x1;
    float dy = y2 - y1;
    
    // 计算垂直向量
    float nx = -dy;
    float ny = dx;
    float len = sqrt(nx*nx + ny*ny);
    nx /= len;
    ny /= len;
    
    // 计算四个顶点
    float halfThickness = thickness / 2.0f;
    float vertices[] = {
        x1 + nx * halfThickness, y1 + ny * halfThickness,  // 顶点1
        x1 - nx * halfThickness, y1 - ny * halfThickness,  // 顶点2
        x2 - nx * halfThickness, y2 - ny * halfThickness,  // 顶点3
        x2 + nx * halfThickness, y2 + ny * halfThickness   // 顶点4
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

Shape *Shape::CreateCylinder(float Height, float Radius, int subdivisions) {
    Shape* shape = new Shape();
    shape->mType = ShapeType::Cylinder;
    
    std::vector<float> positions;
    std::vector<float> colors;
    std::vector<float> uvs;
    std::vector<float> normals;
    std::vector<unsigned int> indices;

    float halfH = 0.5f * Height;
    subdivisions = (std::max)(3, subdivisions);
    //生成侧面
    for (int i=0; i<= subdivisions; i++) {
        float theta = 2.0f * glm::pi<float>() * float(i) / float(subdivisions);

        float x = Radius * cos(theta);
        float z = Radius * sin(theta);

        float nx = cos(theta);
        float nz = cos(theta);

        positions.push_back(x);
        positions.push_back(-halfH);
        positions.push_back(z);

        uvs.push_back((float)i / subdivisions);
        uvs.push_back(0.0f);

        normals.push_back(nx);
        normals.push_back(0.0f);
        normals.push_back(nz);
        
  
        positions.push_back(x);
        positions.push_back(halfH);
        positions.push_back(z);
        
        colors.push_back(0.8f);
        colors.push_back(0.8f);
        colors.push_back(0.8f);
        colors.push_back(1.0f);
        
        uvs.push_back((float)i / subdivisions);
        uvs.push_back(1.0f);
        
        normals.push_back(nx);
        normals.push_back(0.0f);
        normals.push_back(nz);

    }

    //生成侧面索引
    for (int i = 0; i < subdivisions; i++) {
        int base = i * 2;
        indices.push_back(base);
        indices.push_back(base + 2);
        indices.push_back(base + 1);
    
        indices.push_back(base + 2);
        indices.push_back(base + 3);
        indices.push_back(base + 1);
    }

    // 生成顶面和底面中心顶点
    // 顶面中心
    positions.push_back(0.0f);
    positions.push_back(halfH);
    positions.push_back(0.0f);
    colors.push_back(1.0f);
    colors.push_back(1.0f);
    colors.push_back(1.0f);
    colors.push_back(1.0f);
    uvs.push_back(0.5f);
    uvs.push_back(0.5f);
    normals.push_back(0.0f);
    normals.push_back(1.0f);
    normals.push_back(0.0f);

    // 顶面圆周顶点
    for (int i = 0; i <= subdivisions; i++) {
        float theta = 2.0f * glm::pi<float>() * float(i) / float(subdivisions);
        float x = Radius * cos(theta);
        float z = Radius * sin(theta);
        
        positions.push_back(x);
        positions.push_back(halfH);
        positions.push_back(z);
        colors.push_back(1.0f);
        colors.push_back(1.0f);
        colors.push_back(1.0f);
        colors.push_back(1.0f);
        uvs.push_back(0.5f + 0.5f * cos(theta));
        uvs.push_back(0.5f + 0.5f * sin(theta));
        normals.push_back(0.0f);
        normals.push_back(1.0f);
        normals.push_back(0.0f);
    }

    // 生成底面顶点 (从中心开始)
    // 底面中心
    positions.push_back(0.0f);
    positions.push_back(-halfH);
    positions.push_back(0.0f);
    colors.push_back(0.6f);
    colors.push_back(0.6f);
    colors.push_back(0.6f);
    colors.push_back(1.0f);
    uvs.push_back(0.5f);
    uvs.push_back(0.5f);
    normals.push_back(0.0f);
    normals.push_back(-1.0f);
    normals.push_back(0.0f);

    // 底面圆周顶点
    for (int i = 0; i <= subdivisions; i++) {
        float theta = 2.0f * glm::pi<float>() * float(i) / float(subdivisions);
        float x = Radius * cos(theta);
        float z = Radius * sin(theta);
        
        positions.push_back(x);
        positions.push_back(-halfH);
        positions.push_back(z);
        colors.push_back(0.6f);
        colors.push_back(0.6f);
        colors.push_back(0.6f);
        colors.push_back(1.0f);
        uvs.push_back(0.5f + 0.5f * cos(theta));
        uvs.push_back(0.5f + 0.5f * sin(theta));
        normals.push_back(0.0f);
        normals.push_back(-1.0f);
        normals.push_back(0.0f);
    }

    int sideVertexCount = (subdivisions + 1) * 2;
    int topBottomStartIndex = sideVertexCount;
    int topCenterIndex = topBottomStartIndex;
    int topRingStartIndex = topBottomStartIndex + 1;
    int bottomCenterIndex = topRingStartIndex + (subdivisions + 1);
    int bottomRingStartIndex = bottomCenterIndex + 1;

    // 生成顶面索引 (顺时针顺序)
    for (int i = 0; i < subdivisions; i++) {
        indices.push_back(topCenterIndex);
        indices.push_back(topRingStartIndex + i);
        indices.push_back(topRingStartIndex + i + 1);
    }

    // 生成底面索引 (顺时针顺序，从底面看)
    for (int i = 0; i < subdivisions; i++) {
        indices.push_back(bottomCenterIndex);
        indices.push_back(bottomRingStartIndex + i + 1);
        indices.push_back(bottomRingStartIndex + i);
    }

    // 创建顶点缓冲区和索引缓冲区
    std::shared_ptr<VertexBuffer> Pos, UV, Normal, Color;
    Pos.reset(VertexBuffer::Create(positions.data(), positions.size() * sizeof(float)));
    Color.reset(VertexBuffer::Create(colors.data(), colors.size() * sizeof(float)));
    UV.reset(VertexBuffer::Create(uvs.data(), uvs.size() * sizeof(float)));
    Normal.reset(VertexBuffer::Create(normals.data(), normals.size() * sizeof(float)));
    
    std::shared_ptr<IndexBuffer> index;
    index.reset(IndexBuffer::Create(indices.data(), indices.size()));

    // 设置顶点布局
    Pos->SetLayout({{ShaderDataType::Float3, VertexType::Position, "pos"}});
    Color->SetLayout({{ShaderDataType::Float4, VertexType::Color, "color"}});
    UV->SetLayout({{ShaderDataType::Float2, VertexType::UV, "uv"}});
    Normal->SetLayout({{ShaderDataType::Float3, VertexType::Normal, "normal"}});

    // 创建顶点数组并添加缓冲区
    shape->mVao.reset(VertexArray::Create());
    shape->mVao->AddVertexBuffer(Pos);
    shape->mVao->AddVertexBuffer(Color);
    shape->mVao->AddVertexBuffer(UV);
    shape->mVao->AddVertexBuffer(Normal);                     
    shape->mVao->SetIndexBuffer(index);
    
    return shape;
}
}