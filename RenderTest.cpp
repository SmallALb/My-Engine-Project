#include "FISH.h"
#include "FISH/Editor/VertexEditor.h"
#include "FISH/Base/TaskPool.h"
#include "FISH/physics/Ray/Ray_Collider/RayAndCylinder.h"
#include "FISH/Renderer/Renderers/PhongLightRender.h"
#include "FISH/Object/Arrow.h"
class MainLayer : public FISH::Layer {
        //初始化
    void OnAttach() override {
        scene.reset(new FISH::Scene());
        //线程池
        pool.reset(new TaskPool(3));
        arrow.reset(new FISH::Arrow({0.0f, 1.0f, 0.0f}));
        arrow->setDirection({0.0f, 0.0f, 1.0f});
        arrow2.reset(new FISH::Arrow({1.0f, 0.0f, 0.0f}));
        arrow2->setDirection({1.0f, 0.0f, 0.0f});
        arrow3.reset(new FISH::Arrow({0.0f, 0.0f, 1.0f}));
        arrow3->setDirection({0.0f, 1.0f, 0.0f});

        //相机
        camera.reset(new FISH::perspectiveCamera("MainCamera"));
        Static_PtrCastTo<FISH::perspectiveCamera>(camera)->init(
          75.0f, APP.GetWindow().GetWidth()/APP.GetWindow().GetHeight(),
          0.1, 200.0f  
        );
        camera->setPosition({1.0, 1.0, -1.0});
        camera->setLookAt({0.0, 0.0, 0.0});

        //单向光
        auto dirlight =  std::make_shared<FISH::DirectionLight>();
        dirlight->setPosition({-1.0, 1.0, -1.0});
        dirlight->setLookAtPoint(glm::vec3{0.0});
        FISH::Renderer::setAmbiLight(dirlight);

        //模型1
        auto mesh1 = std::make_shared<FISH::Mesh>();
        auto shape1 = std::shared_ptr<FISH::Shape>(FISH::Shape::CreateBox(0.3));
        auto material = std::make_shared<FISH::Material>();
        material->setBlendColorTag(1);
        material->setDepthWriteTag(0);
        //尝试上纹理
        // FISH::TextureManager::get().loadTextureAsync("picture/window2.png", ChannelType::RGBA,
        // [material](FISH::TexturePtr texture) {
        //     material->setTexture(FISH::TextureType::None, texture);
        // });

        //尝试上动画
        auto mAni = std::make_shared<FISH::SpriteAnimation>("picture/sprite", "cat ({}).png", 21, 1, 56);
        mAni->play(FISH::AnimationMode::Loop);

        material->setTexture(FISH::TextureType::None, mAni);
        mesh1->getMaterial() = material;
        mesh1->getShape() = shape1;
        scene->addChild(mesh1);

        //设置清理
        mStatus.reset(FISH::Renderstatus::CreateRenderstatus());
        mStatus->enablestatus(FISH::StatusType::CleanDepth);
        mStatus->enablestatus(FISH::StatusType::CleanColor);
        APP.setClean([&]() {
            APP.GetWindow().CleanBuffer(mStatus->getCleanstatuss());
        });


        APP.GetWindow().SetCleanColor(0.3, 0.25, 0.25, 1.0);
        camera->setAllowedControl(1);

        FISH::GeomtryEditor::get()->setCurrentEditShape(shape1);

        //测试
        arrow->setCamera(camera);
        arrow2->setCamera(camera);
        arrow3->setCamera(camera);
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

        if (FISH::Input::IsKeyPressedOnce(FS_KEY_M)) FS_INFO("Sum is : {}", sum);
        if (FISH::Input::IsKeyPressedOnce(FS_KEY_C)) {
            auto res = pool->submit([this]()->void {
                for (int i=1; i<=100000000; i++) sum += i;
            });
        }
        if(FISH::Input::IsKeyPressedOnce(FS_KEY_L)) 
            !APP.getLockedState() ? APP.LockCursor() : APP.UnLockCursor();
        //camera->setLookAt({0.0, 0.0, 0.0});
        FISH::TextureManager::get().processAsyncUploads();
        FISH::Renderer::RenderInPhongMode(scene, camera);
        arrow->render();
        arrow2->render();
        arrow3->render();
        
        
    }

    std::shared_ptr<FISH::Scene>        scene;
    FISH::CameraPtr                     camera;
    std::shared_ptr<FISH::Renderstatus> mStatus;
    std::unique_ptr<TaskPool>           pool;
    std::shared_ptr<FISH::Arrow>        arrow, arrow2, arrow3;
    long long                           sum {0};


};

class Sandbox : public FISH::Application {
public:
    Sandbox() {
        FISH::Texture::initDefaultTexture();
        FISH::Renderer::DefaultShader::buildAlltheDefaultShader();
        PushLayer(new MainLayer());
        PushLayer(FISH::GeomtryEditor::get());
    }

    ~Sandbox() {}


};

FISH::Application* FISH::CreateApplication() {
    return new Sandbox();
}