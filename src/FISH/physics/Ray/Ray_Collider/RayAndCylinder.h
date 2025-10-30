#pragma once
#include "../Ray.h"

namespace FISH {
    namespace RayAndCollider {
        bool RayAndCylinder(const Ray& ray, const CylinderPtr& cylinder);       
    }
}