#include "fspcs.h"
#include "Movement.h"

namespace FISH {
    void Movement::start() {
        misActive = 1;
        misPaused = 0;
    }

    void Movement::stop() {
        misActive = 0;
        misPaused = 0;
        mVelocity = glm::vec3(0.0);
    }

    void Movement::pause() {
        if (misActive) misPaused = 1;
    }
    
    void Movement::resume() {
        if (misActive) misPaused = 0;
    }

    void Movement::update(float dt) {
        if (!misActive || misPaused) {
            mPosD = glm::vec3(0.0);
            return;
        }

        switch(mType) {
            case MovementType::LINEAR:
                updateLinearMovement(dt); break;
            case MovementType::PHYSICS:
                updatePhysicsMovement(dt); break;                
            case MovementType::CURVED:
                updateCurvedMovement(dt); break;
            default:
                mPosD = glm::vec3(0.0);
        }
    }

    void Movement::applyImpulse(const glm::vec3 &impulse) {
        mVelocity += impulse;
    }
    void Movement::addForce(const glm::vec3 &force) {
        mforce += force;
    }
    void Movement::resetForces() {
        mforce = glm::vec3(0.0);
    }
    float Movement::calculateYaw() const {
        return atan2(mDirection.x , mDirection.z);
    }
    float Movement::calculatePitch() const {
        return atan2(mDirection.y, sqrt(mDirection.x * mDirection.x + mDirection.z * mDirection.z));;
    }
    float Movement::calculateZRotation() const {
        return atan2(mDirection.y, mDirection.x);
    }
    //*
    float Movement::calculateRoll() const {
        return 0.0f;
    }
    void Movement::updateLinearMovement(float dt) {
        mPosD = mVelocity * dt;
    }
    void Movement::updatePhysicsMovement(float dt) {

        mVelocity += mAdd * dt;

        mVelocity += mforce * dt;

        mVelocity.y -= mgravity * dt;

        if (mdamping > 0.0) mVelocity *= (1.0f - mdamping * dt);

        float speed = glm::length(mVelocity);
        if (speed > MaxSpeed * MaxSpeed) mVelocity = glm::normalize(mVelocity) * MaxSpeed;

        if (speed > 0.00001f) mDirection = glm::normalize(mVelocity);

        mPosD = mVelocity * dt;
        mSpeed = speed;

        mforce = glm::vec3(0.0);
    }
    void Movement::updateCurvedMovement(float dt) {
        updatePhysicsMovement(dt);
    }
}