#include <FISH.h>
#include <vector> 
#include <memory>



class MainLayer : public FISH::Layer {
public:
    ~MainLayer() {
        FISH::Sound::Shutdown();
    }

    void OnAttach() override {
        FISH::Sound::Initialize();
        // mSound.reset(new FISH::Sound("Sounds/oiiai.wav", FISH::AudioType::Audio3D, 1));

        // mSound->setMaxDistance(600.0f);
        // mSound->setMinDistance(1.0f);
        // mSound->setPosition({0, 0, 0});
        // mSound->setRolloffFactor(35.0f);
        // mSound->setVolume(0.5);

        shader.reset(FISH::Shader::CreateShader());
        mAni.reset(new FISH::SpriteAnimation("picture/sprite", "cat ", 23, 1, 80));
        shader->readVertexShader("sharders/EnginRenderShader/2Dvertex.glsl");
        shader->readFragmentShader("sharders/EnginRenderShader/OnlyColor.glsl");
        shader->CompileLink();

        Pan.reset(FISH::Shape::CreateSphere(3.0));
        Pan->setTexture(mAni);
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
        mesh2.reset(new FISH::Mesh());
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

        mMesh->getShape() = std::shared_ptr<FISH::Shape>(FISH::Shape::CreateRectangle(3.0f, 8.0));
        
        mesh2->getShape() = Pan;
        mesh2->setPosition({0.0, 4.0, -3.0});
        

        auto shape = std::shared_ptr<FISH::Shape>(FISH::Shape::CreateBox(1.0));;
        auto mesh = std::make_shared<FISH::Mesh>();
        auto mesh2 = std::make_shared<FISH::Mesh>();
        auto mesh3 = std::make_shared<FISH::Mesh>();

        mesh->getShape() = shape;
        mesh2->getShape() = shape;
        mesh3->getShape() = shape;


        objs.push_back(mMesh);
        objs.push_back(point);
        objs.push_back(point2);
        objs.push_back(mBox);
        objs.push_back(mesh2);
        //objs.push_back(spot);
        mBox->setTexture(FISH::Texture::CreateTextureFromPath("picture/Sky.png"));

        FISH::Renderer::setUseAmbientLight(light);  
        //FISH::Renderer::setUseShader(shader);
        FISH::Renderer::setUseCamera(cameras[0]);
        //cameras[0]->setAllowedControl(1);
        cameras[0]->setLookAt({0.0, 0.0, -5.0f});
        cameras[0]->addChild(mBox);
        dynamic_cast<FISH::perspectiveCamera*>(cameras[0].get())->setSpeed(0.03);
        dynamic_cast<FISH::perspectiveCamera*>(cameras[0].get())->setSensitivity(0.07);
        mRenderstatuss->enablestatus(FISH::StatusType::DepthTest);
        mRenderstatuss->enablestatus(FISH::StatusType::CleanDepth);
        mRenderstatuss->enablestatus(FISH::StatusType::CleanColor);
        mRenderstatuss->setstatusFunc(FISH::SetType::DepthFunc, FISH::FuncType::DepthLess);

        APP.setClean([&]() {
            APP.GetWindow().CleanBuffer(mRenderstatuss->getCleanstatuss());
        });

        //APP.GetWindow().SetVSync(1);

        mFont.reset(new FISH::Font("Fonts/testfont.ttf"));

        mBotton.reset(new FISH::Botton());
        auto& window = APP.GetWindow();
        mBotton->setPosition(ToProportCoord({100, 30}, window.GetWidth(), window.GetHeight()));
        mBotton->setBoxSize(ToProportCoord({50, 50}, window.GetWidth(), window.GetHeight()));
        FS_INFO("{0}, {1}", mBotton->getPosition().x, mBotton->getPosition().y);
        mBotton->setHoverFunc([&](){
            float x = mBotton->getPosition().x;
            float y = mBotton->getPosition().y;
            //mFont->RenderText("我在这！", x, y, 0.001, {1.0, 1.0, 1.0});
            mFont->RenderText("我在这！", x, y, 0.001, {1.0, 1.0, 1.0});
        });
        mBotton->setClickFunc([&](){
            float x = mBotton->getPosition().x;
            float y = mBotton->getPosition().y;
            mFont->RenderText("草！", x, y, 0.001, {1.0, 0.0, 0.0});
        });
        mAni->setBeginInTexture(17);
        mAni->play(FISH::AnimationMode::Loop);
        //mSound->play();

    }   

    void OnImGuiRender() override {
        //FISH::UI::Text("绘制！");
        FISH::UI::InputText("输入", inputbuffer, sizeof(inputbuffer));    

    }

    void OnUpdate(float dt) override {
        
        cameras[0]->update();
        dynamic_cast<FISH::perspectiveCamera*>(cameras[0].get())->setAspect((float)(APP.GetWindow().GetWidth()/APP.GetWindow().GetHeight()));
        //dynamic_pointer_cast<FISH::SpotLight>(objs[3])->setLightDir(cameras[0]->getFront());

        FISH::Sound::SetListenerPosition(cameras[0]->getPosition());
        FISH::Sound::SetListenerOrientation(
            cameras[0]->getFront(),  // 前向向量
            cameras[0]->getUp()      // 上向量
        );
        FISH::Renderer::render(objs);

        // mRenderstatuss->enablestatus(FISH::StatusType::Blend);
        // mRenderstatuss->setstatusFunc(FISH::SetType::BlendFunc,  FISH::FuncType::SrcAlpha,  FISH::FuncType::MinusSrcAlpha);
        // FISH::Renderer::ShaderLib["OnlyTexture"]->Begin();
        // FISH::Renderer::ShaderLib["OnlyTexture"]->setMat4("projection", cameras[0]->getProjectMatrix());
        // FISH::Renderer::ShaderLib["OnlyTexture"]->setMat4("view", cameras[0]->getViewMatrix());
        // FISH::Renderer::ShaderLib["OnlyTexture"]->setMat4("model", mesh2->getModelMatrix());
        // FISH::Renderer::ShaderLib["OnlyTexture"]->setMat4("normat", mesh2->getNormalMatrix());  
        // FISH::Renderer::ShaderLib["OnlyTexture"]->setTextureHandle("uTexture", mAni->getHandle());
        // mesh2->getShape()->render(TRIANGLES);
        // FISH::Renderer::ShaderLib["OnlyTexture"]->End();
        // mRenderstatuss->disablestatus(FISH::StatusType::Blend);

        mRenderstatuss->disablestatus(FISH::StatusType::DepthTest);
        shader->Begin();
        Shape2D->useShape();
        shader->setVector3("InColor", {1.0, 1.0, 1.0});
        Shape2D->render(TRIANGLES);
        Shape2D->unuseShape();
        shader->End();
        mRenderstatuss->enablestatus(FISH::StatusType::DepthTest);
        

        mFont->RenderText(std::to_string(co), -0.2, 0, 0.004f, {1.0, 1.0, 1.0});

        //bool currntpress = FISH::Input::IsKeyPressed(FS_KEY_C);
        if (FISH::Input::IsKeyPressedOnce(FS_KEY_C)) {
            if (!islock) APP.LockCursor(), islock = 1,cameras[0]->setAllowedControl(1);
            else APP.UnLockCursor(), islock = 0,cameras[0]->setAllowedControl(0);
            // if (mAni->IsPause() || mAni->IsFinsh()) mAni->play(FISH::AnimationMode::Loop);
            // else mAni->pause();
        }

        


        mBotton->setClick(FISH::Input::IsMouseButtonPressed(FS_MOUSE_BUTTON_LEFT));
        auto [mx, my] = FISH::Input::GetMousePos();
        auto ndc = ToNDC({mx, my}, APP.GetWindow().GetWidth(), APP.GetWindow().GetHeight());
        mBotton->update(ndc.x, ndc.y);
        //lstpress = currntpress;
    }

    std::vector<std::shared_ptr<FISH::Object3D>> objs;
    std::vector<std::shared_ptr<FISH::Camera>> cameras;
    std::shared_ptr<FISH::DirectionLight> light;
    std::shared_ptr<FISH::Shader> shader;
    std::shared_ptr<FISH::Shape> Shape2D;
    std::unique_ptr<FISH::Renderstatus> mRenderstatuss;
    std::shared_ptr<FISH::Font>         mFont;
    std::shared_ptr<FISH::Botton>       mBotton;
    std::shared_ptr<FISH::SpriteAnimation>  mAni;
    std::shared_ptr<FISH::Shape>            Pan;
    std::shared_ptr<FISH::Mesh>             mesh2;
    std::shared_ptr<FISH::Sound>            mSound;
    bool lstpress{0}, islock{0};
    char inputbuffer[256] = "";
    int id{0};
    std::atomic_int co{0};

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