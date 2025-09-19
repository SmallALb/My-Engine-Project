#include "FISH.h"
#include "FISH/Editor/VertexEditor.h"

class MainLayer : public FISH::Layer {
        //初始化
    void OnAttach() override {
        //相机
        camera.reset(new FISH::perspectiveCamera("MainCamera"));
        Static_PtrCastTo<FISH::perspectiveCamera>(camera)->init(
          75.0f, APP.GetWindow().GetWidth()/APP.GetWindow().GetHeight(),
          0.1, 200.0f  
        );
        camera->setPosition({1.0, 1.0, -1.0});
        camera->setLookAt({0.0, 0.0, 0.0});
        FISH::Renderer::setUseCamera(camera);

        //单向光
        auto dirlight =  std::make_shared<FISH::DirectionLight>();
        dirlight->setPosition({-1.0, 1.0, -1.0});
        dirlight->setLookAtPoint(glm::vec3{0.0});
        FISH::Renderer::setUseAmbientLight(dirlight);

        //模型1
        auto mesh1 = std::make_shared<FISH::Mesh>();
        auto shape1 = std::shared_ptr<FISH::Shape>(FISH::Shape::CreateBox(1.0));
        auto material = std::make_shared<FISH::Material>();
        //material->setBlendColorTag(1);
        //material->setDepthWriteTag(0);
        //尝试上纹理
        // FISH::TextureManager::get().loadTextureAsync("picture/window2.png", ChannelType::RGBA,
        // [material](FISH::TexturePtr texture) {
        //     material->setTexture(FISH::TextureType::None, texture);
        // });

        // //尝试上动画
        // auto mAni = std::make_shared<FISH::SpriteAnimation>("picture/sprite", "cat ({}).png", 21, 1, 56);
        // mAni->play(FISH::AnimationMode::Loop);

        //material->setTexture(FISH::TextureType::None, mAni);
        mesh1->getMaterial() = material;
        mesh1->getShape() = shape1;
        objs.push_back(mesh1);

        //设置清理
        mStatus.reset(FISH::Renderstatus::CreateRenderstatus());
        mStatus->enablestatus(FISH::StatusType::CleanDepth);
        mStatus->enablestatus(FISH::StatusType::CleanColor);
        APP.setClean([&]() {
            APP.GetWindow().CleanBuffer(mStatus->getCleanstatuss());
        });

        shape1->getVertexArrary()->UpdateVertexPosition(0, {-0.7, -0.7, -0.7});

        APP.GetWindow().SetCleanColor(0.3, 0.25, 0.25, 1.0);
        //camera->setAllowedControl(1);
    }
    //销毁
    void OnDetach() override {}
    //更新
    void OnUpdate(float dt) override {
        camera->update();
        if (FISH::Input::IsKeyPressedOnce(FS_KEY_1)) camera->setPosition({1.0, 1.0, -1.0});
        else if (FISH::Input::IsKeyPressedOnce(FS_KEY_2)) camera->setPosition({-1.0, 1.0, -1.0});
        else if (FISH::Input::IsKeyPressedOnce(FS_KEY_3)) camera->setPosition({1.0, 1.0, 1.0});
        else if (FISH::Input::IsKeyPressedOnce(FS_KEY_4)) camera->setPosition({-1.0, 1.0, 1.0});

        if(FISH::Input::IsKeyPressedOnce(FS_KEY_L)) 
            !APP.getLockedState() ? APP.LockCursor() : APP.UnLockCursor();
        //camera->setLookAt({0.0, 0.0, 0.0});
        FISH::TextureManager::get().processAsyncUploads();
        FISH::Renderer::render(objs);


    }

    std::vector<FISH::Object3DPtr> objs;
    FISH::CameraPtr camera;
    std::shared_ptr<FISH::Renderstatus>          mStatus;

};

class Sandbox : public FISH::Application {
public:
    Sandbox() {
        FISH::Texture::initDefaultTexture();
        FISH::Renderer::initDefaultShader();
        PushLayer(new MainLayer());
        PushLayer(FISH::GeomtryEditor::get());
    }

    ~Sandbox() {}


};

FISH::Application* FISH::CreateApplication() {
    return new Sandbox();
}