#pragma once

#include "FISH/Core.h"

namespace FISH {
    enum class CastType{
        Box,
        Ball,
    };


    class API_ RayTest {
    public:
        static bool IsRayCastObj(
            const glm::vec3& RayPos, 
            const glm::vec3& RayDir, 
            const std::shared_ptr<FISH::Object3D>& obj, 
            float siz = 10,
            CastType TYP = CastType::Ball);
    private: 
        static bool BoxCast(const glm::vec3 &RayPos, const glm::vec3 &RayDir, const std::shared_ptr<FISH::Object3D> &obj, float siz);

        static bool BallCast(const glm::vec3 &RayPos, const glm::vec3 &RayDir, const std::shared_ptr<FISH::Object3D> &obj, float siz);

    };


}