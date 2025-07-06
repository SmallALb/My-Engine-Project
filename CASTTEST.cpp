#include "FISH.h"
//继承GameObject生成的AObj
class AObj : public FISH::GameObject {
public:
    AObj(const FISH::AABB& _bounds, const string& name = "GameObj") : GameObject(_bounds, name) {

    }

    ~AObj() {

    }

    void setObj(const std::shared_ptr<FISH::Object3D>& obj) {
        mObj = obj;
        mObj->setScale(mBounds.size());
        mObj->setPosition(mPosition);
    }

    void update(float dt) override {
        if (needUpdate) {
            mObj->setPosition(mPosition);
        }
        needUpdate = 0;
    }

    void onCollision(const std::shared_ptr<GameObject>& other) override {
        FS_INFO("CAST: {0}, {1}", mName, other->getName());
    }

    std::shared_ptr<FISH::Object3D> mObj{nullptr};

};

class MainLayer : public FISH::Layer {
    void OnAttach() override {
        shader.reset(FISH::Shader::CreateShader());

        shader->readVertexShader("sharders/EnginRenderShader/2Dvertex.glsl");
        shader->readFragmentShader("sharders/EnginRenderShader/OnlyColor.glsl");
        Shape2D.reset(FISH::Shape::CreateCircle2D(0.005f));
        auto light = std::shared_ptr<FISH::DirectionLight>(new FISH::DirectionLight());
        light->setLightOn(1);
        light->setPosition({0.0, 4.0, 0.0});
        light->setLookAtPoint({0.0, 0.0, 0.0});
        light->setColor({1.0, 1.0, 1.0});
        FISH::Renderer::setUseAmbientLight(light);

        mCamera.reset(new FISH::perspectiveCamera("camera"));
        mCamera->init(
            60.f, 
            (float)(APP.GetWindow().GetWidth()/APP.GetWindow().GetHeight()),
            0.1f,
            1000.f
        );

        FISH::Renderer::setUseCamera(mCamera);
        mCamera->setPosition({0.0, 0.0, -13.0});
        mCamera->setLookAt({0.0, 0.0, 0.0});

        //设置基本渲染状态
        mRenderstatuss.reset(FISH::Renderstatus::CreateRenderstatus());
        mRenderstatuss->enablestatus(FISH::StatusType::CleanColor);
        mRenderstatuss->enablestatus(FISH::StatusType::CleanDepth);
        mRenderstatuss->enablestatus(FISH::StatusType::DepthTest);
        mRenderstatuss->setstatusFunc(FISH::SetType::DepthFunc, FISH::FuncType::DepthLess);

        //设置清理APP函数
        APP.setClean([this]() {
            APP.GetWindow().CleanBuffer(mRenderstatuss->getCleanstatuss());
        });


        CTest.reset(new FISH::CollisionTest({{-100, -100, -100}, {100, 100, 100}}));

        auto shape = std::shared_ptr<FISH::Shape>(FISH::Shape::CreateBox(1.0));;
        auto mesh = std::make_shared<FISH::Mesh>();
        auto mesh2 = std::make_shared<FISH::Mesh>();
        auto mesh3 = std::make_shared<FISH::Mesh>();

        mesh->getShape() = shape;
        mesh2->getShape() = shape;
        mesh3->getShape() = shape;

        mGameObj.reset(new AObj({{-1, -1, -1}, {1, 1, 1}}, "11"));
        mGameObj->setObj(mesh);
        mGameObj->setPosition({-2, -2, -2});
        mGameObj2.reset(new AObj({{-1, -1, -1}, {1, 1, 1}}, "22"));
        mGameObj2->setObj(mesh2);
        mGameObj2->setPosition({3, 3, 3});
        mGameObj3.reset(new AObj({{-1, -1, -1}, {1, 1, 1}}, "33"));
        mGameObj3->setObj(mesh3);

        CTest->insert(mGameObj);
        CTest->insert(mGameObj2);
        CTest->insert(mGameObj3);

        //mCamera->setAllowedControl(1);

    }

    void OnUpdate() override {
        mCamera->update();
        CTest->update();
        CTest->check();

        //FS_INFO("{}", glm::to_string(mCamera->getFront()));

        std::vector<std::shared_ptr<FISH::Object3D>> renderObjs;

        renderObjs = {mGameObj->mObj, mGameObj2->mObj, mGameObj3->mObj};

        FISH::Renderer::render(renderObjs);

        //CTest->cleanUp(0);
    }

    std::unique_ptr<FISH::CollisionTest> CTest;
    std::shared_ptr<AObj> mGameObj;
    std::shared_ptr<AObj> mGameObj2;
    std::shared_ptr<AObj> mGameObj3;
    std::unique_ptr<FISH::Renderstatus> mRenderstatuss;
    std::shared_ptr<FISH::perspectiveCamera> mCamera;
    std::shared_ptr<FISH::Shape> Shape2D;
    std::shared_ptr<FISH::Shader> shader;

    
};


class Sandbox : public FISH::Application {
public:
    Sandbox() {
        FISH::Texture::initDefaultTexture();
        FISH::Renderer::initDefaultShader();
        PushLayer(new MainLayer());
    }

    ~Sandbox() {}


};

FISH::Application* FISH::CreateApplication() {
    return new Sandbox();
}