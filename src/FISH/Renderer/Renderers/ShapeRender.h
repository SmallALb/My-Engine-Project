#pragma once

#include "DefaultShader.h"

#include "../RenderElement.h"

namespace FISH {
    namespace Renderer  {
        void RenderShape(const ShapePtr& shape, const glm::vec3& color, ElementType typ, const glm::mat4& model, const CameraPtr& camera);
    }
}