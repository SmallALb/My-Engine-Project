#pragma once

#include "FISH/Core.h"

namespace FISH {
    enum class MovementType {
        None,
        //静态
        STATIC,
        //线性
        LINEAR,
        //基于物理
        PHYSICS,
        //曲线
        CURVED
    };

    class API_ Movement {
    public:
        Movement() {}

        ~Movement() {}

        void start();

        void stop();

        void pause();

        void resume();

        inline void setType(MovementType type) { mType = type;};
        //设置速度
        inline void setVelocity(const glm::vec3& velocity) { mVelocity = velocity;};
        //设置方向
        inline void setDirection(const glm::vec3& dir) { mDirection = glm::normalize(dir); };
        //设置速度
        inline void setSpeed(float speed) {
            mSpeed = speed;
            mVelocity = mSpeed * mDirection;
        };

        //设置X方向的速度
        inline void setVelocity_X(float value) {mVelocity.x = value;}
        //设置Y方向的速度
        inline void setVelocity_Y(float value) {mVelocity.y = value;}
        //设置Z方向的速度
        inline void setVelocity_Z(float value) {mVelocity.z = value;}

        //设置重力
        inline void setGravity(float gravity) {mgravity = gravity;};
        //设置加速度
        inline void setAcceleration(const glm::vec3& acceleration) {mAdd = acceleration;};
        //阻尼
        inline void setDamping(float damping) {mdamping = damping;}


        bool isMoving() const {return misActive && !misPaused;}
        bool isPaused() const {return misPaused;}
        MovementType getType() const {return mType;}
        //获取当前组合速度
        const glm::vec3& getVelocity() const {return mVelocity;}
        //获取当前方向
        const glm::vec3& getCurrentDirection() const {return mDirection;}
        //获取位置增量
        glm::vec3 getPositionDelta() const {return mPosD; }
        //获取重力
        float getGravity() const {return mgravity;}
        // 更新运动状态
        void update(float dt);
        
        // 特殊运动效果
        void applyImpulse(const glm::vec3& impulse);
        void addForce(const glm::vec3& force);
        void resetForces();
        
        // 旋转相关
        float calculateYaw() const;   // 偏航角
        float calculatePitch() const; // 俯仰角
        float calculateZRotation() const;
        float calculateRoll() const;  // 翻滚角

    private:
        void updateLinearMovement(float dt);
        void updatePhysicsMovement(float dt);
        void updateCurvedMovement(float dt);
    private:
        MovementType    mType{MovementType::None};
        glm::vec3       mVelocity{0.0f};
        glm::vec3       mDirection{1.0, 0.0, 0.0};
        glm::vec3       mAdd{0.0f};
        glm::vec3       mforce{0.0f};
        glm::vec3       mPosD{0.0};

        float           mSpeed = 0.0;
        float           MaxSpeed = 100.0f;
        float           mgravity = 9.8f;
        float           mdamping = 0.0f;
        bool            misActive = false;
        bool            misPaused = false;
    };
}