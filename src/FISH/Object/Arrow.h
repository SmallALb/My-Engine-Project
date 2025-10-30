#pragma once

#include "Object.h"
#include "FISH/Physics/Colliders/Collider.h"
#include "FISH/Physics/Colliders/Cylinder.h"

namespace FISH {
    //箭头类
    class Arrow : public Object3D {
    public:
        OBJ_CLASS_TYPE(Arrow)

        Arrow(
            const glm::vec3 color,
            float length =2.0f, 
            float shaftRadius = 0.05f, 
            float headLength = 0.3f, 
            float headRadius = 0.1f,
            const std::string& name = "Arrow"
        );

        ~Arrow() = default;

        void setDirection(const glm::vec3 direction);
        
        glm::vec3 getDirection() const {return mDirection;};

        void setColor(const glm::vec3& color) {mColor = color;};

        void setSelected(bool selected) {mIsSelected = selected;};

        bool isSelected() const {return mIsSelected;}

        bool checkCollision(const Ray& ray);
        //*临时这样写先
        void render();
        //*临时这样写先
        void setCamera(const CameraPtr& camera);
    private:
        void updateDirection(const glm::vec3& target);
    private:
        CylinderPtr collider;
        float mlength;
        float mShaftRadius;
        float mShaftLength;
        float mHeadLength;
        float mHeadRadius;
        glm::vec3 mColor;
        bool mIsSelected{0};

        glm::vec3 mDirection;

        ShapePtr Cone;
        ShapePtr cylinder;
        //*临时存放
        CameraPtr usingCamera{nullptr};

        glm::mat4 mShaftTransform{1.0f};

        glm::mat4 mHeadTransform{1.0f};
    };
}