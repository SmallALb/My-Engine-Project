#pragma once

#include "Light.h"
#include "FISH/Core.h"

namespace FISH {
    class PointLight : public Light {
    public:
        PointLight();

        ~PointLight();

        //设置基础光亮常数
        inline void setk2(float val) { mk2 = val;}

        //设置线性衰减参数
        inline void setk1(float val) { mk1 = val;}

        //设置二次衰减参数
        inline void setkc(float val) { mkc = val;}

        //基础光亮常数
        inline float getk2() const {return mk2;}
        
        //线性衰减参数
        inline float getk1() const {return mk1;}

        //二次衰减参数
        inline float getkc() const {return mkc;}

    protected:
        float mk2{1.0f};
        float mk1{1.0f};
        float mkc{1.0f};
    };

}