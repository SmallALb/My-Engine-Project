#include "fspcs.h"
#include "SpotLight.h"

namespace FISH {
        SpotLight::SpotLight() {
            lightType = LightType::Spot;
        }

        SpotLight::~SpotLight()
        {
        }

        glm::vec3 SpotLight::getLightDir() const {
             return glm::normalize(mlookPoint - mPosition);
        }

        void SpotLight::setLightDir(const glm::vec3& dir) {
            mlookPoint =  mPosition + (dir);
        }

        
}