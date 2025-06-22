#include "fspcs.h"
#include "AmbientLight.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Light.h"

namespace FISH {
    Light::~Light() {

    }

    Light::Light() {
        mType = ObjType::Light;
    }
}
