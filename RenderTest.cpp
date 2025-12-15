#include "FISH.h"
#include "FISH/Editor/VertexEditor.h"
#include "FISH/Base/TaskPool.h"
#include "FISH/physics/Ray/Ray_Collider/RayAndCylinder.h"
#include "FISH/Renderer/Renderers/PhongLightRender.h"
#include "FISH/Object/Arrow.h"
#include "FISH/Renderer/ModelImporter/ModelManager.h"
#include "FISH/Renderer/RendererImport.h"

class MainLayer : public FISH::Layer {
    //初始化
    void OnAttach() override {
        FISH::Renderer::DefaultShader::buildAlltheDefaultShader();
        
        auto& vertexSystem = *FISH::BufferSystem::GetInstance();
        
        // 创建二维平面（使用ECS系统）
        float halfW = 1.0f;
        float halfH = 1.0f;
        
        // 顶点数据
        std::vector<float> positions = {
            -halfW, -halfH,  // 左下角
             halfW, -halfH,  // 右下角
             halfW,  halfH,  // 右上角
            -halfW,  halfH,  // 左上角
        };

        FISH::BufferData pos{
            .type = FISH::BufferType::VERTEX,
            .data = FISH::BufferData::VData{
                positions, 
                {FISH::BufferData::Vlayout{FISH::InputDataType::Float2, 0}}
            }
        };
        uint32_t Posentity = vertexSystem.createBuffer(pos);
        
        // 纹理坐标
        std::vector<float> texCoords = {
            0.0f, 0.0f,  // 左下角
            1.0f, 0.0f,  // 右下角
            1.0f, 1.0f,  // 右上角
            0.0f, 1.0f   // 左上角
        };

        FISH::BufferData uv{
            .type = FISH::BufferType::VERTEX,
            .data = FISH::BufferData::VData{
                texCoords, 
                {FISH::BufferData::Vlayout{FISH::InputDataType::Float2, 0}}
            }
        };

        uint32_t UVentity = vertexSystem.createBuffer(uv);

        std::vector<float> colors = {
            1.0f, 0.0f, 0.0f, 0.0f,  // 左下角
            1.0f, 0.0f, 0.0f, 0.0f, // 右下角
            1.0f, 0.0f, 0.0f, 0.0f,  // 右上角
            1.0f, 0.0f, 0.0f, 0.0f  // 左上角
        };
        
        FISH::BufferData color{
            .type = FISH::BufferType::VERTEX,
            .data = FISH::BufferData::VData{
                colors, 
                {FISH::BufferData::Vlayout{FISH::InputDataType::Float4, 0}}
            }
        };

        uint32_t Colorentity = vertexSystem.createBuffer(color);
        
        // 索引数据
        std::vector<uint32_t> indices = {
            0, 1, 2,  // 第一个三角形
            0, 2, 3   // 第二个三角形
        };

        FISH::BufferData index{
            .type = FISH::BufferType::INDEX,
            .data = indices
        };

        uint32_t Indexentity = vertexSystem.createBuffer(index);


        auto& VertexArraySystem = *FISH::VertexArraySystem::GetInstance();
        mVertexArrayEntity = VertexArraySystem.create();
        VertexArraySystem.addBuffer(mVertexArrayEntity, {Posentity, Colorentity, UVentity});
        VertexArraySystem.setIndex(mVertexArrayEntity, Indexentity);


        // 加载纹理
        auto& textureSystem = *FISH::TextureSystem::GetInstance();

        textureSystem.create("picture/Name.jpg", FISH::TextureLoadType::TEXTURE2D, [this](uint32_t entity) {mTextures.push_back(entity);});
        textureSystem.create("picture/Test.png", FISH::TextureLoadType::TEXTURE2D, [this](uint32_t entity) {mTextures.push_back(entity);});
        textureSystem.create("picture/fo.jpg", FISH::TextureLoadType::TEXTURE2D, [this](uint32_t entity) {mTextures.push_back(entity);});
        textureSystem.create("picture/sh.jpg", FISH::TextureLoadType::TEXTURE2D, [this](uint32_t entity) {mTextures.push_back(entity);});
        textureSystem.create("picture/Sky3.png", FISH::TextureLoadType::TEXTURE2D, [this](uint32_t entity) {mTextures.push_back(entity);});
        textureSystem.create("picture/Sky.png", FISH::TextureLoadType::TEXTURE2D, [this](uint32_t entity) {mTextures.push_back(entity);});

        FISH::ShaderSystem::GetInstance()->create("sharders/EnginRenderShader/NormalFragment CPY.glsl", FISH::ShaderType::FRAGMENT);
    }
    
    //销毁
    void OnDetach() override {}
    
    //更新
    void OnUpdate(float dt) override {
        APP.GetWindow().CleanBuffer(0x00000100 | 0x00004000 | 0x00000400);
        
        if (FISH::Input::IsKeyPressedOnce(FS_KEY_R)) {
            mRender = !mRender;
            FS_INFO("Render toggled: {}", mRender);
        }
        
        if (mRender) {
            testFunc();
        }
    }

    void testFunc() {
        auto textureSystem = FISH::TextureSystem::GetInstance();
        if (mTextures.empty()) {
            FS_WARN("Texture not loaded yet");
            return;
        }
        
        // 使用顶点数组系统绑定VAO
        if (mVertexArrayEntity != static_cast<uint32_t>(-1)) {
            auto vertexArraySystem = FISH::VertexArraySystem::GetInstance();
            vertexArraySystem->bind(mVertexArrayEntity);
            //FS_INFO("Bound vertex array: {}", mVertexArrayEntity);
        }
        
        // 使用默认纹理着色器
        auto shader = FISH::Renderer::DefaultShader::getShaderFromName(FISH::Renderer::DefaultShaderName::Texture2D);
        if (!shader) {
            FS_ERROR("Texture2D shader not found");
            return;
        }
        
        // 更新纹理索引
        if (FISH::Input::IsKeyPressedOnce(FS_KEY_PAGE_UP)) {
            mIndex = (mIndex + 1) % mTextures.size();
            FS_INFO("Texture index changed to: {}", mIndex);
        }
        
        // 删除纹理测试
        if (FISH::Input::IsKeyPressedOnce(FS_KEY_DELETE)) {
            textureSystem->destoryEntity(mTextures[mIndex]);
            FS_INFO("Destroyed texture at index: {}", mIndex);
        }
        
        // 渲染
        shader->Begin();
        textureSystem->bindHandle(mTextures[mIndex]);
        shader->setInt("uTexture", 0);
        
        FISH::VertexArrayCreator::Draw(0, 6, TRIANGLES); // 6个顶点（2个三角形）
        
        shader->End();
    }
    
private:
    bool mRender = true;
    std::vector<uint32_t> mTextures;
    int mIndex = 0;
    uint32_t mVertexArrayEntity = -1;
};

class Sandbox : public FISH::Application {
public:
    Sandbox() {
        // 按顺序推送系统层
        PushLayer(FISH::FileSystem::GetInstance());
        PushLayer(FISH::BufferSystem::GetInstance());
        PushLayer(FISH::VertexArraySystem::GetInstance());
        PushLayer(FISH::TextureSystem::GetInstance());
        PushLayer(FISH::ShaderSystem::GetInstance());
        PushLayer(new MainLayer());
    }

    ~Sandbox() {}
};

FISH::Application* FISH::CreateApplication() {
    return new Sandbox();
}
