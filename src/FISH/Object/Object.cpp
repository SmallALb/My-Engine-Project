#include "fspcs.h"
#include "FISH/Log.h"
#include "Light.h"
#include "Camera.h"
#include "perspectiveCamera.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "DirectionLight.h"
#include "Mesh.h"
#include "Scene.h"
#include "Object.h"

namespace FISH {
    namespace glm_2d {
        // 2D平移矩阵
        inline glm::mat3 translate(const glm::mat3& m, const glm::vec2& v) {
            glm::mat3 result(m);
            result[2] = m[0] * v.x + m[1] * v.y + m[2];
            return result;
        }
        
        // 2D缩放矩阵
        inline glm::mat3 scale(const glm::mat3& m, const glm::vec2& v) {
            glm::mat3 result;
            result[0] = m[0] * v.x;  // X轴缩放
            result[1] = m[1] * v.y;  // Y轴缩放
            result[2] = m[2];        // 保持位移不变
            return result;
        }
        
        // 2D旋转矩阵（绕Z轴）
        inline glm::mat3 rotate(const glm::mat3& m, float radians) {
            float c = cos(radians);
            float s = sin(radians);
            
            glm::mat3 rotation(1.0f);
            rotation[0][0] = c; rotation[0][1] = s;  // 旋转矩阵左上2x2区域
            rotation[1][0] = -s; rotation[1][1] = c;
            
            return m * rotation;  // 应用旋转
        }
    }

    ///////////////////////
    /// 3D  //////////////
    //////////////////////
    Object3D::Object3D(const string& name) {
        mName = name;
    }

    void Object3D::setPosition(const glm::vec3& pos) {
        auto parent = mParent.lock();
        if (parent) {
            glm::mat4 invParentMat = glm::inverse(parent->getModelMatrix());
            glm::vec4 localPos = invParentMat * glm::vec4(pos, 1.0f);
            mPosition = glm::vec3(localPos);
        }else {
            mPosition = pos;
        }
    }

    glm::vec3 Object3D::getPosition() const{
        return glm::vec3(getModelMatrix()[3]);
    }



    glm::mat4 Object3D::getModelMatrix() const
    {
        glm::mat4 transform(1.0f);

        glm::mat4 parentMat(1.0f);
        auto parentPtr = mParent.lock(); 
        if (parentPtr != nullptr) {
            parentMat = parentPtr->getModelMatrix();
        }

        //设置物体的各类变化
        transform = glm::scale(transform, mScale);
        transform = glm::rotate(transform, glm::radians(mAngleX), glm::vec3(1.0, 0.0, 0.0));
        transform = glm::rotate(transform, glm::radians(mAngleY), glm::vec3(0.0, 1.0, 0.0));
        transform = glm::rotate(transform, glm::radians(mAngleZ), glm::vec3(0.0, 0.0, 1.0));



        //如果他的父节点发生了变化，那他相对的也得跟着变化
        transform = parentMat * glm::translate(glm::mat4(1.0), mPosition) * transform;

        return transform;
    }

    glm::mat4 Object3D::getNormalMatrix() const {
        return glm::transpose(glm::inverse(getModelMatrix()));
    }

    void Object3D::addChild(const std::shared_ptr<Object3D>& obj) {
        obj->mParent = shared_from_this();
        mChilds.push_back(obj);
    }
    void Object3D::eraseChild(const std::shared_ptr<Object3D> &obj) {
        auto it = std::find(mChilds.begin(), mChilds.end(), obj);
        if (it == mChilds.end()) return;
        obj->mParent.reset();
        mChilds.erase(it);
    }

    ///////////////////////
    /// 2D  //////////////
    //////////////////////
    Object2D::Object2D(const std::string &Name) {
        mName = Name;
    }

    void Object2D::setPosition(const glm::vec2 &pos) {
        mPosition = pos;
    }

    glm::mat3 Object2D::getModelMatrix() const {
        glm::mat3 transform(1.0f);

        glm::mat3 parentMat(1.0f);
        auto parentPtr = mParent.lock(); 
        if (parentPtr != nullptr) {
            parentMat = parentPtr->getModelMatrix();
        }

    transform = glm_2d::scale(transform, mScale);
    transform = glm_2d::rotate(transform, glm::radians(mAngle));
    transform = glm_2d::translate(transform, mPosition);


        return transform;
    }

    void Object2D::addChild(const std::shared_ptr<Object2D>& obj) {
        obj->mParent = shared_from_this();
        mChilds.push_back(obj);
    }

    glm::vec2 Object2D::getPosition() const {
        return glm::vec2(getModelMatrix()[2]);
    }

}