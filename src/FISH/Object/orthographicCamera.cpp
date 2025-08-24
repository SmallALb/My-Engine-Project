#include "fspcs.h"
#include "orthographicCamera.h"

namespace FISH {
    orthographicCamera::orthographicCamera(const string &name) {
        mName = name;
        mCameraType = CameraType::ORTHOGRAPHIC;
    }

    void orthographicCamera::scale(float deltaScale) {

    }

    glm::vec3 orthographicCamera::getFront() const {
        return glm::vec3(0.0, 0.0, -1.0);
    }

    void orthographicCamera::update() {

    }

    glm::mat4 orthographicCamera::getProjectMatrix() {
        return glm::ortho(mLeft, mRight, mBottom, mTop, mNear, mFar);
    }

    void orthographicCamera::init(float l, float r, float t, float b, float n, float f) {
        mLeft = l;
        mRight = r;
        mTop = t;
        mBottom = b;
        mNear = n;
        mFar = f;
    }
}