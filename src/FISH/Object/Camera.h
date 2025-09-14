#pragma once
#include "Object.h"
#include "FISH/Core.h"

namespace  FISH {
    //相机类型
    enum class CameraType {
        PERSPECTIVE,            //透视
        ORTHOGRAPHIC            //正交
    };
    //相机类
    class API_ Camera : public Object3D {
    public:
        Camera();

        ~Camera() {}
        //获取视角矩阵
        glm::mat4 getViewMatrix();
        //获取透视矩阵
        virtual glm::mat4 getProjectMatrix() { return glm::mat4();}
        //设置视角缩放
        virtual void scale(float deltaScale) = 0;

        //获取看向方向
        virtual glm::vec3 getFront() const {return mFront;};
        
        //获取up方向
        virtual glm::vec3 getUp() const {return mUp;}

        //相机创建
        static Camera* CreateCamera(CameraType cameratype = CameraType::PERSPECTIVE, const std::string& name = "Camera");

        //获取相机的left向量
        virtual const glm::vec3& getLeft() const {return mLeft;}

        //更新相机
        virtual void update() = 0;

        virtual void setLookAt(const glm::vec3& pos);

        //查看是否能够进行控制
        virtual bool IsAllowControl() const { return IsControl;}

        //设置是否能够控制
        virtual void setAllowedControl(bool status) { IsControl = status; }
        
        //设置摄像机坐标系y向量
        virtual void setUp(const glm::vec3& upVec) {mUp = upVec;}
        //获取看向的点
        virtual glm::vec3 getLookAtPoint() const {return mLookAtpoint;}

        virtual void setPosition(const glm::vec3& position) override;
        //获取相机类型
        CameraType getCameraType() const { return mCameraType; } 

        OBJ_CLASS_TYPE(Camera)
    protected:
        virtual void updateVectors();

    protected:
        glm::vec3 mUp{0.0, 1.0, 0.0};
        glm::vec3 mLeft{1.0, 0.0, 0.0};
        glm::vec3 mLookAtpoint{0.0};
        glm::vec3 mFront{0.0};
        float mNear = 0.0f;
        float mFar = 0.0f;
        CameraType mCameraType;
        bool IsControl{0};
    };

    using CameraPtr = std::shared_ptr<Camera>;
} // namespace  FISH
