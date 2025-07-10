#include "fspcs.h"
#include "FISH/input.h"
#include "perspectiveCamera.h"
#include "FISH/Keycodes.h"
#include "FISH/Log.h"
#include "Camera.h"
#include "FISH/Time.h"

namespace FISH {
    perspectiveCamera::perspectiveCamera(const string& name) {
        mName = name;
        mCameraType = CameraType::PERSPECTIVE;
    }

    void perspectiveCamera::init(float f, float a, float n, float far_) {
        	mFovy = f;
	        mAspect = a;
	        mNear = n;
	        mFar = far_;
    }

    void perspectiveCamera::scale(float deltaScale) {
            mFovy += deltaScale;
            // 限制视场角范围，避免畸变
            if (mFovy < 1.0f) mFovy = 1.0f;
            if (mFovy > 90.0f) mFovy = 90.0f;
    }

    glm::vec3 perspectiveCamera::getFront() const{ return glm::normalize(glm::cross(mRight, mUp)); }

    void perspectiveCamera::setLookAtFromMousePosition(const std::pair<float, float> &pos) {
        auto& [xpos, ypos] = pos;
        float dY = (ypos - mCurrentY) * mSensitivity;
        float dX = (xpos - mCurrentX) * mSensitivity;
        //FS_INFO("{0}", dt);
        yaw(-dX);
        pitch(dY);
        mCurrentX = xpos;
        mCurrentY = ypos;
    }

    void perspectiveCamera::update() {
        if (!IsControl) return;

        mAccumulatedTime += Time::DeltaTime; 
        while(mAccumulatedTime >= Time::Step) {
            //鼠标位置
            setLookAtFromMousePosition(Input::GetMousePos());
      
            //键盘响应
            auto front = glm::cross(mRight, mUp);
            glm::vec3 dir(0.0);
            if (Input::IsKeyPressed(FS_KEY_W) || Input::IsKeyPressed(FS_KEY_UP))
                dir += front;
            if (Input::IsKeyPressed(FS_KEY_S) || Input::IsKeyPressed(FS_KEY_DOWN))
                dir -= front;
            if (Input::IsKeyPressed(FS_KEY_D) || Input::IsKeyPressed(FS_KEY_RIGHT))
                dir -= mRight;
            if (Input::IsKeyPressed(FS_KEY_A) || Input::IsKeyPressed(FS_KEY_LEFT))
                dir += mRight;
            
            if (glm::length(dir) != 0) mPosition += glm::normalize(dir) * mSpeed;

            mAccumulatedTime -= Time::Step;
        }
        //FS_INFO("pos :({0}, {1}, {2})", mPosition[0], mPosition[1], mPosition[2]);
        
    }



    void perspectiveCamera::pitch(float angle)
    {
        mPitch += angle;
        if (mPitch > 89.5 || mPitch < -89.5) {
            mPitch -= angle;
            return;
        }
        auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), mRight);
        mUp = mat * glm::vec4(mUp, 0.0f);
    }

    void perspectiveCamera::yaw(float angle) {
        auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
	    mRight = mat * glm::vec4(mRight, 0.0);
	    mUp = mat * glm::vec4(mUp, 0.0);
    }

    glm::mat4 perspectiveCamera::getProjectMatrix() {
        return glm::perspective(glm::radians(mFovy), mAspect, mNear, mFar);
    };

}
