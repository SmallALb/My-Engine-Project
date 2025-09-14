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
            updateVectors();
    }

    void perspectiveCamera::scale(float deltaScale) {
            mFovy += deltaScale;
            // 限制视场角范围，避免畸变
            if (mFovy < 1.0f) mFovy = 1.0f;
            if (mFovy > 90.0f) mFovy = 90.0f;
    }

    void perspectiveCamera::setLookAtFromMousePosition(const std::pair<float, float> &pos) {
        auto& [xpos, ypos] = pos;
        float dY = (ypos - mCurrentY) * mSensitivity;
        float dX = (xpos - mCurrentX) * mSensitivity;
        //FS_INFO("{0}", dt);
        yaw(-dX);
        pitch(dY);
        mCurrentX = xpos;
        mCurrentY = ypos;

        mLookAtpoint = mPosition + mFront;
    }


    void perspectiveCamera::update() {
        if (!IsControl) return;

        mAccumulatedTime += Time::DeltaTime; 
        while(mAccumulatedTime >= Time::Step) {
            //鼠标位置
            setLookAtFromMousePosition(Input::GetMousePos());
      
            //键盘响应

            glm::vec3 dir(0.0);
            if (Input::IsKeyPressed(FS_KEY_W) || Input::IsKeyPressed(FS_KEY_UP))
                dir += mFront;
            if (Input::IsKeyPressed(FS_KEY_S) || Input::IsKeyPressed(FS_KEY_DOWN))
                dir -= mFront;
            if (Input::IsKeyPressed(FS_KEY_D) || Input::IsKeyPressed(FS_KEY_RIGHT))
                dir -= mLeft;
            if (Input::IsKeyPressed(FS_KEY_A) || Input::IsKeyPressed(FS_KEY_LEFT))
                dir += mLeft;
            
            if (glm::length(dir) != 0) {
                auto locaDir = glm::inverse(glm::mat3(getModelMatrix())) * dir;
                mPosition += glm::normalize(locaDir) * mSpeed;
                mLookAtpoint = mPosition + mFront;
                updateVectors();
            }

            mAccumulatedTime -= Time::Step;
        }
        //FS_INFO("pos :({0}, {1}, {2})", mPosition[0], mPosition[1], mPosition[2]);
        
    }



    void perspectiveCamera::pitch(float angle) {
        mPitch += angle;
        if (mPitch > 89.5f || mPitch < -89.5f) {
            mPitch -= angle;
            return;
        }
        
        // 围绕左向量旋转前向向量
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), mLeft);
        mFront = glm::normalize(rotation * glm::vec4(mFront, 0.0f));
        
        // 更新上向量以确保正交
        mUp = glm::normalize(glm::cross(mFront, mLeft));
    }

    void perspectiveCamera::yaw(float angle) {
        auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
	    mLeft = glm::normalize(mat * glm::vec4(mLeft, 0.0f));
        mFront = glm::normalize(mat * glm::vec4(mFront, 0.0f));
        mUp = glm::normalize(glm::cross(mFront, mLeft));
    }

    glm::mat4 perspectiveCamera::getProjectMatrix() {
        return glm::perspective(glm::radians(mFovy), mAspect, mNear, mFar);
    };

}
