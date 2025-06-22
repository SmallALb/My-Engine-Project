#pragma once

#include "PointLight.h"
#include "FISH/Core.h"

namespace FISH {
    //聚光灯类
    class SpotLight : public PointLight {
    public:
        SpotLight();

        ~SpotLight();
        
        //获取方向
        glm::vec3 getLightDir() const;

        //设置看向的点
        inline void setLookAtPoint(const glm::vec3& lookPoint) {mlookPoint = lookPoint;}

        //获取看向的点
        inline const glm::vec3& getLookPoint() const {return mlookPoint;}

        //设置主要照亮角度
        inline void setVisibleAngle(float VisibleAngle) {mVisibleAngle = VisibleAngle;}

        //设置外部衰减角度
        inline void setOuterAngle(float OuterAngle) {mOuterAngle = OuterAngle; }

        //获取主要照亮角度
        inline float getVisibleAngle() const {return mVisibleAngle;}

        //获取外部衰减角度
        inline float getOuterAngle() const {return mOuterAngle;}

        //设置方向
        void setLightDir(const glm::vec3& Dir);
    private:
        glm::vec3       mlookPoint{0.0f};
        float           mVisibleAngle{ 0.0f };
        float           mOuterAngle{ 0.0f };
    };


}