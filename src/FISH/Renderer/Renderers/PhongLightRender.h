#pragma once

#include "DefaultShader.h"

#include "../RenderElement.h"

namespace FISH {
    namespace Renderer {
        
        void ImportLightInfo(const std::vector<std::shared_ptr<SpotLight>> &spotlights, 
            const std::vector<std::shared_ptr<PointLight>> &pointlights, const CameraPtr& camera);

        void RenderObj_Phong(const std::shared_ptr<Object3D> &obj,
            const std::vector<std::shared_ptr<SpotLight>> &spotlights, 
            const std::vector<std::shared_ptr<PointLight>> &pointlights,
            const CameraPtr& camera);

        void RenderInPhongMode(const std::shared_ptr<Scene>& scene, const CameraPtr& camera);

        void setAmbiLight(const std::shared_ptr<DirectionLight>& light);

        static std::shared_ptr<DirectionLight> useAmbiLight{nullptr};

    }
}