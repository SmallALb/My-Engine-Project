#pragma once
#include "Object.h"
#include "FISH/Core.h"

namespace  FISH {
    //�������
    enum class CameraType {
        PERSPECTIVE,            //͸��
        ORTHOGRAPHIC            //����
    };
    //�����
    class API_ Camera : public Object3D {
    public:
        Camera();

        ~Camera() {}
        //��ȡ�ӽǾ���
        glm::mat4 getViewMatrix();
        //��ȡ͸�Ӿ���
        virtual glm::mat4 getProjectMatrix() { return glm::mat4();}
        //�����ӽ�����
        virtual void scale(float deltaScale) = 0;

        //��ȡ������
        virtual glm::vec3 getFront() const = 0;

        //�������
        static Camera* CreateCamera(CameraType cameratype = CameraType::PERSPECTIVE, const std::string& name = "Camera");

        //��ȡ�����right����
        const glm::vec3& getRightvec() const {return mRight;}

        //��ȡ�����up����
        const glm::vec3& getUpvec() const {return mUp;}

        //�������
        virtual void update() = 0;

        virtual void setLookAt(const glm::vec3& pos);

        //�鿴�Ƿ��ܹ����п���
        virtual bool IsAllowControl() const { return IsControl;}

        //�����Ƿ��ܹ�����
        virtual void setAllowedControl(bool status) { IsControl = status; }

        //��ȡ����ĵ�
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
