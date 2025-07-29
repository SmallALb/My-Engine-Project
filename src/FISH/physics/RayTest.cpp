#include "fspcs.h"
#include "FISH/Log.h"
#include "FISH/Renderer/API.h"
#include "FISH/Object/Object.h"
#include "FISH/Object/Camera.h"
#include "FISH/Renderer/Buffer.h"
#include "FISH/Renderer/VertexArray.h"
#include "FISH/Renderer/RenderElement.h"
#include "ObjectCast.h"
#include "FISH/Time.h"
#include "RayTest.h"

namespace FISH {
    bool RayTest::IsRayCastObj(const glm::vec3 &RayPos, const glm::vec3 &RayDir, const Object3DPtr &obj, float siz, CastType TYP) {
        switch(TYP) {
            case CastType::Ball:
                return BoxCast(RayPos, RayDir, obj, siz);
            case CastType::Box:
                return BallCast(RayPos, RayDir, obj, siz);
        }
        FS_ERROR("Can not find the type of Cast!");
        return false;
    }
    bool RayTest::IsRayCastGameObj(const glm::vec3 &RayPos, const glm::vec3 &RayDir, const GameObjPtr &obj, CastType TYP) {
        if (!obj || !obj->getBounds()) return false;

        if (auto aabb = PtrCastTo<AABB>(obj->getBounds())) 
            return TYP == CastType::Box ? BoxCast(RayPos, RayDir, aabb) : BallCast(RayPos, RayDir, aabb);
        else if (auto obb = PtrCastTo<OBB>(obj->getBounds()))
            return OBBCast(RayPos, RayDir, obb);

        FS_ERROR("Can not find the type of Cast!");
        return false;
    }
    bool RayTest::BoxCast(const glm::vec3 &RayPos, const glm::vec3 &RayDir, const Object3DPtr &obj, float siz)
    {
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

    bool RayTest::BallCast(const glm::vec3 &RayPos, const glm::vec3 &RayDir, const Object3DPtr &obj, float siz) {
        auto Center = obj->getPosition();
        float r = siz;

        auto oc = RayPos - Center;
        float a = glm::dot(RayDir, RayDir);
        float b = 2.0f * glm::dot(oc, RayDir);
        float c = glm::dot(oc, oc) - r * r;
        float d = b*b - 4*a*c;

        return d >= 0;
    }

    bool RayTest::BoxCast(const glm::vec3 &RayPos, const glm::vec3 &RayDir, const AABBPtr& box) {

        auto [min, max] = box->getBoundingPos();
        float tMin = 0.0f;
        float tMax = 1000.0f;

        for (int i = 0; i < 3; i++) { // 检测 X/Y/Z 轴
            if (fabs(RayDir[i]) < 0.0001f) { // 平行于轴
                if (RayPos[i] < min[i] || RayPos[i] > max[i]) 
                    return false;
            } else {
                float invDir = 1.0f / RayDir[i];
                float t1 = (min[i] - RayPos[i]) * invDir;
                float t2 = (max[i] - RayPos[i]) * invDir;
                
                if (invDir < 0.0f) std::swap(t1, t2);
                
                tMin = t1 > tMin ? t1 : tMin;
                tMax = t2 < tMax ? t2 : tMax;
                
                if (tMin > tMax) return false;
            }
        }
        return true;
    }

    bool RayTest::BallCast(const glm::vec3 &RayPos, const glm::vec3 &RayDir, const AABBPtr& box) {
        auto [min, max] = box->getBoundingPos();
        glm::vec3 center = (min + max) * 0.5f;
        float radius = glm::length(max - min) * 0.5f;
        
        glm::vec3 oc = RayPos - center;
        float a = glm::dot(RayDir, RayDir);
        float b = 2.0f * glm::dot(oc, RayDir);
        float c = glm::dot(oc, oc) - radius * radius;
        float d = b * b - 4 * a * c;
        
        return d >= 0;
    }

    bool RayTest::OBBCast(const glm::vec3 &RayPos, const glm::vec3 &RayDir, const OBBPtr &box) {
        glm::vec3 half = box->size() / 2.0f;

        glm::mat3 rotM = glm::mat3(box->getAxisX(), box->getAxisY(), box->getAxisZ());
        auto invRotM = glm::transpose(rotM);

        glm::vec3 RayOpos = invRotM * (RayPos - box->getPosition());
        glm::vec3 RayOdir = invRotM * RayDir;


        float tMin = 0.0f;
        float tMax = 1000.0f;

        for (int i = 0; i < 3; i++) {
            if (fabs(RayOpos[i]) < 0.0001f) {
                if (RayOpos[i] < -half[i] || RayOpos[i] > half[i])
                    return false;
            } else {
                float invDir = 1.0f / RayOdir[i];
                float t1 = (-half[i] - RayOpos[i]) * invDir;
                float t2 = (half[i] - RayOpos[i]) * invDir;
                
                if (invDir < 0.0f) std::swap(t1, t2);
                
                tMin = (glm::max)(t1, tMin);
                tMax = (glm::min)(t2, tMax);
                
                if (tMin > tMax) return false;
            }
        }

        return true;
    }

    glm::vec3 MousePosToWorldPos_InViewPlane(std::pair<double, double> mousePos, CameraPtr camera, float width, float height) {
        auto [Ox, Oy] = mousePos;
        auto pos = ToNDC({Ox, Oy}, width, height);
        glm::vec4 rayClip(pos, -1.0f, 1.0f);
        glm::mat4 invVP = glm::inverse(camera->getProjectMatrix() * camera->getViewMatrix());
        auto res = invVP * rayClip;
        if (camera->getCameraType() == CameraType::PERSPECTIVE) res /= res.w;

        return glm::vec3(res);
    }
    glm::vec3 MousePosToWorldPos_AtDistance(std::pair<double, double> mousePos, CameraPtr camera, float width, float height, float distance) {
        auto [Ox, Oy] = mousePos;
        auto pos = ToNDC({Ox, Oy}, width, height);
        glm::vec4 rayClip(pos, -1.0f, 1.0f);
        glm::mat4 invVP = glm::inverse(camera->getProjectMatrix() * camera->getViewMatrix());
        auto res = invVP * rayClip;
        if (camera->getCameraType() == CameraType::PERSPECTIVE) res /= res.w;

        auto dir = glm::normalize(glm::vec3(res) - camera->getPosition());

        return camera->getPosition() + dir * distance;
    }

    glm::vec3 getRayDirectionFromMouse(std::pair<double, double> mousePos, CameraPtr camera, float width, float height)
    {
        auto Pos = MousePosToWorldPos_InViewPlane(mousePos, camera, width, height);
        return glm::normalize(Pos-camera->getPosition());
    }
}