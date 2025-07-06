#include "fspcs.h"
#include "orthographicCamera.h"

namespace FISH {
    OrthographicCamera::OrthographicCamera(const string &name) {
        mName = name;
        mCameraType = CameraType::ORTHOGRAPHIC;
    }

    void OrthographicCamera::scale(float deltaScale) {

    }

    glm::vec3 OrthographicCamera::getFront() const {
        return glm::vec3(0.0, 0.0, -1.0);
    }

    void OrthographicCamera::update() {

    }

    glm::mat4 OrthographicCamera::getProjectMatrix() {
        return glm::ortho(mLeft, mRight, mTop, mBottom, mNear, mFar);
    }

    void OrthographicCamera::init(float l, float r, float t, float b, float n, float f) {
        mLeft = l;
        mRight = r;
        mTop = t;
        mBottom = b;
        mNear = n;
        mFar = f;
    }
}