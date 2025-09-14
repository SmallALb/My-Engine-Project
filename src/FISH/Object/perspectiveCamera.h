#pragma once

#include "FISH/Core.h"
#include "Camera.h"

namespace FISH {
    //透视相机
    class perspectiveCamera : public Camera {
    public:
        perspectiveCamera(const string& name);

        ~perspectiveCamera() {}
        //初始化数据传入
        void init(float f, float a, float n, float far_);

        //设置屏幕的宽高比
        inline void setAspect(float val) {mAspect = val;}

        //设置视张角
        inline void setFovy(float angle) {mFovy = angle;}

        //获取透视投影
        virtual glm::mat4 getProjectMatrix() override;
        
        //改变视场角
        virtual void scale(float deltaScale) override;
        
        //更新相机
        virtual void update();

        //设置速度
        inline void setSpeed(float speed) {mSpeed = speed;}

        //设置灵敏度
        inline void setSensitivity(float Sensitivity) {mSensitivity = Sensitivity;}

        //获取速度
        inline float getSpeed() const {return mSpeed;}

        //获取灵敏度
        inline float getSensitivity() const {return mSensitivity;}

        //获取当前相机在2D视口中的位置x
        inline void setCurrentX(double x) {mCurrentX = x;}

        //获取当前相机在2D视口中的位置y
        inline void setCurrentY(double y) {mCurrentY = y;}


        //从指定位置中设置相机看向视角
        void setLookAtFromMousePosition(const std::pair<float, float>& pos);
    private:
        //视角上/下
        void pitch(float angle);
        //视角左/右
        void yaw(float angle);
    private:
        float           mFovy = 0.0f;
        float           mAspect = 0.0f;
        float           mPitch = 0.0f;
        float           mSpeed = 0.03f;
        double          mCurrentX = 0.0, mCurrentY = 0.0;
        float           mSensitivity = 0.03f;
        float           mAccumulatedTime = 0.0f;
    };
}