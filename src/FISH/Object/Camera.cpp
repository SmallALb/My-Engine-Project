#include "fspcs.h"
#include "FISH/Log.h"
#include "Camera.h"
#include "perspectiveCamera.h"

namespace FISH {
    Camera::Camera() {
        mType = ObjType::Camera;
    }

    glm::mat4 Camera::getViewMatrix() {
        auto pos = getPosition();
        glm::vec3 front = glm::cross(mRight, mUp);
        auto Wfront = glm::normalize(glm::vec3(getModelMatrix() * glm::vec4(front, 0.0)));
        auto Wup = glm::normalize(glm::vec3(getModelMatrix() * glm::vec4(mUp, 0.0)));
        glm::vec3 center = pos + Wfront;

        return glm::lookAt(pos, center, Wup);
    }


    Camera *Camera::CreateCamera(CameraType cameratype, const std::string &name) {
        switch(cameratype) {
            case CameraType::PERSPECTIVE : return new perspectiveCamera(name); 
        }
        FS_ERROR("未知摄像机类型");
        return nullptr;
    }

    void Camera::setLookAt(const glm::vec3 &pos) {
        auto newFront = glm::normalize(pos - mPosition);

        mRight = glm::normalize(glm::cross(mUp, newFront));
        mUp = glm::normalize(glm::cross(newFront, mRight));
    }

    glm::vec3 Camera::getLookAtPoint() const {
        glm::vec3 front = glm::cross(mRight, mUp);
        glm::vec3 center = mPosition + front;
        //FS_INFO("center :({0}, {1}, {2})", center[0], center[1], center[2]);

        return center;
    }
}