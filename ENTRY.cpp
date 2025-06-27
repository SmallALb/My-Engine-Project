#include <FISH.h>
#include <vector> 
#include <memory>

class MainLayer : public FISH::Layer {
    void OnAttach() override {

        shader.reset(FISH::Shader::CreateShader());
        
        shader->readVertexShader("sharders/EnginRenderShader/2Dvertex.glsl");
        shader->readFragmentShader("sharders/EnginRenderShader/OnlyColor.glsl");
        shader->CompileLink();


        cameras.emplace_back(FISH::Camera::CreateCamera()); 
        dynamic_cast<FISH::perspectiveCamera*>(cameras[0].get())->
            init(
                60.f, 
                (float)(APP.GetWindow().GetWidth()/APP.GetWindow().GetHeight()),
                 0.1f,
                1000.f
            );

        cameras[0]->setPosition({0.0, 0.0, -13.0});

        light.reset(new FISH::DirectionLight());
        light->setLightOn(1);
        light->setPosition({0.0, 4.0, 0.0});
        light->setLookAtPoint({0.0, 0.0, 0.0});

        //std::shared_ptr<FISH::Shape> AShape(FISH::Shape::CreateSphere(2.0f));
        
        mRenderstatuss.reset(FISH::Renderstatus::CreateRenderstatus());
        std::shared_ptr<FISH::Mesh> mMesh(new FISH::Mesh());
        std::shared_ptr<FISH::SkyBox> mBox(new FISH::SkyBox());
        std::shared_ptr<FISH::PointLight> point(new FISH::PointLight());
        std::shared_ptr<FISH::PointLight> point2(new FISH::PointLight());
        //std::shared_ptr<FISH::SpotLight>  spot(new FISH::SpotLight());

        Shape2D.reset(FISH::Shape::CreateCircle2D(0.005f));

        point->setLightOn(1);
        point->setSpecularIntensity(1.0f);

        point->setIntensity(1.0f);
        point->setk2(0.1);
        point->setkc(0.01);
        point->setPosition({0.0, 0.0, -5.0f});
        point->setColor({1.0, 0.0, 0.0});

        point2->setLightOn(1);
        point2->setSpecularIntensity(1.0f);

        point2->setIntensity(1.0f);
        point2->setk2(0.1);
        point2->setkc(0.01);
        point2->setPosition({5.0, 0.0, 0.0f});
        point2->setColor({0.0, 1.0, 0.0});

        mMesh->getShape() = std::shared_ptr<FISH::Shape>(FISH::Shape::CreateBox(1.0f));
        
        

        objs.push_back(mMesh);
        objs.push_back(point);
        objs.push_back(point2);
        objs.push_back(mBox);
        //objs.push_back(spot);
        mBox->setTexture(FISH::Texture::CreateTextureFromPath("picture/Sky.png"));

        FISH::Renderer::setUseAmbientLight(light);  
        //FISH::Renderer::setUseShader(shader);
        FISH::Renderer::setUseCamera(cameras[0]);
        //cameras[0]->setAllowedControl(1);
        cameras[0]->setLookAt({0.0, 0.0, -5.0f});
        cameras[0]->addChild(mBox);
        mRenderstatuss->enablestatus(FISH::StatusType::DepthTest);
        mRenderstatuss->enablestatus(FISH::StatusType::CleanDepth);
        mRenderstatuss->enablestatus(FISH::StatusType::CleanColor);
        mRenderstatuss->setstatusFunc(FISH::SetType::DepthFunc, FISH::FuncType::DepthLess);

        APP.setClean([&]() {
            APP.GetWindow().CleanBuffer(mRenderstatuss->getCleanstatuss());
        });

        mFont.reset(new FISH::Font("Fonts/testfont.ttf"));
    }   

    void OnImGuiRender() override {
        //FISH::UI::Text("绘制！");
        FISH::UI::InputText("输入", inputbuffer, sizeof(inputbuffer));       
    }

    void OnUpdate() override {

        cameras[0]->update();
        dynamic_cast<FISH::perspectiveCamera*>(cameras[0].get())->setAspect((float)(APP.GetWindow().GetWidth()/APP.GetWindow().GetHeight()));
        //dynamic_pointer_cast<FISH::SpotLight>(objs[3])->setLightDir(cameras[0]->getFront());
        shader->Begin();
        Shape2D->useShape();
        shader->setVector3("InColor", {1.0, 1.0, 1.0});
        Shape2D->render(TRIANGLES);
        Shape2D->unuseShape();
        shader->End();
        

        FISH::Renderer::render(objs);

        mFont->RenderText(inputbuffer, -0.2, 0, 0.004f, {1.0, 1.0, 1.0});

        for (auto& obj : objs) if (FISH::Input::IsMouseButtonPressed(FS_MOUSE_BUTTON_LEFT)) {
            if (obj->GetObjType() != FISH::ObjType::SkyBox && FISH::RayTest::IsRayCastObj(cameras[0]->getPosition(), cameras[0]->getFront(), obj, 0.05)) {
                FS_WARN("CAST");
                break;
            }
        }
        bool currntpress = FISH::Input::IsKeyPressed(FS_KEY_C);
        if (!lstpress && currntpress) {
            if (!islock) APP.LockCursor(), islock = 1,cameras[0]->setAllowedControl(1);
            else APP.UnLockCursor(), islock = 0,cameras[0]->setAllowedControl(0);
                     
        }


        lstpress = currntpress;
        //FS_INFO("get camera lookat, {0}", glm::to_string(cameras[0]->getLookAtPoint()));
    }

    std::vector<std::shared_ptr<FISH::Object3D>> objs;
    std::vector<std::shared_ptr<FISH::Camera>> cameras;
    std::shared_ptr<FISH::DirectionLight> light;
    std::shared_ptr<FISH::Shader> shader;
    std::shared_ptr<FISH::Shape> Shape2D;
    std::unique_ptr<FISH::Renderstatus> mRenderstatuss;
    std::shared_ptr<FISH::Font>         mFont;
    bool lstpress{0}, islock{0};
    char inputbuffer[256] = "";
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