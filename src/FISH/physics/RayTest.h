#pragma once

#include "FISH/Game/GameObject.h"
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
            const Object3DPtr& obj, 
            float siz = 10,
            CastType TYP = CastType::Ball);

        static bool IsRayCastGameObj(            
            const glm::vec3& RayPos, 
            const glm::vec3& RayDir,
            const GameObjPtr& obj,
            CastType TYP = CastType::Box);
    private: 
        static bool BoxCast(const glm::vec3 &RayPos, const glm::vec3 &RayDir, const Object3DPtr& obj, float siz);

        static bool BallCast(const glm::vec3 &RayPos, const glm::vec3 &RayDir, const Object3DPtr& obj, float siz);
        
        static bool BoxCast(const glm::vec3 &RayPos, const glm::vec3 &RayDir, const AABBPtr& box);

        static bool BallCast(const glm::vec3 &RayPos, const glm::vec3 &RayDir, const AABBPtr& box);

        static bool OBBCast(const glm::vec3& RayPos, const glm::vec3& RayDir, const OBBPtr& box);
        
    };

    glm::vec3 MousePosToWorldPos_InViewPlane(std::pair<double, double> mousePos, CameraPtr camera, float width, float height);

    glm::vec3 MousePosToWorldPos_AtDistance(std::pair<double, double> mousePos, 
                                      CameraPtr camera, 
                                      float width, float height,
                                      float distance);

    glm::vec3 getRayDirectionFromMouse(std::pair<double, double> mousePos, CameraPtr camera, float width, float height);

    
}