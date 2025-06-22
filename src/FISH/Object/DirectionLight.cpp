#include "fspcs.h"
#include "DirectionLight.h"
#include "SpotLight.h"

namespace FISH{

    DirectionLight::DirectionLight() {
        lightType = LightType::Directional;
    }

    DirectionLight::~DirectionLight() {

    }
    glm::vec3 DirectionLight::getLightDir() const {
             return glm::normalize(mPosition - mlookPoint);
    }

    void DirectionLight::setLightDir(const glm::vec3& dir) {
             mlookPoint = glm::normalize(dir) + mPosition;
    }
}