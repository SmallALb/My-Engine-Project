#pragma once
#include "Object.h"
#include "FISH/Core.h"

namespace FISH {
    enum class LightType {
        None,
        Directional,
        Ambient,
        Spot,
        Point
    };
    //灯光类
    class Light : public Object3D {
    public:
        Light();
        ~Light();

        //设置光的颜色
        virtual void setColor(const glm::vec3& color) { mColor = color; }

        //获取颜色
        virtual const glm::vec3& getColor() const { return mColor; }

        //获取光样式
        virtual LightType getLightType() const { return lightType; }

        //设置灯是否开启
        virtual void setLightOn(bool status) { LightOn = status; }

        //设置镜面强度
        virtual void setSpecularIntensity(float SpecularIntensity) {mSpecularIntensity = SpecularIntensity;}

        //获取灯光是否开启
        virtual bool getIsLightOn() { return LightOn; }

        //获取镜面强度
        virtual float getSpecularIntensity() const {return mSpecularIntensity; }

        virtual void setIntensity(float Intensity)  { mIntensity = Intensity; }

        virtual float getIntensity() const {return mIntensity; }
        OBJ_CLASS_TYPE(Light)
    
    protected:
        //是否启用
        bool LightOn{0};
        LightType lightType;
        //颜色
        glm::vec3 mColor{ 1.0f };
        //光源强度
        float mSpecularIntensity{ 1.0f };

        float mIntensity{1.0f};
    };
}