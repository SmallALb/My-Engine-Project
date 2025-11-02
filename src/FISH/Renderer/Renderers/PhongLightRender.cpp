#include "fspcs.h"
#include "FISH/Log.h"
#include "../Buffer.h"
#include "../VertexArray.h"
#include "../BaseShape.h"
#include "../Material.h"
#include "../Renderstatus.h"
#include "FISH/Object/Object.h"
#include "FISH/Object/Light.h"
#include "FISH/Object/Camera.h"
#include "FISH/Object/DirectionLight.h"
#include "FISH/Object/PointLight.h"
#include "FISH/Object/SpotLight.h"
#include "FISH/Object/Mesh.h"
#include "FISH/Object/Scene.h"
#include "PhongLightRender.h"

namespace FISH {
    namespace Renderer {
        void ImportLightInfo(const std::vector<std::shared_ptr<SpotLight>> &spotlights, const std::vector<std::shared_ptr<PointLight>> &pointlights, const CameraPtr &camera) {
            auto shader = DefaultShader::getShaderFromName(DefaultShaderName::PhongLight);
            shader->setVector3("cameraPosition", camera->getPosition());
            //设置聚光灯参数
            shader->setInt("numSpotLights", spotlights.size());
            for (unsigned int i = 0; i < spotlights.size(); ++i) {
                auto& light = spotlights[i];
                shader->setVector3("spotLights[" + std::to_string(i) + "].position", light->getPosition());
                shader->setVector3("spotLights[" + std::to_string(i) + "].direction", light->getLightDir());
                shader->setVector3("spotLights[" + std::to_string(i) + "].color", light->getColor());
                shader->setFloat("spotLights[" + std::to_string(i) + "].intensity", light->getIntensity());
                shader->setFloat("spotLights[" + std::to_string(i) + "].kc", light->getkc());
                shader->setFloat("spotLights[" + std::to_string(i) + "].k1", light->getk1());
                shader->setFloat("spotLights[" + std::to_string(i) + "].k2", light->getk2());
                shader->setFloat("spotLights[" + std::to_string(i) + "].cutOff", glm::cos(glm::cos(glm::radians(light->getVisibleAngle()))));
                shader->setFloat("spotLights[" + std::to_string(i) + "].outerCutOff", glm::cos(glm::cos(glm::radians(light->getOuterAngle()))));
                shader->setFloat("spotLights[" + std::to_string(i) + "].specularIntensity", light->getSpecularIntensity());
                shader->setInt("spotLights[" + std::to_string(i) + "].flag", light->getIsLightOn());
            }

            //设置点光源参数
            shader->setInt("numPointLights", pointlights.size());
            for (unsigned int i = 0; i < pointlights.size(); ++i) {
                auto& light = pointlights[i];
                shader->setVector3("pointLights[" + std::to_string(i) + "].position", light->getPosition());
                shader->setVector3("pointLights[" + std::to_string(i) + "].color", light->getColor());
                shader->setFloat("pointLights[" + std::to_string(i) + "].kc", light->getkc());
                shader->setFloat("pointLights[" + std::to_string(i) + "].k1", light->getk1());
                shader->setFloat("pointLights[" + std::to_string(i) + "].k2", light->getk2());
                shader->setFloat("pointLights[" + std::to_string(i) + "].specularIntensity", light->getSpecularIntensity());
                shader->setInt("pointLights[" + std::to_string(i) + "].flag", light->getIsLightOn());
            }
            if (!useAmbiLight) {
                return;
            }
            
            shader->setInt("dirLightOn", useAmbiLight->getIsLightOn());
            shader->setVector3("ambientcolor", useAmbiLight->getColor());
            shader->setVector3("dirLight.direction", useAmbiLight->getLightDir());
            shader->setVector3("dirLight.color", useAmbiLight->getColor());
            shader->setFloat("dirLight.intensity", useAmbiLight->getIntensity());
            shader->setFloat("dirLight.specularIntensity", useAmbiLight->getSpecularIntensity());

        }

        void RenderObj_Phong(const std::shared_ptr<Object3D> &obj, const std::vector<std::shared_ptr<SpotLight>> &spotlights, const std::vector<std::shared_ptr<PointLight>> &pointlights, const CameraPtr &camera) {
            if (obj == nullptr) return;
            RernderStatusPtr mstatuss;
            mstatuss.reset(Renderstatus::CreateRenderstatus());
            auto shader = DefaultShader::getShaderFromName(DefaultShaderName::PhongLight);
            switch (obj->GetObjType()) {
                //当obj为模型时
                case ObjType::Mesh: {
                    auto ptr = Static_PtrCastTo<Mesh>(obj);
                    auto shape = ptr->getShape();
                    auto material = ptr->getMaterial();
                    
                    if (material->isFaceCullEnable()) {
                        mstatuss->enablestatus(StatusType::CullFace);
                        mstatuss->setstatusFunc(SetType::CullFaceFunc, material->getCullFace());
                        mstatuss->setstatusFunc(SetType::FrontFaceDIR, material->getFrontFaceDir());
                    }
                    if (!material->isDepthWriteEnable())  {
                        mstatuss->setstatusFunc(SetType::DepthFunc, material->getDepthTestMode());
                        mstatuss->setstatusFunc(SetType::DepthMask, FuncType::FALSETyp);
                    }
                    else mstatuss->setstatusFunc(SetType::DepthFunc, material->getDepthTestMode());

                    if (material->isBlendColorEnable()) {
                        mstatuss->disablestatus(StatusType::CullFace);
                        mstatuss->enablestatus(StatusType::Blend);
                        mstatuss->setstatusFunc(SetType::BlendFunc, material->getSrcAlpthaBlendMode(), material->getDstAlpthBlendMode());
                        
                    }
                    //渲染被标记时的边缘
                    if (ptr && ptr->isHightLight()){       
                        mstatuss->setstatusFunc(SetType::StencilFunc, FuncType::Always, 1, 0xff);
                        mstatuss->setstatusFunc(FISH::SetType::StencilOperation, FISH::FuncType::Keep, FISH::FuncType::Keep, FISH::FuncType::Replace);
                        mstatuss->setstatusFunc(SetType::StencilMask, FuncType::ZERO, 0xff);         
                    }
                    shader->Begin();
                    shader->setMat4("projection", camera->getProjectMatrix());
                    shader->setMat4("view", camera->getViewMatrix());
                    ImportLightInfo(spotlights, pointlights, camera);

                    shader->setTextureHandle("sampler", material->getTextureHandle(TextureType::None));
                    shader->setMat4("model", obj->getModelMatrix());
                    shader->setMat4("normat", obj->getNormalMatrix());
                    shader->setFloat("shininess", 32.0f);
                    shape->render(TRIANGLES); 
                    shader->End();

                    mstatuss->disablestatus(StatusType::CullFace);
                    break;
                }

            }
            mstatuss->setstatusFunc(FISH::SetType::DepthFunc, FISH::FuncType::DepthLess);
            mstatuss->setstatusFunc(FISH::SetType::StencilOperation, FISH::FuncType::Keep, FISH::FuncType::Keep, FISH::FuncType::Keep);
            mstatuss->setstatusFunc(FISH::SetType::StencilMask, FISH::FuncType::ZERO, 0xff);
            mstatuss->setstatusFunc(FISH::SetType::DepthMask, FISH::FuncType::TRUETyp);
            mstatuss->enablestatus(FISH::StatusType::DepthTest);
            mstatuss->disablestatus(FISH::StatusType::Blend);
        }

        void RenderInPhongMode(const std::shared_ptr<Scene> &scene, const CameraPtr &camera) {
            std::vector<std::shared_ptr<SpotLight>> spotlights;
            std::vector<std::shared_ptr<PointLight>> pointlights;
            std::vector<FISH::MeshPtr>  Blends;
            std::vector<FISH::MeshPtr>  NoBlends;
            //获取灯光
            auto getlights = [&](Object3DPtr obj, auto&& self)->void {
                if (obj->GetObjType() == FISH::ObjType::Light) {
                    if (Static_PtrCastTo<Light>(obj)->getLightType() == LightType::Point) {
                        pointlights.push_back(Static_PtrCastTo<PointLight>(obj));
                    }
                    else if (Static_PtrCastTo<Light>(obj)->getLightType() == LightType::Spot) {
                        spotlights.push_back(Static_PtrCastTo<SpotLight>(obj));
                    }
                }       

                for (auto& child : obj->getChilds()) self(child, self);
            };
            //划分透明渲染物体
            auto diviedBlend = [&](Object3DPtr obj, auto&& self)->void {
                if (obj->GetObjType() == ObjType::Mesh) {    
                    auto ptr = Static_PtrCastTo<Mesh>(obj);
                    auto material = ptr->getMaterial();
                    if (material->isBlendColorEnable()) {
                        Blends.push_back(ptr);
                    }
                    else {
                        NoBlends.push_back(ptr);
                    }
                }
                for (auto& child : obj->getChilds()) self(child, self);
            };
            getlights(scene, getlights); diviedBlend(scene, diviedBlend);

            std::sort(Blends.begin(), Blends.end(), [&](auto& a, auto& b) {
            auto cameraPos = camera->getPosition();
            auto front = camera->getFront();
            auto vecA = a->getPosition() - cameraPos;
            auto vecB = b->getPosition() - cameraPos;
            float lenA = (glm::dot(vecA, front) / glm::dot(front, front));
            float lenB = (glm::dot(vecB, front) / glm::dot(front, front));
            return lenA < lenB;
        });


        for (auto& obj : NoBlends) 
            RenderObj_Phong(obj, spotlights, pointlights, camera);
        //FS_CONDITION_INFO(Blends.empty(),"Blends Is empty");
        for (auto& obj : Blends) 
            RenderObj_Phong(obj, spotlights, pointlights, camera);
        }

        void setAmbiLight(const std::shared_ptr<DirectionLight> &light) {
            useAmbiLight = light;
        }
    }
}