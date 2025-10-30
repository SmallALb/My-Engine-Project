#include "Ray.h"



namespace FISH {
    bool RayCollisionAtPoint(const Ray &ray, const glm::vec3 &center, float size) {
        auto oc = ray.position - center;
        float l_d = glm::dot(oc, ray.direction);
        float l_l = glm::dot(oc, oc);

        return (l_l - l_d* l_d) <= ( size * size);
    }

    bool RayCollisionAtBox(const Ray &ray, const glm::vec3 &center, float size) {
        glm::vec3 Min = center - size / 2.0f;
        glm::vec3 Max = center + size /2.0f;
        float tMin = 0.0f;
        float tMax = 3.402823466e+38F;
        for (int i=0; i<3; i++) 
            if (glm::abs(ray.direction[i]) < 0.000f){
                if (ray.position[i] < Min[i] || ray.position[i] > Max[i]) return false;
            } else {
                float invDir = 1.0f / ray.direction[i];
                float t1 = (Min[i] - ray.position[i]) * invDir;
                float t2 = (Max[i] - ray.position[i]) * invDir;
                if (t1 > t2) std::swap(t1, t2);
                tMin = t1 > tMin ? t1 : tMin;
                tMax = t2 < tMax ? t2 : tMax;
                if (tMin > tMax) return false;
            }
        return true;
    }

    bool RayCollisionAtBox(const Ray &ray, const glm::vec3 &Min, const glm::vec3 &Max) {
        float tMin = 0.0f;
        float tMax = 3.402823466e+38F;
        for (int i=0; i<3; i++) 
            if (glm::abs(ray.direction[i]) < 0.000f){
                if (ray.position[i] < Min[i] || ray.position[i] > Max[i]) return false;
            } else {
                float invDir = 1.0f / ray.direction[i];
                float t1 = (Min[i] - ray.position[i]) * invDir;
                float t2 = (Max[i] - ray.position[i]) * invDir;
                if (t1 > t2) std::swap(t1, t2);
                tMin = t1 > tMin ? t1 : tMin;
                tMax = t2 < tMax ? t2 : tMax;
                if (tMin > tMax) return false;
            }
        return true;
    }
}