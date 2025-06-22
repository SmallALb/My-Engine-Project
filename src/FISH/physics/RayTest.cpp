#include "fspcs.h"
#include "FISH/Log.h"
#include "FISH/Object/Object.h"
#include "RayTest.h"

namespace FISH {
    bool RayTest::IsRayCastObj(const glm::vec3 &RayPos, const glm::vec3 &RayDir, const std::shared_ptr<FISH::Object3D> &obj, float siz, CastType TYP) {
        switch(TYP) {
            case CastType::Ball:
                return BoxCast(RayPos, RayDir, obj, siz);
            case CastType::Box:
                return BallCast(RayPos, RayDir, obj, siz);
        }
        FS_ERROR("Can not find the type of Cast!");
        return false;
    }
    bool RayTest::BoxCast(const glm::vec3 &RayPos, const glm::vec3 &RayDir, const std::shared_ptr<FISH::Object3D> &obj, float siz) {
        glm::vec3 aabbMin = obj->getPosition() - siz / 2.0f;
        glm::vec3 aabbMax = obj->getPosition() + siz / 2.0f;

        float tMin = 0.0f;
        float tMax = 1000.0f;

         for (int i = 0; i < 3; i++) { // 检测 X/Y/Z 轴
            if (fabs(RayDir[i]) < 0.0001f) { // 平行于轴
                if (RayPos[i] < aabbMin[i] || RayPos[i] > aabbMax[i]) 
                    return false;
            } else {
                float invDir = 1.0f / RayDir[i];
                float t1 = (aabbMin[i] - RayPos[i]) * invDir;
                float t2 = (aabbMax[i] - RayPos[i]) * invDir;
                if (t1 > t2) std::swap(t1, t2);
                tMin = t1 > tMin ? t1 : tMin;
                tMax = t2 < tMax ? t2 : tMax;
                if (tMin > tMax) {
                    return false;
                }
            }
        }
        return true;
    }

    bool RayTest::BallCast(const glm::vec3 &RayPos, const glm::vec3 &RayDir, const std::shared_ptr<FISH::Object3D> &obj, float siz) {
        auto Center = obj->getPosition();
        float r = siz;

        auto oc = RayPos - Center;
        float a = glm::dot(RayDir, RayDir);
        float b = 2.0f * glm::dot(oc, RayDir);
        float c = glm::dot(oc, oc) - r * r;
        float d = b*b - 4*a*c;

        return d >= 0;
    }
}