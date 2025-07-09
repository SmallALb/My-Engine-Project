#include "FISH.h"
//继承GameObject生成的AObj
class AObj : public FISH::GameObject {
public:
    AObj(const FISH::ColliderPtr& _bounds, const string& name = "GameObj") : GameObject(_bounds, name) {

    }

    ~AObj() {

    }

    void setObj(const std::shared_ptr<FISH::Object3D>& obj) {
        mObj = obj;
        mObj->setScale(mBounds->size());
        mObj->setPosition(mPosition);
    }

    void update(float dt) override {
        if (needUpdate) {
            mObj->setPosition(mPosition);
        }
        needUpdate = 0;
    }

    void onCollision(const std::shared_ptr<GameObject>& other) override {
        if (mBounds->getType() == FISH::ColliderType::OBB) FS_INFO("CAST: {0}, {1}, count:{2}", mName, other->getName(), ++cou);
    }

    std::shared_ptr<FISH::OBB> getOBB() const {
        if (mBounds->getType() == FISH::ColliderType::OBB)
            return PtrCastTo<FISH::OBB>(mBounds);
        return nullptr;
    }

    void rotateZ(float angle) {
        float currentAngleZ = mObj->getAngleZ();
            
        // 更新物体的旋转角度
        mObj->setAngleZ(currentAngleZ + angle);
        
        // 创建纯Z轴旋转矩阵（基于累计角度）
        glm::mat4 rotation = glm::rotate(
            glm::mat4(1.0f),
            glm::radians(currentAngleZ + angle),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );
        
        // 应用旋转到OBB（保持与物体同步）
        PtrCastTo<FISH::OBB>(mBounds)->setRotation(glm::mat3(rotation));
        
        needUpdate = true;
    }

    std::shared_ptr<FISH::Object3D> mObj{nullptr};

    int cou = 0;

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


        CTest.reset(new FISH::CollisionTest(std::make_shared<FISH::AABB>(glm::vec3(-100, -100, -100), glm::vec3(100, 100, 100))));

        auto shape = std::shared_ptr<FISH::Shape>(FISH::Shape::CreateBox(1.0));;
        auto mesh = std::make_shared<FISH::Mesh>();
        auto mesh2 = std::make_shared<FISH::Mesh>();
        auto mesh3 = std::make_shared<FISH::Mesh>();

        mesh->getShape() = shape;
        mesh2->getShape() = shape;
        mesh3->getShape() = shape;

        mGameObj.reset(new AObj(std::make_shared<FISH::OBB>(glm::vec3(1, 1, 1), glm::mat3(1.0f)), "11"));
        mGameObj->setObj(mesh);
        mGameObj->setPosition({-2, -1.8, -2});
        mGameObj2.reset(new AObj(std::make_shared<FISH::AABB>(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1)), "22"));
        mGameObj2->setObj(mesh2);
        mGameObj2->setPosition({3, 3, 3});
        mGameObj3.reset(new AObj(std::make_shared<FISH::AABB>(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1)), "33"));
        mGameObj3->setObj(mesh3);



        CTest->insert(mGameObj);
        CTest->insert(mGameObj2);
        CTest->insert(mGameObj3);

       // mCamera->setAllowedControl(1);

    }

    void OnUpdate() override {

        mCamera->update();
        CTest->update();
        CTest->check();
        using namespace FISH;
        Renderer::ShaderLib["OnlyColor"]->Begin();
        Renderer::ShaderLib["OnlyColor"]->setMat4("projection", mCamera->getProjectMatrix());
        Renderer::ShaderLib["OnlyColor"]->setMat4("view", mCamera->getViewMatrix());
        Renderer::ShaderLib["OnlyColor"]->setMat4("model", glm::mat4(1.0f));
        Renderer::ShaderLib["OnlyColor"]->setMat4("normat", mGameObj->mObj->getNormalMatrix());  
        Renderer::ShaderLib["OnlyColor"]->setVector3("InColor", {0.0, 0.0, 1.0});
        mGameObj->getOBB()->getVertices()->renderIndex(0, LINES);
        Renderer::ShaderLib["OnlyColor"]->End();

        if (FISH::Input::IsKeyPressed(FS_KEY_C)) mGameObj->rotateZ(0.01f);

        if (FISH::Input::IsKeyPressed(FS_KEY_SPACE) && lastPress == 0) {
            if (!APP.getLockedState()) APP.LockCursor(), mCamera->setAllowedControl(1);
            else APP.UnLockCursor(), mCamera->setAllowedControl(0);
        }

        lastPress =  FISH::Input::IsKeyPressed(FS_KEY_SPACE);


        //FS_INFO("{}", glm::to_string(mCamera->getProjectMatrix()));

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
    bool lastPress = 0;
    
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