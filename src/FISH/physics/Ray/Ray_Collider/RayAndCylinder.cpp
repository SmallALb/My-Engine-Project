#include "fspcs.h"
#include "FISH/Renderer/Buffer.h"
#include "FISH/physics/Colliders/Collider.h"
#include "FISH/physics/Colliders/Cylinder.h"
#include "RayAndCylinder.h"

bool FISH::RayAndCollider::RayAndCylinder(const Ray& ray, const CylinderPtr& cylinder) {
    //获取信息
    glm::vec3 center = cylinder->getPosition();
    float r = cylinder->getRadius();
    float Halfh = cylinder->getHeight() * 0.5f;
    glm::vec3 upDir = cylinder->getUpDirection();

    glm::vec3 bottomCenter = center - upDir * Halfh;
    glm::vec3 topCenter = center + upDir * Halfh;

    //转换到圆柱坐标中
    glm::vec3 localRayPos = ray.position - center;

    //获取投影
    float rayDotAxis = glm::dot(ray.direction, upDir);
    float posDotAxis = glm::dot(localRayPos, upDir);

    //计算交向量
    glm::vec3 crossDirAxis = glm::cross(ray.direction, upDir);
    glm::vec3 crossPosAxis = glm::cross(localRayPos, upDir);

    float A = glm::dot(crossDirAxis, crossPosAxis);
    float B = 2.0f * glm::dot(crossDirAxis, crossPosAxis);
    float C = glm::dot(crossDirAxis, crossPosAxis) - r * r;


    float d = B*B - 4.0f * A * C;

    if (d < 0.0f) return false;

    d = sqrt(d);

    float t0 = (-B - d) / (2.0f *A);
    float t1 = (-B + d) / (2.0f *A);

    if (t0 > t1) std::swap(t0, t1);

    for (auto t : {t0, t1}) {
        if (t <0.0f) continue;

        glm::vec3 hitP = ray.position + ray.direction * t;
        float hitH = glm::dot(hitP - center, upDir);

        if (glm::abs(hitH) <= Halfh) return true;
    }

    if (rayDotAxis != 0.0f) {
        float tBottom = (glm::dot(bottomCenter - ray.position, upDir)) / rayDotAxis;
        if (tBottom >= 0.0f) {
            glm::vec3 bottomH = ray.position + ray.position * tBottom;
            if (glm::length(bottomH - bottomCenter) <= r) return true;
        }

        float tTop = (glm::dot(topCenter - ray.position, upDir)) / rayDotAxis;
        if (tTop >= 0.0f) {
            glm::vec3 topH = ray.position + ray.direction * tTop;
            if (glm::length(topH - topCenter) <= r) return true;
        }
    }

    return false;
} 