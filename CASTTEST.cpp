#include "FISH.h"
#include <algorithm>
#include <ranges>
#include "FISH/Editor/ColliderEditor.h"
//继承GameObject生成的AObj
class AObj : public FISH::GameObject {
public:
    AObj(const FISH::ColliderPtr& _bounds, const string& name = "GameObj") : GameObject(_bounds, name) {

    }

    ~AObj() {

    }

    void setObj(const std::shared_ptr<FISH::Mesh>& obj) {
        mObj = obj;
        mObj->setScale(mBounds->size());
        mObj->setPosition(mPosition);
    }

    void addPos(const glm::vec3& dir) {
        setPosition(mPosition + dir);
    }

    void update(float dt) override {
        if (needUpdate) {
            mObj->setPosition(mPosition);
        }
        needUpdate = 0;
        hasCollided = 0;
    }

    void onCollision(const std::shared_ptr<GameObject>& other) override {
        if (mBounds->getType() == FISH::ColliderType::OBB && !hasCollided) {
            FS_INFO("CAST: {0}, {1}", mName, other->getName());
            string face = "None";
            switch (other->getBounds()->getCollisionInfo(mBounds).collisionFace) {
                case FISH::CollisionFace::Top:      face = "Top"; break;
                case FISH::CollisionFace::Back:     face = "Back"; break;
                case FISH::CollisionFace::Bottom:   face = "Bottom"; break;
                case FISH::CollisionFace::Front:    face = "Front"; break;
                case FISH::CollisionFace::Right:    face = "Right"; break;
                case FISH::CollisionFace::Left:    face = "Left"; break;
            }
            FS_INFO("Collision_FACE:{0}", face);
            hasCollided = 1;
        }
        
    }

    std::shared_ptr<FISH::OBB> getOBB() const {
        if (mBounds->getType() == FISH::ColliderType::OBB)
            return PtrCastTo<FISH::OBB>(mBounds);
        return nullptr;
    }

    void rotateZ(float angle) {
        if (std::abs(angle) <= 0.0001) return;
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
        
        needUpdate = 1;
    }

    std::shared_ptr<FISH::Mesh> mObj{nullptr};

    bool tag{0}, lasttag{0};
    bool hasCollided{false};
    
};

class MainLayer : public FISH::Layer {
public:
    ~MainLayer() {
        mGameData["Position"] = mGameObj->getPosition();
        mStorage->save("Test", mGameData);

    }

    void OnAttach() override {
        FISH::ColliderEditor::get()->openColliderEditor();
        mStorage.reset(new FISH::JsonFileStorage("TestData"));
        mStorage->load("Test", mGameData);
        shader.reset(FISH::Shader::CreateShader());
        //手柄测试///////////////
        mController.reset(FISH::Controller::CreateController());
        mController->init();
        ///////////////////

        //模板测试//
        std::shared_ptr<FISH::Shape> box;
        box.reset(FISH::Shape::CreateBox(2.1));
        wMesh.reset(new FISH::Mesh());
        wMesh->getShape() = box;
        ///////////
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
        mRenderstatuss->enablestatus(FISH::StatusType::CleanStencil);
        mRenderstatuss->enablestatus(FISH::StatusType::StencilTest);
        mRenderstatuss->enablestatus(FISH::StatusType::DepthTest);
        
        //设置清理APP函数
        APP.setClean([this]() {
            APP.GetWindow().CleanBuffer(mRenderstatuss->getCleanstatuss());
        });


        CTest.reset(new FISH::CollisionTest(std::make_shared<FISH::AABB>(glm::vec3(-50, -50, -50), glm::vec3(50, 50, 50))));
        CTest->setMaxObjsPreNode(20);
        CTest->setMaxdepth(3);
        auto shape = std::shared_ptr<FISH::Shape>(FISH::Shape::CreateBox(1.0));
        auto mesh = std::make_shared<FISH::Mesh>("TestS");
        auto mesh2 = std::make_shared<FISH::Mesh>();
        auto mesh3 = std::make_shared<FISH::Mesh>();


        mesh->getShape() = shape;
        mesh2->getShape() = shape;
        mesh3->getShape() = shape;
        

        mGameObj.reset(new AObj(std::make_shared<FISH::OBB>(glm::vec3(1, 1, 1), glm::mat3(1.0f)), "11"));
        mGameObj->setObj(mesh);
        mGameObj->setPosition( mGameData["Position"]);
        mGameObj2.reset(new AObj(std::make_shared<FISH::AABB>(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1)), "22"));
        mGameObj2->setObj(mesh2);
        mGameObj2->setPosition({3, 3, 3});
        mGameObj3.reset(new AObj(std::make_shared<FISH::AABB>(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1)), "33"));
        mGameObj3->setObj(mesh3);
        mGameObj3->setPosition({3, 3, -3});
        objs.push_back(mGameObj->mObj);
        objs.push_back(mGameObj2->mObj);
        objs.push_back(mGameObj3->mObj);
        objs = {mGameObj->mObj, mGameObj2->mObj, mGameObj3->mObj};
        for(int i = 4; i <= 12; i++) {
            auto obj = std::make_shared<AObj>(
                    std::make_shared<FISH::OBB>(glm::vec3(0.8f), glm::mat3(1.0f))
            );
            
            obj->setObj(std::make_shared<FISH::Mesh>(shape));
            
            objs.push_back(obj->mObj);
            // 在场景中分散位置
            float x = (i % 5) * 3.0f - 6.0f;
            float y = ((i + 1) % 3) * 3.0f - 3.0f;
            float z = ((i + 2) % 4) * 2.0f - 4.0f;
            obj->setPosition({x, y, z});
            
            // 随机旋转一些OBB对象
            if(i % 3 == 0) {
                obj->rotateZ(30.0f * i);
            }
            
            gameObjects.push_back(obj);
        }

        for(auto& obj : gameObjects) {
            CTest->insert(obj);
        }

        CTest->insert(mGameObj);
        CTest->insert(mGameObj2);
        CTest->insert(mGameObj3);

       // mCamera->setAllowedControl(1);
        FISH::ColliderEditor::get();
        FISH::ColliderEditor::get()->setViewer(mCamera);
    }

    void OnUpdate(float dt) override {
        mController->update();
        mCamera->update();
        CTest->update();
        CTest->check();
        const auto& vec = FISH::ColliderEditor::get()->outputGameBoxData(); 
        for (auto& box : vec) objs.push_back(box->getMesh());
        
        //FISH::Renderer::renderColliderBox(mGameObj->getBounds());

        if (mController->getKeyInfo("A").getKeyBool()) mGameObj->addPos({0, 3.0* dt, 0});
        if (mController->getKeyInfo("B").getKeyBool()) mGameObj->addPos({0, -3.0* dt, 0});
        if (mController->getKeyInfo("LB").getKeyBool()) CTest->enableRenderBox();
        if (mController->getKeyInfo("RB").getKeyBool()) CTest->disableRenderBox();

        mGameObj->rotateZ(5.0f * mController->getKeyInfo("LeftStickX").getKeyFloat() * dt);
        mGameObj->addPos({-mController->getKeyInfo("RightStickX").getKeyFloat()*dt*3, 0, mController->getKeyInfo("RightStickY").getKeyFloat()*dt*3});
        mController->setVibration(std::abs(mController->getKeyInfo("LeftStickX").getKeyFloat())*0.4, std::abs(mController->getKeyInfo("LeftStickX").getKeyFloat())*0.4);

        // if (FISH::Input::IsKeyPressed(FS_KEY_SPACE) && lastPress == 0) {
        //     if (!APP.getLockedState()) APP.LockCursor(), mCamera->setAllowedControl(1);
        //     else APP.UnLockCursor(), mCamera->setAllowedControl(0);
        // }

        lastPress =  FISH::Input::IsKeyPressed(FS_KEY_SPACE);


        
        

        //FS_INFO("{}", glm::to_string(mCamera->getProjectMatrix()));

        FISH::Renderer::render(objs);

        for (int i=0; i<vec.size(); i++) objs.pop_back();

        //CTest->cleanUp(0);
    }

    std::unique_ptr<FISH::CollisionTest> CTest;
    std::shared_ptr<AObj> mGameObj;
    std::shared_ptr<AObj> mGameObj2;
    std::shared_ptr<AObj> mGameObj3;
    std::unique_ptr<FISH::Renderstatus> mRenderstatuss;
    std::shared_ptr<FISH::perspectiveCamera> mCamera;
    std::shared_ptr<FISH::Shape> Shape2D;
    std::shared_ptr<FISH::Shader>                       shader;
    std::vector<std::shared_ptr<FISH::Object3D>>        objs;
    bool lastPress = 0;
    std::shared_ptr<FISH::Controller>                   mController;
    std::unique_ptr<FISH::JsonFileStorage>              mStorage;
    FISH::Json                                          mGameData;
    std::shared_ptr<FISH::Mesh>                        wMesh;
    std::vector<std::shared_ptr<AObj>>                  gameObjects;
    std::set<FISH::MeshPtr>                                   has;

};


class Sandbox : public FISH::Application {
public:
    Sandbox() {
        FISH::Texture::initDefaultTexture();
        FISH::Renderer::initDefaultShader();
        PushLayer(new MainLayer());
        PushLayer(FISH::ColliderEditor::get());
    }

    ~Sandbox() {}


};

FISH::Application* FISH::CreateApplication() {
    return new Sandbox();
}