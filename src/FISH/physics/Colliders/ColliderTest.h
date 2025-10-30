#pragma once

#include "Collider.h"
#include "AABB.h"
#include "OBB.h"
#include "Sphere.h"
#include "Cylinder.h"

namespace FISH {

    // 碰撞面
    enum class CollisionFace {
        None,
        Top,
        Bottom,
        Front,
        Back,
        Left,
        Right
    };

    // 碰撞信息
    struct CollisionInfo {
        CollisionInfo() {
            contactPoints[0] = glm::vec3{0.0};
            contactPoints[1] = glm::vec3{0.0};
            contactPoints[2] = glm::vec3{0.0};
            contactPoints[3] = glm::vec3{0.0};
        }
        
        bool IsColliding = false;
        float Depth = 0.0f;
        float penetrationDepth = 0.0f;
        CollisionFace collisionFace = CollisionFace::None;
        glm::vec3 collisionNormal;
        glm::vec3 contactPoints[4];
        glm::vec3 collisionPoint;
        std::string colliderType;
    };
    //碰撞测试类
    class ColliderTest {
    public:
        //查看两顶点是否相交
        static bool intersects(const ColliderPtr& A, const ColliderPtr& B);
        //获取两物体的碰撞信息
        static CollisionInfo getCollisionInfo(const ColliderPtr& A, const ColliderPtr& B);
        //获取碰撞的面信息
        static CollisionFace getCollisionFace(const glm::vec3& normal, const glm::vec3& Upvec = {0, 1, 0});
    private:
        //aabb - aabb
        static bool intersectsAABB_AABB(const AABBPtr& a, const AABBPtr& b);
        //aabb - obb
        static bool intersectsAABB_OBB(const AABBPtr& a, const OBBPtr& b);
        //obb - obb
        static bool intersectsOBB_OBB(const OBBPtr& a, const OBBPtr& b);
        //sphere - aabb
        static bool intersectsSphere_AABB(const SpherePtr& a, const AABBPtr& b);
        //sphere - obb
        static bool intersectsSphere_OBB(const SpherePtr& a, const OBBPtr& b);
        //sphere - sphere
        static bool intersectsSphere_Sphere(const SpherePtr& a, const SpherePtr& b);
        //cylinder - cylinder
        static bool intersectsCylinder_Cylinder(const CylinderPtr& a, const CylinderPtr& b);
        //cylinder - aabb
        static bool intersectsCylinder_AABB(const CylinderPtr& a, const AABBPtr& b);
        //cylinder - obb
        static bool intersectsCylinder_OBB(const CylinderPtr& a, const OBBPtr& b);
        //cylinder - sphere
        static bool intersectsCylinder_Sphere(const CylinderPtr& a, const SpherePtr& b);
        
        //碰撞信息获取
        static CollisionInfo getAABB_AABBCollisionInfo(const AABBPtr& a, const AABBPtr& b);
        static CollisionInfo getAABB_OBBCollisionInfo(const AABBPtr& a, const OBBPtr& b);
        static CollisionInfo getOBB_OBBCollisionInfo(const OBBPtr& a, const OBBPtr& b);
        static CollisionInfo getSphere_AABBCollisionInfo(const SpherePtr& a, const AABBPtr& b);
        static CollisionInfo getSphere_OBBCollisionInfo(const SpherePtr& a, const OBBPtr& b);
        static CollisionInfo getSphere_SphereCollisionInfo(const SpherePtr& a, const SpherePtr& b);
        static CollisionInfo getCylinder_AABBCollisionInfo(const CylinderPtr& a, const AABBPtr& b);
        static CollisionInfo getCylinder_OBBCollisionInfo(const CylinderPtr& a, const OBBPtr& b);
        static CollisionInfo getCylinder_SphereCollisionInfo(const CylinderPtr& a, const SpherePtr& b);
    };



}