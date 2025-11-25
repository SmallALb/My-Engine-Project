#include "FISH.h"
#include "FISH/Editor/VertexEditor.h"
#include "FISH/Base/TaskPool.h"
#include "FISH/physics/Ray/Ray_Collider/RayAndCylinder.h"
#include "FISH/Renderer/Renderers/PhongLightRender.h"
#include "FISH/Object/Arrow.h"
#include "FISH/Renderer/ModelImporter/ModelManager.h"
#include "FISH/Renderer/GPU/TextureCreator.h"
#include "FISH/Renderer/S/TextureSystem.h"

class MainLayer : public FISH::Layer {
        //初始化
    void OnAttach() override {
        FISH::Renderer::DefaultShader::buildAlltheDefaultShader();
        FramePlan = std::shared_ptr<FISH::Shape>(FISH::Shape::CreateRectangle2D(2.0f, 2.0f));

        auto system = FISH::TextureSystem::GetInstance();
        system->create({"picture/Name.jpg", "picture/Test.png", "picture/fo.jpg", "picture/sh.jpg", "picture/Sky3.png", "picture/Sky.png"}, FISH::TextureLoadType::TEXTURE2D, 
            [this](uint32_t entity) {
                FrameTex = entity;
            }
        );
    }
    //销毁
    void OnDetach() override {}
    //更新
    void OnUpdate(float dt) override {
        APP.GetWindow().CleanBuffer(0x00000100 | 0x00004000 | 0x00000400);
        if (FISH::Input::IsKeyPressedOnce(FS_KEY_R)) render = !render;
        if (render) testFunc();
    }

    void testFunc() {
        auto system = FISH::TextureSystem::GetInstance();
        if (FrameTex == -1) return;
        
        auto shader = FISH::Renderer::DefaultShader::getShaderFromName(FISH::Renderer::DefaultShaderName::Texture2D);
        index = (index + FISH::Input::IsKeyPressedOnce(FS_KEY_PAGE_UP)) % 6;
        if (FISH::Input::IsKeyPressedOnce(FS_KEY_DELETE)) system->destoryTexture(FrameTex, index);
        shader->Begin();
        system->bindHandle(FrameTex, index);
        shader->setInt("uTexture", 0);
        FramePlan->render(TRIANGLES);
        shader->End();

    }
    bool render = false;
    int32_t FrameTex = -1;
    int index = 0;
    FISH::ShapePtr FramePlan;
};

class Sandbox : public FISH::Application {
public:
    Sandbox() {
        PushLayer(FISH::TextureSystem::GetInstance());
        PushLayer(new MainLayer());
    }

    ~Sandbox() {}
};

FISH::Application* FISH::CreateApplication() {
    return new Sandbox();
}