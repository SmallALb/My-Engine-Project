#include "fspcs.h"
#include "FISH/Log.h"
#include "FISH/Renderer/Buffer.h"
#include "FISH/Renderer/VertexArray.h"
#include "FISH/Renderer/RenderElement.h"
#include "FISH/Renderer/BaseShape.h"
#include "FISH/physics/Ray/Ray.h"
#include "Camera.h"
#include "FISH/Renderer/Renderers/ShapeRender.h"
#include "Arrow.h"
#include "FISH/physics/Ray/Ray_Collider/RayAndCylinder.h"


namespace FISH {

    Arrow::Arrow(
        const glm::vec3 color,
        float length, float shaftRadius,
        float headLength,
        float headRadius,
        const std::string &name): mColor(color), mlength(length), mHeadRadius(headRadius), mShaftRadius(shaftRadius), mHeadLength(headLength)
    {
        mName = name;
        mShaftLength = mlength - mHeadLength;
        mDirection = {0.0f, 1.0f, 0.0f};
        //设定几何体
        Cone = std::shared_ptr<Shape>(Shape::CreateCone(mHeadLength, mHeadRadius, 16));
        cylinder = std::shared_ptr<Shape>(Shape::CreateCylinder(mShaftLength, mShaftRadius, 16));

        //设定碰撞体
        collider = std::make_shared<Cylinder>((std::max)(mShaftRadius, mHeadRadius), mlength + mHeadLength);
        collider->setPosition(mPosition);

        mShaftTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, mShaftLength*0.5f, 0.0f));
        mHeadTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, mShaftLength + mHeadLength * 0.5f, 0.0f));
    }

    void Arrow::setDirection(const glm::vec3 direction) {
        updateDirection(direction);
        mDirection = direction;

    }

    bool Arrow::checkCollision(const Ray &ray) {
        return RayAndCollider::RayAndCylinder(ray, collider);
    }

    void Arrow::render() {
        auto model = getModelMatrix();

        Renderer::RenderShape(cylinder, mColor, ElementType::triangles, model * mShaftTransform, usingCamera);

        Renderer::RenderShape(Cone, mColor, ElementType::triangles, model * mHeadTransform, usingCamera);
    }

    void Arrow::setCamera(const CameraPtr &camera) {
        usingCamera = camera;
    }

    void Arrow::updateDirection(const glm::vec3& target) {
        glm::vec3 targetDir = glm::normalize(target);
        
        // 如果方向接近为零向量，使用默认方向
        if (glm::length(targetDir) < 0.001f) {
            targetDir = mDirection;
        }
        
        // 计算从默认方向(0,1,0)到目标方向的旋转四元数
        glm::vec3 defaultDir = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::quat rotationQuat = glm::rotation(defaultDir, targetDir);
        
        // 将四元数转换为欧拉角（注意顺序）
        glm::vec3 euler = glm::eulerAngles(rotationQuat);
        
        // 设置绝对角度（弧度转角度）
        setAngleX(glm::degrees(euler.x));
        setAngleY(-glm::degrees(euler.y));
        setAngleZ(glm::degrees(euler.z));
        
        // 更新碰撞体
        collider->setRotation(glm::mat3_cast(rotationQuat));
        collider->setPosition(getPosition());
        
       
    }

}