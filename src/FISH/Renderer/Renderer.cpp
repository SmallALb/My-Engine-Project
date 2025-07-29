#include "fspcs.h"

#include "API.h"
#include "Shader.h"
#include "RenderElement.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "BaseShape.h"
#include "../Object/Object.h"
#include "../Object/SpotLight.h"
#include "../Object/PointLight.h"
#include "../Object/DirectionLight.h"
#include "../Object/Camera.h"
#include "../Object/Mesh.h"
#include "../Object/SkyBox.h"
#include "../physics/ObjectCast.h"
#include "../Log.h"
#include "Renderstatus.h"
#include "Renderer.h"

namespace FISH {

    std::shared_ptr<Camera> Renderer::UseCamera = nullptr;
    std::shared_ptr<DirectionLight> Renderer::UseambientLight = nullptr;
    std::map<string, std::shared_ptr<Shader>> Renderer::ShaderLib {};
    std::unique_ptr<Shape> Renderer::ball = nullptr;
    std::unique_ptr<Renderstatus> Renderer::mstatuss = nullptr;
    bool Renderer::renderLight = 1;

    void Renderer::render(
        const std::vector<std::shared_ptr<Object3D>> &objs,
        const std::vector<std::shared_ptr<SpotLight>> &spotlights, 
        const std::vector<std::shared_ptr<PointLight>> &pointlights) {

        //上面不能有任何的依赖关系
        for (auto& obj : objs) if (obj->getParent() == nullptr) RenderObj(obj, spotlights, pointlights);
        
    }
    //3D渲染
    void Renderer::render(const std::vector<std::shared_ptr<Object3D>> &objs) {
        std::vector<std::shared_ptr<SpotLight>> spotlights;
        std::vector<std::shared_ptr<PointLight>> pointlights;

        for (auto obj : objs) {
            if (obj->GetObjType() == ObjType::Light) {
                auto lightobj = dynamic_pointer_cast<Light>(obj);
                if (lightobj->getLightType() == LightType::Point) {
                    pointlights.push_back(dynamic_pointer_cast<PointLight>(lightobj));
                }
                else {
                    spotlights.push_back(dynamic_pointer_cast<SpotLight>(lightobj));
                }
            }
        }
        for (auto& obj : objs) if (obj->getParent() == nullptr || obj->getParent()->GetObjType() == ObjType::Camera) 
            RenderObj(obj, spotlights, pointlights);
    }

    void Renderer::render(const std::vector<std::shared_ptr<Object2D>>& objs) {
        for (auto& obj : objs) if (obj->getParent() == nullptr)
            RenderObj(obj);
    }

    void Renderer::renderColliderBox(const ColliderPtr &box, const glm::vec3& color) {
        mstatuss->disablestatus(StatusType::DepthTest);
        ShaderLib["OnlyColor"]->Begin();
        ShaderLib["OnlyColor"]->setMat4("projection", UseCamera->getProjectMatrix());
        ShaderLib["OnlyColor"]->setMat4("view", UseCamera->getViewMatrix());
        ShaderLib["OnlyColor"]->setMat4("model", glm::mat4(1.0f));
        ShaderLib["OnlyColor"]->setMat4("normat", {1.0});  
        ShaderLib["OnlyColor"]->setVector3("InColor", color);
        box->getVertices()->renderIndex(0, LINES);
        ShaderLib["OnlyColor"]->End();
        mstatuss->enablestatus(StatusType::DepthTest);
    }

    void Renderer::initDefaultShader() {
        ball.reset(Shape::CreateSphere(0.5f));
        mstatuss.reset(Renderstatus::CreateRenderstatus());
        //冯氏光照处理着色器
        ShaderLib.emplace("Phong", std::shared_ptr<Shader>(Shader::CreateShader()));
        ShaderLib["Phong"]->readVertexShader("sharders/EnginRenderShader/3Dvertex.glsl");
        ShaderLib["Phong"]->readFragmentShader("sharders/EnginRenderShader/NormalFragment.glsl");
        ShaderLib["Phong"]->CompileLink();


        //纯颜色着色器
        ShaderLib.emplace("OnlyColor", std::shared_ptr<Shader>(Shader::CreateShader()));
        ShaderLib["OnlyColor"]->readVertexShader("sharders/EnginRenderShader/3Dvertex.glsl");
        ShaderLib["OnlyColor"]->readFragmentShader("sharders/EnginRenderShader/OnlyColor.glsl");
        ShaderLib["OnlyColor"]->CompileLink();
        
        //纯材质着色器
        ShaderLib.emplace("OnlyTexture", std::shared_ptr<Shader>(Shader::CreateShader()));
        ShaderLib["OnlyTexture"]->readVertexShader("sharders/EnginRenderShader/3Dvertex.glsl");
        ShaderLib["OnlyTexture"]->readFragmentShader("sharders/EnginRenderShader/Texture.glsl");
        ShaderLib["OnlyTexture"]->CompileLink();

        //2d纯色渲染
        ShaderLib.emplace("OnlyColor(2D)", std::shared_ptr<Shader>(Shader::CreateShader()));
        ShaderLib["OnlyColor(2D)"]->readVertexShader("sharders/EnginRenderShader/2Dvertex.glsl");
        ShaderLib["OnlyColor(2D)"]->readFragmentShader("sharders/EnginRenderShader/OnlyColor.glsl");
        ShaderLib["OnlyColor(2D)"]->CompileLink();
        
        //2d纯材质
        ShaderLib.emplace("OnlyTexture(2D)", std::shared_ptr<Shader>(Shader::CreateShader()));
        ShaderLib["OnlyTexture(2D)"]->readVertexShader("sharders/EnginRenderShader/2Dvertex.glsl");
        ShaderLib["OnlyTexture(2D)"]->readFragmentShader("sharders/EnginRenderShader/Texture.glsl");
        ShaderLib["OnlyTexture(2D)"]->CompileLink();
    }

    //分离渲染
    void Renderer::RenderObj(const std::shared_ptr<Object3D> &obj,
        const std::vector<std::shared_ptr<SpotLight>> &spotlights, 
        const std::vector<std::shared_ptr<PointLight>> &pointlights)
    {
        if (obj == nullptr) return;
        switch (obj->GetObjType()) {
            //当obj为模型时
            case ObjType::Mesh: {
                mstatuss->enablestatus(StatusType::CullFace);
                mstatuss->setstatusFunc(SetType::CullFaceFunc, FuncType::Back);
                mstatuss->setstatusFunc(SetType::FrontFaceDIR, FuncType::FaceCW);
                auto ptr = Static_PtrCastTo<Mesh>(obj);
                //渲染被标记时的边缘
                if (ptr && ptr->isHightLight()){       
                    mstatuss->setstatusFunc(SetType::StencilFunc, FuncType::Always, 1, 0xff);
                    mstatuss->setstatusFunc(FISH::SetType::StencilOperation, FISH::FuncType::Keep, FISH::FuncType::Keep, FISH::FuncType::Replace);
                    mstatuss->setstatusFunc(SetType::StencilMask, FuncType::ZERO, 0xff);         
                }
                ShaderLib["Phong"]->Begin();
                ShaderLib["Phong"]->setMat4("projection", UseCamera->getProjectMatrix());
                ShaderLib["Phong"]->setMat4("view", UseCamera->getViewMatrix());
                caculateLight(spotlights, pointlights);
                auto shape = (dynamic_cast<Mesh*>(obj.get()))->getShape();   
                ShaderLib["Phong"]->setTextureHandle("sampler", shape->useTexture());
                ShaderLib["Phong"]->setMat4("model", obj->getModelMatrix());
                ShaderLib["Phong"]->setMat4("normat", obj->getNormalMatrix());
                shape->render(TRIANGLES); 
                ShaderLib["Phong"]->End();
                if (ptr && ptr->isHightLight()) {
                    auto wMode = ptr->getModelMatrix();
                    wMode = glm::scale(wMode, glm::vec3(1.02));
                    mstatuss->setstatusFunc(FISH::SetType::StencilFunc, FISH::FuncType::NotEqual, 1, 0xff);
                    mstatuss->setstatusFunc(FISH::SetType::StencilMask, FISH::FuncType::ZERO, 0xff);
                    ShaderLib["OnlyColor"]->Begin();
                    ShaderLib["OnlyColor"]->setVector3("InColor", {1.0, 1.0, 1.0});
                    ShaderLib["OnlyColor"]->setMat4("projection", UseCamera->getProjectMatrix());
                    ShaderLib["OnlyColor"]->setMat4("view", UseCamera->getViewMatrix());
                    ShaderLib["OnlyColor"]->setMat4("model", wMode);
                    ptr->renderMesh(TRIANGLES);
                    ShaderLib["OnlyColor"]->End();
                }

                mstatuss->disablestatus(StatusType::CullFace);
                break;
            }
            //当obj为灯光时
            case ObjType::Light: {
                if (!renderLight) break;
                mstatuss->enablestatus(StatusType::CullFace);
                mstatuss->setstatusFunc(SetType::CullFaceFunc, FuncType::Back);
                mstatuss->setstatusFunc(SetType::FrontFaceDIR, FuncType::FaceCW);
                ShaderLib["OnlyColor"]->Begin();
                ShaderLib["OnlyColor"]->setMat4("projection", UseCamera->getProjectMatrix());
                ShaderLib["OnlyColor"]->setMat4("view", UseCamera->getViewMatrix());
                //ShaderLib["OnlyColor"]->setTextureHandle("sampler", ball->useTexture());
                auto color = (dynamic_cast<Light*>(obj.get()))->getColor();
                ShaderLib["OnlyColor"]->setMat4("model", obj->getModelMatrix());
                ShaderLib["OnlyColor"]->setMat4("normat", obj->getNormalMatrix());  
                ShaderLib["OnlyColor"]->setVector3("InColor", color);
                ball->render(TRIANGLES);
                ShaderLib["OnlyColor"]->End();
                mstatuss->disablestatus(StatusType::CullFace);
                //FS_INFO("RenderLight Done!");
                break;
            }
            //当obj为天空盒时
            case ObjType::SkyBox: {
                mstatuss->enablestatus(StatusType::CullFace);
                mstatuss->setstatusFunc(SetType::CullFaceFunc, FuncType::Front);
                ShaderLib["OnlyTexture"]->Begin();
                ShaderLib["OnlyTexture"]->setMat4("projection", UseCamera->getProjectMatrix());
                ShaderLib["OnlyTexture"]->setMat4("view", UseCamera->getViewMatrix());
                ShaderLib["OnlyTexture"]->setMat4("model", obj->getModelMatrix());
                ShaderLib["OnlyTexture"]->setMat4("normat", obj->getNormalMatrix());  
                ShaderLib["OnlyTexture"]->setTextureHandle("uTexture", dynamic_pointer_cast<SkyBox>(obj)->useTexture());
                dynamic_pointer_cast<SkyBox>(obj)->renderSkyBox();
                ShaderLib["OnlyTexture"]->End();
                mstatuss->disablestatus(StatusType::CullFace);
                break;
            }
        
        }
        mstatuss->setstatusFunc(FISH::SetType::DepthFunc, FISH::FuncType::DepthLess);
        mstatuss->setstatusFunc(FISH::SetType::StencilOperation, FISH::FuncType::Keep, FISH::FuncType::Keep, FISH::FuncType::Keep);
        mstatuss->setstatusFunc(FISH::SetType::StencilMask, FISH::FuncType::ZERO, 0xff);
        //渲染子节点
        for (auto &child : obj->getChilds()) {
            RenderObj(child, spotlights, pointlights);
        }
    }

    void Renderer::RenderObj(const std::shared_ptr<Object2D> &obj) {
        if (obj == nullptr) return;
        

        for (auto &child : obj->getChilds())
            RenderObj(child);
    }

    //计算灯光
    void Renderer::caculateLight(
        const std::vector<std::shared_ptr<SpotLight>> &spotlights,
         const std::vector<std::shared_ptr<PointLight>> &pointlights) {
                    ShaderLib["Phong"]->setVector3("cameraPosition", UseCamera->getPosition());
        //设置聚光灯参数
        ShaderLib["Phong"]->setInt("numSpotLights", spotlights.size());
        for (unsigned int i = 0; i < spotlights.size(); ++i) {
            auto& light = spotlights[i];
            ShaderLib["Phong"]->setVector3("spotLights[" + std::to_string(i) + "].position", light->getPosition());
            ShaderLib["Phong"]->setVector3("spotLights[" + std::to_string(i) + "].direction", light->getLightDir());
            ShaderLib["Phong"]->setVector3("spotLights[" + std::to_string(i) + "].color", light->getColor());
            ShaderLib["Phong"]->setFloat("spotLights[" + std::to_string(i) + "].intensity", light->getIntensity());
            ShaderLib["Phong"]->setFloat("spotLights[" + std::to_string(i) + "].kc", light->getkc());
            ShaderLib["Phong"]->setFloat("spotLights[" + std::to_string(i) + "].k1", light->getk1());
            ShaderLib["Phong"]->setFloat("spotLights[" + std::to_string(i) + "].k2", light->getk2());
            ShaderLib["Phong"]->setFloat("spotLights[" + std::to_string(i) + "].cutOff", glm::cos(glm::cos(glm::radians(light->getVisibleAngle()))));
            ShaderLib["Phong"]->setFloat("spotLights[" + std::to_string(i) + "].outerCutOff", glm::cos(glm::cos(glm::radians(light->getOuterAngle()))));
            ShaderLib["Phong"]->setFloat("spotLights[" + std::to_string(i) + "].specularIntensity", light->getSpecularIntensity());
            ShaderLib["Phong"]->setInt("spotLights[" + std::to_string(i) + "].flag", light->getIsLightOn());
        }

        //设置点光源参数
        ShaderLib["Phong"]->setInt("numPointLights", pointlights.size());
        for (unsigned int i = 0; i < pointlights.size(); ++i) {
            auto& light = pointlights[i];
            ShaderLib["Phong"]->setVector3("pointLights[" + std::to_string(i) + "].position", light->getPosition());
            ShaderLib["Phong"]->setVector3("pointLights[" + std::to_string(i) + "].color", light->getColor());
            ShaderLib["Phong"]->setFloat("pointLights[" + std::to_string(i) + "].kc", light->getkc());
            ShaderLib["Phong"]->setFloat("pointLights[" + std::to_string(i) + "].k1", light->getk1());
            ShaderLib["Phong"]->setFloat("pointLights[" + std::to_string(i) + "].k2", light->getk2());
            ShaderLib["Phong"]->setFloat("pointLights[" + std::to_string(i) + "].specularIntensity", light->getSpecularIntensity());
            ShaderLib["Phong"]->setInt("pointLights[" + std::to_string(i) + "].flag", light->getIsLightOn());
        }
        ShaderLib["Phong"]->setInt("dirLightOn", UseambientLight->getIsLightOn());
        ShaderLib["Phong"]->setVector3("ambientcolor", UseambientLight->getColor());
        ShaderLib["Phong"]->setVector3("dirLight.direction", UseambientLight->getLightDir());
        ShaderLib["Phong"]->setVector3("dirLight.color", UseambientLight->getColor());
        ShaderLib["Phong"]->setFloat("dirLight.intensity", UseambientLight->getIntensity());
        ShaderLib["Phong"]->setFloat("dirLight.specularIntensity", UseambientLight->getSpecularIntensity());
        ShaderLib["Phong"]->setFloat("shiness", 64.0f);



    }
}