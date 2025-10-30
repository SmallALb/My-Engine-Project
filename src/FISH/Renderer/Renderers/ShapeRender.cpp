#include "fspcs.h"
#include "../Renderstatus.h"
#include "FISH/Object/Camera.h"
#include "../Buffer.h"
#include "../VertexArray.h"
#include "../BaseShape.h"
#include "ShapeRender.h"

namespace FISH { 
    namespace Renderer {
        void RenderShape(const ShapePtr &shape, const glm::vec3 &color, ElementType typ, const glm::mat4& model, const CameraPtr& camera) {
            auto shader = DefaultShader::getShaderFromName(DefaultShaderName::OnlyColor3D);
            shader->Begin();
            shader->setMat4("model", model);
            shader->setMat4("projection", camera->getProjectMatrix());
            shader->setMat4("view", camera->getViewMatrix());
            shader->setVector3("InColor", color);
            shape->render((int)typ);
            shader->End();
        }
    }
}
