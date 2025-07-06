#pragma once

#include "FISH/Core.h"

namespace FISH {
    enum class CastType{
        //盒体碰撞
        Box,
        //球体碰撞
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