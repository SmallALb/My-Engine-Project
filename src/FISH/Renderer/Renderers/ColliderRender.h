#pragma once
#include "DefaultShader.h"

#include "../RenderElement.h"

namespace FISH {
    namespace Renderer {
        void renderCollider(const ColliderPtr &box, const glm::vec3& color, const CameraPtr& camera);
    }
}