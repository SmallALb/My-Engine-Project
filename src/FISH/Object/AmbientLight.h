#pragma once
#include "Light.h"
#include "FISH/Core.h"

namespace FISH {
    class AmbientLight : public Light {
    public:
        AmbientLight();

        ~AmbientLight();
    };

}
