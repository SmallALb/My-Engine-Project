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

    void Shape::setTexture(const std::shared_ptr<TextureHandle>& texture) {
        mTexture = texture;
    }

    string Shape::getAnimationName() const {
        if (getType() == TextureHandleType::Dynamic)
            return Static_PtrCastTo<SpriteAnimation>(mTexture)->getName();
        return "";
    }

    std::shared_ptr<TextureHandle> Shape::getTexutreHandle()
    {
        return mTexture;
    }

    int Shape::getAnimationBeginIndex() const {
        if (getType() == TextureHandleType::Dynamic)
            return Static_PtrCastTo<SpriteAnimation>(mTexture)->getAnimationIndex();
        return -1; 
    }

    int Shape::getAnimationDuration() const {
            if (getType() == TextureHandleType::Dynamic)
                return Static_PtrCastTo<SpriteAnimation>(mTexture)->getDuration();
            return -1;
    }

    int Shape::Animationsize() const {
             if (getType() == TextureHandleType::Dynamic)
                return Static_PtrCastTo<SpriteAnimation>(mTexture)->Size();
            return -1;
    }

    Shape* Shape::CreateBox(float size) {
        Shape* shape = new Shape();
        shape->mType = ShapeType::Box;
        float halfSize = size / 2.0f;
        //位置
        float vertices[] = {
            // 前面（-Z）

            -halfSize, -halfSize, -halfSize,   // v0
            halfSize, -halfSize, -halfSize,    // v1
            halfSize,  halfSize, -halfSize,     // v2
            -halfSize, -halfSize, -halfSize,    // v0 (重复)
            halfSize,  halfSize, -halfSize,     // v2 (重复)
            -halfSize,  halfSize, -halfSize,    // v3

            // 后面（+Z）

            -halfSize, -halfSize,  halfSize,   // v4
            -halfSize,  halfSize,  halfSize,   // v5
            halfSize,  halfSize,  halfSize,    // v6
            -halfSize, -halfSize,  halfSize,   // v4 (重复)
            halfSize,  halfSize,  halfSize,    // v6 (重复)
            halfSize, -halfSize,  halfSize,    // v7

            // 左面（-X）

            -halfSize,  halfSize,  halfSize,   // v8
            -halfSize, -halfSize,  halfSize,   // v9
            -halfSize, -halfSize, -halfSize,   // v10
            -halfSize,  halfSize,  halfSize,   // v8 (重复)
            -halfSize, -halfSize, -halfSize,   // v10 (重复)
            -halfSize,  halfSize, -halfSize,   // v11

            // 右面（+X）

            halfSize,  halfSize, -halfSize,    // v12
            halfSize, -halfSize, -halfSize,    // v13
            halfSize, -halfSize,  halfSize,    // v14
            halfSize,  halfSize, -halfSize,    // v12 (重复)
            halfSize, -halfSize,  halfSize,    // v14 (重复)
            halfSize,  halfSize,  halfSize,    // v15

            // 上面（+Y）

            -halfSize,  halfSize, -halfSize,   // v16
            halfSize,  halfSize, -halfSize,    // v17
            halfSize,  halfSize,  halfSize,    // v18
            -halfSize,  halfSize, -halfSize,   // v16 (重复)
            halfSize,  halfSize,  halfSize,    // v18 (重复)
            -halfSize,  halfSize,  halfSize,   // v19

            // 下面（-Y）

            -halfSize, -halfSize, -halfSize,   // v20
            -halfSize, -halfSize,  halfSize,   // v21
            halfSize, -halfSize,  halfSize,    // v22
            -halfSize, -halfSize, -halfSize,   // v20 (重复)
            halfSize, -halfSize,  halfSize,    // v22 (重复)
            halfSize, -halfSize, -halfSize,    // v23             
        };

        // 定义uv坐标数组
        float uvs[] = {
       // 前面

            0.0f, 0.0f,   // v0
            1.0f, 0.0f,   // v1
            1.0f, 1.0f,   // v2
            0.0f, 0.0f,   // v0
            1.0f, 1.0f,   // v2
            0.0f, 1.0f,   // v3

            // 后面

            0.0f, 0.0f,   // v4
            0.0f, 1.0f,   // v5
            1.0f, 1.0f,   // v6
            0.0f, 0.0f,   // v4
            1.0f, 1.0f,   // v6
            1.0f, 0.0f,   // v7

            // 左面

            1.0f, 0.0f,   // v8
            0.0f, 0.0f,   // v9
            0.0f, 1.0f,   // v10
            1.0f, 0.0f,   // v8
            0.0f, 1.0f,   // v10
            1.0f, 1.0f,   // v11

            // 右面

            1.0f, 0.0f,   // v12
            0.0f, 0.0f,   // v13
            0.0f, 1.0f,   // v14
            1.0f, 0.0f,   // v12
            0.0f, 1.0f,   // v14
            1.0f, 1.0f,   // v15

            // 上面

            0.0f, 1.0f,   // v16
            1.0f, 1.0f,   // v17
            1.0f, 0.0f,   // v18
            0.0f, 1.0f,   // v16
            1.0f, 0.0f,   // v18
            0.0f, 0.0f,   // v19

            // 下面

            0.0f, 1.0f,   // v20
            0.0f, 0.0f,   // v21
            1.0f, 0.0f,   // v22
            0.0f, 1.0f,   // v20
            1.0f, 0.0f, 
            1.0f, 1.0f,   // v23
        };

        //法线
        float normals[] = {
            // 前面（-Z）

            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,

            // 后面（+Z）

            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,

            // 左面（-X）

            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,

            // 右面（+X）

            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,

            // 上面（+Y）

            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            // 下面（-Y）

            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
        };

        // 定义索引数组（每面4个顶点，每个三角形的索引是连续的）
        unsigned int indices[] = {
            0, 1, 2, 3, 4, 5,
            6, 7, 8, 9, 10, 11,
            12,13,14,15,16,17,
            18,19,20,21,22,23,
            24,25,26,27,28,29,
            30,31,32,33,34,35
        };

        //颜色
        float colors[] = {
            // 第一面（红色，完全不透明）
            1.0, 0.1, 0.0, 1.0,
            1.0, 0.1, 0.0, 1.0,
            1.0, 0.1, 0.0, 1.0,
            1.0, 0.1, 0.0, 1.0,
            1.0, 0.1, 0.0, 1.0,
            1.0, 0.1, 0.0, 1.0,
        
            // 第二面（红色，稍微透明）
            1.0, 0.3, 0.0, 0.8,
            1.0, 0.3, 0.0, 0.8,
            1.0, 0.3, 0.0, 0.8,
            1.0, 0.3, 0.0, 0.8,
            1.0, 0.3, 0.0, 0.8,
            1.0, 0.3, 0.0, 0.8,
        
            // 第三面（红色，透明度降低）
            1.0, 0.5, 0.0, 0.6,
            1.0, 0.5, 0.0, 0.6,
            1.0, 0.5, 0.0, 0.6,
            1.0, 0.5, 0.0, 0.6,
            1.0, 0.5, 0.0, 0.6,
            1.0, 0.5, 0.0, 0.6,
        
            // 第四面（红色，更加透明）
            1.0, 0.7, 0.0, 0.4,
            1.0, 0.7, 0.0, 0.4,
            1.0, 0.7, 0.0, 0.4,
            1.0, 0.7, 0.0, 0.4,
            1.0, 0.7, 0.0, 0.4,
            1.0, 0.7, 0.0, 0.4,
        
            // 第五面（红色，接近完全透明）
            1.0, 0.9, 0.0, 0.2,
            1.0, 0.9, 0.0, 0.2,
            1.0, 0.9, 0.0, 0.2,
            1.0, 0.9, 0.0, 0.2,
            1.0, 0.9, 0.0, 0.2,
            1.0, 0.9, 0.0, 0.2,
        
            // 第六面（红色，完全透明）
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
            shape->mType = ShapeType::Sphere;
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
}