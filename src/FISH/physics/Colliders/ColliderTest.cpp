#include "fspcs.h"
#include "FISH/Base/HashTable.h"
#include "ColliderTest.h"

namespace FISH {
    bool ColliderTest::intersects(const ColliderPtr &A, const ColliderPtr &B) {
        //不能有None
        if (A->getType() == ColliderType::None || B->getType() == ColliderType::None) return false;

        switch ((uint8_t)A->getType() | (uint8_t)B->getType()) {
            //单个物体
            case (uint8_t)ColliderType::AABB: {
                return intersectsAABB_AABB(Static_PtrCastTo<AABB>(A), Static_PtrCastTo<AABB>(B));
            }
            case (uint8_t)ColliderType::OBB: {
                return intersectsOBB_OBB(Static_PtrCastTo<OBB>(A), Static_PtrCastTo<OBB>(B));
            }
            case (uint8_t)ColliderType::Sphere: {
                return intersectsSphere_Sphere(Static_PtrCastTo<Sphere>(A), Static_PtrCastTo<Sphere>(B));
            }
            case (uint8_t) ColliderType::Cylinder: {
                return intersectsCylinder_Cylinder(Static_PtrCastTo<Cylinder>(A),Static_PtrCastTo<Cylinder>(B)); 
            }
            //多个物体
            //AABB-OBB
            case (uint8_t)ColliderType::AABB | (uint8_t)ColliderType::OBB: {
                return 
                    A->getType() == ColliderType::AABB ? 
                    intersectsAABB_OBB(Static_PtrCastTo<AABB>(A), Static_PtrCastTo<OBB>(B)) : 
                    intersectsAABB_OBB(Static_PtrCastTo<AABB>(B), Static_PtrCastTo<OBB>(A));
            }
            //Sphere-OBB
            case (uint8_t)ColliderType::Sphere | (uint8_t)ColliderType::OBB: {
                return
                    A->getType() == ColliderType::Sphere ? 
                    intersectsSphere_OBB(Static_PtrCastTo<Sphere>(A), Static_PtrCastTo<OBB>(B)) : 
                    intersectsSphere_OBB(Static_PtrCastTo<Sphere>(B), Static_PtrCastTo<OBB>(A));
            }
            //Sphere-AABB
            case (uint8_t)ColliderType::Sphere | (uint8_t)ColliderType::AABB: {
                return
                    A->getType() == ColliderType::Sphere ? 
                    intersectsSphere_AABB(Static_PtrCastTo<Sphere>(A), Static_PtrCastTo<AABB>(B)) : 
                    intersectsSphere_AABB(Static_PtrCastTo<Sphere>(B), Static_PtrCastTo<AABB>(A));
            }
              //Cylinder-AABB
            case (uint8_t)ColliderType::Cylinder | (uint8_t)ColliderType::AABB: {
                return
                    A->getType() == ColliderType::Cylinder ? 
                    intersectsCylinder_AABB(Static_PtrCastTo<Cylinder>(A), Static_PtrCastTo<AABB>(B)) : 
                    intersectsCylinder_AABB(Static_PtrCastTo<Cylinder>(B), Static_PtrCastTo<AABB>(A));
            }
            //Cylinder-OBB
            case (uint8_t)ColliderType::Cylinder | (uint8_t)ColliderType::OBB: {
                return
                    A->getType() == ColliderType::Cylinder ? 
                    intersectsCylinder_OBB(Static_PtrCastTo<Cylinder>(A), Static_PtrCastTo<OBB>(B)) : 
                    intersectsCylinder_OBB(Static_PtrCastTo<Cylinder>(B), Static_PtrCastTo<OBB>(A));
            }
            //Cylinder-Sphere
            case (uint8_t)ColliderType::Cylinder | (uint8_t)ColliderType::Sphere: {
                return
                    A->getType() == ColliderType::Cylinder ? 
                    intersectsCylinder_Sphere(Static_PtrCastTo<Cylinder>(A), Static_PtrCastTo<Sphere>(B)) : 
                    intersectsCylinder_Sphere(Static_PtrCastTo<Cylinder>(B), Static_PtrCastTo<Sphere>(A));
            }

        }
        return false;
    }

    CollisionInfo ColliderTest::getCollisionInfo(const ColliderPtr &A, const ColliderPtr &B) {
        //不能有None
        if (A->getType() == ColliderType::None || B->getType() == ColliderType::None) return CollisionInfo();

        switch ((uint8_t)A->getType() | (uint8_t)B->getType()) {
            //单个物体
            case (uint8_t)ColliderType::AABB: {
                return getAABB_AABBCollisionInfo(Static_PtrCastTo<AABB>(A), Static_PtrCastTo<AABB>(B));
            }
            case (uint8_t)ColliderType::OBB: {
                return getOBB_OBBCollisionInfo(Static_PtrCastTo<OBB>(A), Static_PtrCastTo<OBB>(B));
            }
            case (uint8_t)ColliderType::Sphere: {
                return getSphere_SphereCollisionInfo(Static_PtrCastTo<Sphere>(A), Static_PtrCastTo<Sphere>(B));
            }


            //多个物体
            //AABB-OBB
            case (uint8_t)ColliderType::AABB | (uint8_t)ColliderType::OBB: {
                return 
                    A->getType() == ColliderType::AABB ? 
                    getAABB_OBBCollisionInfo(Static_PtrCastTo<AABB>(A), Static_PtrCastTo<OBB>(B)) : 
                    getAABB_OBBCollisionInfo(Static_PtrCastTo<AABB>(B), Static_PtrCastTo<OBB>(A));
            }
            //Sphere-OBB
            case (uint8_t)ColliderType::Sphere | (uint8_t)ColliderType::OBB: {
                return
                    A->getType() == ColliderType::Sphere ? 
                    getSphere_OBBCollisionInfo(Static_PtrCastTo<Sphere>(A), Static_PtrCastTo<OBB>(B)) : 
                    getSphere_OBBCollisionInfo(Static_PtrCastTo<Sphere>(B), Static_PtrCastTo<OBB>(A));
            }
            //Sphere-AABB
            case (uint8_t)ColliderType::Sphere | (uint8_t)ColliderType::AABB: {
                return
                    A->getType() == ColliderType::Sphere ? 
                    getSphere_AABBCollisionInfo(Static_PtrCastTo<Sphere>(A), Static_PtrCastTo<AABB>(B)) : 
                    getSphere_AABBCollisionInfo(Static_PtrCastTo<Sphere>(B), Static_PtrCastTo<AABB>(A));
            }

        }
        return CollisionInfo();
    }

    CollisionFace ColliderTest::getCollisionFace(const glm::vec3 &normal, const glm::vec3 &Upvec) {
        //偏移内角度
        const float faceThreshold = glm::cos(glm::radians(45.0));
        glm::vec3 nor = glm::normalize(normal);
        //先判上下
        float upDot = glm::dot(nor, Upvec);
        if (upDot > faceThreshold) return CollisionFace::Top;
        if (upDot < -faceThreshold) return CollisionFace::Bottom;
        //再判左右
        auto right = glm::vec3{1, 0, 0};
        float rightDot = glm::dot(nor, right);
        if (std::abs(rightDot) > faceThreshold) 
            return (rightDot > 0.0) ? CollisionFace::Right : CollisionFace::Left;
        //最后判断前后
        auto forward = glm::vec3{0, 0, 1};
        float forwardDot = glm::dot(nor, forward);
        if (std::abs(forwardDot) > faceThreshold) 
            return (forwardDot > 0.0) ? CollisionFace::Right : CollisionFace::Left;
        
        return CollisionFace::None; 
    }


    bool ColliderTest::intersectsAABB_AABB(const AABBPtr &a, const AABBPtr &b) {
        auto aPos = a->getBoundingPos(); 
        auto bPos = b->getBoundingPos(); 
        return 
            (aPos.first.x <= bPos.second.x && aPos.second.x >= bPos.first.x) &&
            (aPos.first.y <= bPos.second.y && aPos.second.y >= bPos.first.y) &&
            (aPos.first.z <= bPos.second.z && aPos.second.z >= bPos.first.z);
    }

    bool ColliderTest::intersectsAABB_OBB(const AABBPtr &a, const OBBPtr &b) {
        auto aabb = Static_PtrCastTo<AABB>(a);
        glm::vec3 halfSize = (a->Max - a->Min) * 0.5f;
        glm::vec3 aabbCenter = a->getPosition() + (a->Min + a->Max) * 0.5f;
        auto abb_obb = std::make_shared<OBB>(
            halfSize,
            glm::mat3(1.0f)
        );
        abb_obb->setPosition(aabbCenter);
        return intersectsOBB_OBB(abb_obb, b);
    }

    bool ColliderTest::intersectsOBB_OBB(const OBBPtr &a, const OBBPtr &b) {
                 // 需要检测的分离轴：两个OBB的6个轴向 + 9个叉积轴 包括重复则 <= 15条轴
        
         //使用无序set来去重
        std::unordered_set<glm::vec3, Vec3Hash> testAxes;

        // 填充boxA的3个轴向
        testAxes.insert(a->getAxisX()); // boxA的X轴
        testAxes.insert(a->getAxisY()); // boxA的Y轴
        testAxes.insert(a->getAxisZ()); // boxA的Z轴

        // 填充boxB的3个轴向
        testAxes.insert(b->getAxisX()); // boxB的X轴
        testAxes.insert(b->getAxisY()); // boxB的Y轴
        testAxes.insert(b->getAxisZ()); // boxB的Z轴

        int index = 6;
        //计算所有的叉积轴体
        for (int i=0; i<3; i++) {
            for (int j = 0; j < 3; j++) 
                testAxes.insert(glm::cross(a->getAxis(i), b->getAxis(j)));
        }

        //轴分离计算投影半径
        for (const auto& axis : testAxes) {
            //0向量跳过
            if (glm::length(axis) < (std::numeric_limits<float>::min)()) continue;
            
            glm::vec3 normAxis = glm::normalize(axis);

            //A
            float rA = 0.0f;
            rA += a->HalfExtents.x * std::abs(glm::dot((a->getAxisX()), normAxis));
            rA += a->HalfExtents.y * std::abs(glm::dot((a->getAxisY()), normAxis));
            rA += a->HalfExtents.z * std::abs(glm::dot((a->getAxisZ()), normAxis));

            //B;
            float rB = 0.0f;
            rB += b->HalfExtents.x * std::abs(glm::dot((b->getAxisX()), normAxis));
            rB += b->HalfExtents.y * std::abs(glm::dot((b->getAxisY()), normAxis));
            rB += b->HalfExtents.z * std::abs(glm::dot((b->getAxisZ()), normAxis));

            //中心点距离
            float dis = std::abs(glm::dot(b->getPosition() - a->getPosition(), normAxis));

            if (dis > rA + rB + 0.001f) return false; 
        }

        return true;
    }

    bool ColliderTest::intersectsSphere_AABB(const SpherePtr &a, const AABBPtr &b) {
        auto [minB, maxB] = b->getBoundingPos();
        glm::vec3 closestPoint = glm::clamp(a->getPosition(), minB, maxB);
        return glm::length(a->getPosition() - closestPoint) <= a->getRadius();
    }

    bool ColliderTest::intersectsSphere_OBB(const SpherePtr &a, const OBBPtr &b) {
         // 转换到OBB局部空间
        glm::vec3 localSpherePos = glm::transpose(b->Rotation) * (a->getPosition() - b->getPosition());
        // 在局部空间中找最近点
        glm::vec3 closestPointLocal = glm::clamp(localSpherePos, -b->HalfExtents, b->HalfExtents);
        // 计算距离
        glm::vec3 closestPointWorld = b->getPosition() + b->Rotation * closestPointLocal;
        return glm::length(a->getPosition() - closestPointWorld) <= a->getRadius();
    }

    bool ColliderTest::intersectsSphere_Sphere(const SpherePtr &a, const SpherePtr &b) {
        float distance = glm::length(a->getPosition() - b->getPosition());
        return distance <= (a->getRadius() + b->getRadius());
    }

    bool ColliderTest::intersectsCylinder_Cylinder(const CylinderPtr &a, const CylinderPtr &b) {
        //获取上轴
        auto aUp = a->getUpDirection();
        auto bUp = b->getUpDirection();   
        //获取中心点
        glm::vec3 aTop = a->getTopCenter();
        glm::vec3 aBottom = a->getBottomCenter();
        glm::vec3 bTop = b->getTopCenter();
        glm::vec3 bBottom = b->getBottomCenter();
        //计算A到B的轴距离
        glm::vec3 AtoB = b->getPosition() - a->getPosition();
        
        //计算A半长高度
        float AhalfHmin = -a->getHeight() * 0.5f;
        float AhalfHmax = a->getHeight() * 0.5f;
        //将AB距离投影到A物体为坐标系下
        float bProjOnA = glm::dot(AtoB, aUp);
        //将B物体的高半长投影到A物体的坐标系下 
        float bHalfOnA = b->getHeight() * 0.5 * glm::abs(glm::dot(aUp, bUp));
        //
        if (bProjOnA - bHalfOnA > AhalfHmax || bProjOnA + bHalfOnA < AhalfHmin) return false;

        glm::vec3 crossAB = glm::cross(aUp, bUp);
        //两柱体的上轴平行
        if (glm::length(crossAB) < std::numeric_limits<float>::epsilon()) {
            glm::vec3 perpendic = AtoB - aUp * glm::dot(AtoB, aUp);
            return glm::length(perpendic) <= a->getRadius() + b->getRadius();
        }

        float dis = std::abs(glm::dot(AtoB, glm::normalize(crossAB)));
        return dis <= (a->getRadius() + b->getRadius());
        
    }

    bool ColliderTest::intersectsCylinder_AABB(const CylinderPtr &a, const AABBPtr &b) {
        auto [minB, maxB] = b->getBoundingPos();
        auto boxCenter = (minB + maxB) * 0.5f;
        auto halfExtens = (maxB - minB) * 0.5f;

        glm::vec3 localCylinderPos = a->getPosition() - boxCenter;
        glm::vec3 aUp = a->getUpDirection();

        //转化到AABB空间中进行轴检查
        for (int i=0; i<3; i++) {
            glm::vec3 axis = glm::vec3(0.0f);
            axis[i] = 1.0f;

            float cylinderProj = std::abs(glm::dot(localCylinderPos, axis)) + 
                    a->getRadius() * std::sqrt(1.0f - glm::dot(aUp, axis) * glm::dot(aUp, axis)) +
                    a->getHeight() * 0.5f * std::abs(glm::dot(aUp, axis));
            float boxProj = halfExtens[i];
            if (cylinderProj > boxProj) return false;
        }

        return true;
    }

    bool ColliderTest::intersectsCylinder_OBB(const CylinderPtr &a, const OBBPtr &b) {
        glm::mat3 inv = glm::transpose(b->getRotation());
        glm::vec3 localCylinderPos = inv * (a->getPosition() - b->getPosition());
        glm::vec3 localCylinder = b->getHalfExtens();
        glm::vec3 localCylinderUp = inv * a->getUpDirection();
        for (int i=0; i<3; i++) {
            glm::vec3 axis = glm::vec3(0.0f);
            axis[i] = 1.0f;

            float cylinderProj = std::abs(glm::dot(localCylinderPos, axis)) +
                a->getRadius() * std::sqrt(1.0f - glm::dot(localCylinderUp, axis)*glm::dot(localCylinderUp,axis)) +
                a->getHeight() * 0.5 * std::abs(glm::dot(localCylinderUp,axis));
            float obbProj = b->getHalfExtens()[i];
            if (cylinderProj > obbProj) return false;
        }

        return true;
    }

    bool ColliderTest::intersectsCylinder_Sphere(const CylinderPtr &a, const SpherePtr &b) {
        glm::vec3 toSphere = b->getPosition() - a->getPosition();
        glm::vec3 projection = a->getUpDirection() * glm::dot(toSphere, a->getUpDirection());
        glm::vec3 perpendicular = toSphere - projection;

        float halfH = a->getHeight() * 0.5f;
        float projectedHeight = glm::dot(toSphere, a->getUpDirection());
        //在外部
        if (std::abs(projectedHeight) > halfH + b->getRadius()) return false;
        
        float distanceToAxis = glm::length(perpendicular);
        float minDistance = a->getRadius();
        if (std::abs(projectedHeight) <= halfH) return distanceToAxis <= minDistance + b->getRadius();
        
        glm::vec3 capCenter = a->getPosition() + a->getUpDirection() * (projectedHeight > 0 ? halfH : -halfH);
        float distanceToCap = glm::length(a->getPosition()- capCenter);
        
        return distanceToCap <= minDistance + b->getRadius();
    }

    CollisionInfo ColliderTest::getAABB_AABBCollisionInfo(const AABBPtr &a, const AABBPtr &b) {
        CollisionInfo info;
        auto [minA, maxA] = a->getBoundingPos();
        auto [minB, maxB] = b->getBoundingPos();

        // 检查各轴重叠
        bool overlapX = (maxA.x >= minB.x) && (minA.x <= maxB.x);
        bool overlapY = (maxA.y >= minB.y) && (minA.y <= maxB.y);
        bool overlapZ = (maxA.z >= minB.z) && (minA.z <= maxB.z);

        info.IsColliding = overlapX && overlapY && overlapZ;

        if (info.IsColliding) {
            // 计算穿透深度和碰撞法线
            glm::vec3 overlap(
                (std::min)(maxA.x, maxB.x) - (std::max)(minA.x, minB.x),
                (std::min)(maxA.y, maxB.y) - (std::max)(minA.y, minB.y),
                (std::min)(maxA.z, maxB.z) - (std::max)(minA.z, minB.z)
            );
            
            // 找到最小重叠的轴
            if (overlap.x < overlap.y && overlap.x < overlap.z) {
                info.Depth = overlap.x;
                info.collisionNormal = glm::vec3((minA.x < minB.x) ? -1.0f : 1.0f, 0.0f, 0.0f);
            } else if (overlap.y < overlap.z) {
                info.Depth = overlap.y;
                info.collisionNormal = glm::vec3(0.0f, (minA.y < minB.y) ? -1.0f : 1.0f, 0.0f);
            } else {
                info.Depth = overlap.z;
                info.collisionNormal = glm::vec3(0.0f, 0.0f, (minA.z < minB.z) ? -1.0f : 1.0f);
            }

               // 计算主要碰撞点（两个AABB的中心点连线与表面的交点）
            glm::vec3 centerA = (minA + maxA) * 0.5f;
            glm::vec3 centerB = (minB + maxB) * 0.5f;
            glm::vec3 direction = glm::normalize(centerB - centerA);
            info.collisionPoint = centerA + direction * (glm::length(maxA - minA) * 0.5f);


            info.collisionFace = getCollisionFace(info.collisionNormal);
        }
        info.colliderType = "AABB - AABB";
        return info;
    }
    CollisionInfo ColliderTest::getAABB_OBBCollisionInfo(const AABBPtr &a, const OBBPtr &b) {
        CollisionInfo info;
        auto half = (a->Max - a->Min) * 0.5f;
        auto center = a->mPosition + (a->Min + a->Max) * 0.5f;
        
        auto aabbObb = std::make_shared<OBB>(half, glm::mat3(1.0f));
        aabbObb->setPosition(center);

        info = getOBB_OBBCollisionInfo(aabbObb, b);
        info.colliderType = "AABB - OBB";
        return info;
    }
    CollisionInfo ColliderTest::getOBB_OBBCollisionInfo(const OBBPtr &a, const OBBPtr &b) {
        CollisionInfo info;
        info.IsColliding = false;
        info.Depth = (std::numeric_limits<float>::max)();
         //使用无序set来去重
        std::unordered_set<glm::vec3, Vec3Hash> testAxes;

        // 填充boxA的3个轴向
        testAxes.insert(a->getAxisX()); // boxA的X轴
        testAxes.insert(a->getAxisY()); // boxA的Y轴
        testAxes.insert(a->getAxisZ()); // boxA的Z轴

        // 填充boxB的3个轴向
        testAxes.insert(b->getAxisX()); // boxB的X轴
        testAxes.insert(b->getAxisY()); // boxB的Y轴
        testAxes.insert(b->getAxisZ()); // boxB的Z轴

        int index = 6;
        //计算所有的叉积轴体
        for (int i=0; i<3; i++) {
            for (int j = 0; j < 3; j++) 
                testAxes.insert(glm::cross(a->getAxis(i), b->getAxis(j)));
        }

                //轴分离计算投影半径
        for (const auto& axis : testAxes) {
            //0向量跳过
            if (glm::length(axis) < (std::numeric_limits<float>::min)()) continue;
            
            glm::vec3 normAxis = glm::normalize(axis);

            //A
            float rA = 0.0f;
            rA += a->HalfExtents.x * std::abs(glm::dot((a->getAxisX()), normAxis));
            rA += a->HalfExtents.y * std::abs(glm::dot((a->getAxisY()), normAxis));
            rA += a->HalfExtents.z * std::abs(glm::dot((a->getAxisZ()), normAxis));

            //B;
            float rB = 0.0f;
            rB += b->HalfExtents.x * std::abs(glm::dot((b->getAxisX()), normAxis));
            rB += b->HalfExtents.y * std::abs(glm::dot((b->getAxisY()), normAxis));
            rB += b->HalfExtents.z * std::abs(glm::dot((b->getAxisZ()), normAxis));

            //中心点距离
            float dis = glm::dot(b->getPosition() - a->getPosition(), normAxis);
            float overlap = (rA + rB) - std::abs(dis);
            if (overlap <= 0.001f) {
                info.IsColliding = false;
                return info;
            }; 

            if (overlap < info.Depth) {
                info.Depth = overlap;
                info.collisionNormal = normAxis * (dis < 0 ? -1.0f : 1.0f); 
            }

        }

        info.IsColliding = 1;

        if (info.IsColliding) {
            glm::vec3 direction = glm::normalize(b->getPosition() - a->getPosition());
            info.collisionPoint = a->getPosition() + direction * (glm::length(a->HalfExtents));
        }

        glm::vec3 localNormal = glm::transpose(glm::orthonormalize(a->Rotation)) * info.collisionNormal;
        info.collisionFace = getCollisionFace(localNormal);
        info.colliderType = "OBS - OBS";

        return info;
    }

    CollisionInfo ColliderTest::getSphere_AABBCollisionInfo(const SpherePtr &a, const AABBPtr &b) {
        CollisionInfo info;
        auto [minB, maxB] = b->getBoundingPos();
        // 找到AABB上距离球心最近的点
        glm::vec3 closestPoint = glm::clamp(a->mPosition, minB, maxB);
        // 计算距离
        float distance = glm::length(a->mPosition - closestPoint);
        info.IsColliding = distance <= a->Radius;
        if (info.IsColliding) {
            info.Depth = a->Radius - distance;
            info.collisionNormal = glm::normalize(a->mPosition - closestPoint);
            info.collisionPoint = closestPoint;
            info.collisionFace = getCollisionFace(info.collisionNormal);
        }
        info.colliderType = "Sphere - AABB";
        return info;
    }

    CollisionInfo ColliderTest::getSphere_OBBCollisionInfo(const SpherePtr &a, const OBBPtr &b) {
        CollisionInfo info;
        // 将球心转换到OBB的局部空间
        glm::vec3 localSpherePos = glm::transpose(b->getRotation()) * (a->mPosition - b->getPosition());
        // 在局部空间中找到最近点（类似于AABB）
        glm::vec3 closestPointLocal = glm::clamp(localSpherePos, -b->getHalfExtens(), b->getHalfExtens());
        // 转换回世界空间
        glm::vec3 closestPointWorld = b->getPosition() + b->getRotation() * closestPointLocal;
        // 计算距离
        float distance = glm::length(a->mPosition - closestPointWorld);
        info.IsColliding = distance <= a->Radius;
        if (info.IsColliding) {
            info.Depth = a->Radius - distance;
            info.collisionNormal = glm::normalize(a->mPosition - closestPointWorld);
            info.collisionPoint = closestPointWorld;
            info.collisionFace = getCollisionFace(info.collisionNormal);
        }
        info.colliderType = "Sphere - OBB";
        return info;
    }

    CollisionInfo ColliderTest::getSphere_SphereCollisionInfo(const SpherePtr &a, const SpherePtr &b) {
        CollisionInfo info;
        float distance = glm::length(a->mPosition - b->mPosition);
        float totalRadius = a->Radius + b->Radius;
        info.IsColliding = distance <= totalRadius;
        if (info.IsColliding) {
            info.Depth = totalRadius - distance;
            info.collisionNormal = glm::normalize(a->mPosition - b->mPosition);
            info.collisionPoint = a->mPosition - info.collisionNormal * a->Radius;
            info.collisionFace = getCollisionFace(info.collisionNormal);
        }
        info.colliderType = "Sphere - Sphere";
        return info;
    }
    CollisionInfo ColliderTest::getCylinder_AABBCollisionInfo(const CylinderPtr &a, const AABBPtr &b)
    {
        return CollisionInfo();
    }
    CollisionInfo ColliderTest::getCylinder_OBBCollisionInfo(const CylinderPtr &a, const OBBPtr &b)
    {
        return CollisionInfo();
    }
    CollisionInfo ColliderTest::getCylinder_SphereCollisionInfo(const CylinderPtr &a, const SpherePtr &b)
    {
        return CollisionInfo();
    }
}