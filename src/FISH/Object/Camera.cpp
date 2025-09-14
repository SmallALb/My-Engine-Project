#include "fspcs.h"
#include "FISH/Log.h"
#include "Camera.h"
#include "perspectiveCamera.h"

namespace FISH {
    Camera::Camera() {
        mType = ObjType::Camera;
        updateVectors();
    }

    glm::mat4 Camera::getViewMatrix() {
        auto pos = getPosition();
        return glm::lookAt(pos, mLookAtpoint, mUp);
    }


    Camera *Camera::CreateCamera(CameraType cameratype, const std::string &name) {
        switch(cameratype) {
            case CameraType::PERSPECTIVE : return new perspectiveCamera(name); 
        }
        FS_ERROR("未知摄像机类型");
        return nullptr;
    }

    void Camera::setLookAt(const glm::vec3 &pos) {
        mLookAtpoint = pos;
        updateVectors();
    }

    void Camera::setPosition(const glm::vec3 &position) {
        mPosition = position;
        updateVectors();
    }

    void Camera::updateVectors() {
        mFront = glm::normalize(mLookAtpoint - mPosition);
        mLeft = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), mFront));
        mUp = glm::normalize(glm::cross(mFront, mLeft));
    }
}