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
        virtual glm::vec3 getFront() const = 0;

        //相机创建
        static Camera* CreateCamera(CameraType cameratype = CameraType::PERSPECTIVE, const std::string& name = "Camera");

        //获取相机的right向量
        const glm::vec3& getRightvec() const {return mRight;}

        //获取相机的up向量
        const glm::vec3& getUpvec() const {return mUp;}

        //更新相机
        virtual void update() = 0;

        virtual void setLookAt(const glm::vec3& pos);

        //查看是否能够进行控制
        virtual bool IsAllowControl() const { return IsControl;}

        //设置是否能够控制
        virtual void setAllowedControl(bool status) { IsControl = status; }

        //获取看向的点
        virtual glm::vec3 getLookAtPoint() const; 



        OBJ_CLASS_TYPE(Camera)


    protected:
        glm::vec3 mUp{0.0, 1.0, 0.0};
        glm::vec3 mRight{1.0, 0.0, 0.0};
        float mNear = 0.0f;
        float mFar = 0.0f;
        CameraType mCameraType;
        bool IsControl{0};
    };

} // namespace  FISH
