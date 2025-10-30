#pragma once

#include <glm/glm.hpp>

namespace FISH {
    
    //射线结构体
    struct Ray {
        //发出位置
        glm::vec3 position;
        //发射方向
        glm::vec3 direction;
    };

    //查看射线是否与xx点范围为xx的球体进行碰撞
    bool RayCollisionAtPoint(const Ray& ray, const glm::vec3& center, float size);
    //查看射线是否与xx点范围为xx的盒体进行碰撞
    bool RayCollisionAtBox(const Ray& ray, const glm::vec3& center, float size);
    //查看射线是否与xx点范围为xx的盒体进行碰撞 传入Min 和 Max
    bool RayCollisionAtBox(const Ray& ray, const glm::vec3& Min, const glm::vec3& Max);

}