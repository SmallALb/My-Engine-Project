#pragma once
#include "Light.h"


namespace FISH {
    //单向光源
    class DirectionLight : public Light {
    public:
        DirectionLight();
        ~DirectionLight();

        //获取光源的方向
        glm::vec3 getLightDir() const;

        //设置照亮的点，以此来改变方向
        inline void setLookAtPoint(const glm::vec3& point) {mlookPoint = point;}

        //直接进行方向的设置
        inline void setLightDir(const glm::vec3& dir);
        
        //获取看向的点
        inline const glm::vec3& getLookPoint() const {return mlookPoint;}
    private:
        glm::vec3       mlookPoint{0.0f};
    };
}