#include "fspcs.h"
#include "PointLight.h"


namespace FISH {
    PointLight::PointLight() {
        lightType = LightType::Point;
    }

    PointLight::~PointLight() {
    }
}