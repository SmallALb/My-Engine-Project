#include "fspcs.h"
#include "../Renderstatus.h"
#include "FISH/Object/Camera.h"
#include "FISH/Renderer/Buffer.h"
#include "FISH/physics/Colliders/Collider.h"
#include "ColliderRender.h"


namespace FISH {
    namespace Renderer {
        void renderCollider(const ColliderPtr &box, const glm::vec3 &color, const CameraPtr& camera) {
            RernderStatusPtr status;
            status.reset(Renderstatus::CreateRenderstatus());
            status->disablestatus(StatusType::DepthTest);
            status->disablestatus(StatusType::Blend);
            auto shader = DefaultShader::getShaderFromName(DefaultShaderName::OnlyColor3D);
            shader->Begin();
            shader->setMat4("projection", camera->getProjectMatrix());
            shader->setMat4("view", camera->getViewMatrix());
            shader->setMat4("model", glm::mat4(1.0f));
            shader->setMat4("normat", {1.0});  
            shader->setVector3("InColor", color);
            box->getVertices()->renderIndex(0, LINES);
            shader->End();
            status->enablestatus(StatusType::DepthTest);
            status->enablestatus(StatusType::Blend);
        }
    }
}