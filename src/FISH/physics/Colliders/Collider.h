#pragma once

#include <glm/glm.hpp>
#include "FISH/Renderer/VertexArray.h"

namespace FISH {
    
    //事先声明
    class ICollider;
    class AABB;
    class OBB;
    class Sphere;
    class Cylinder;
    class Capsule;
    class Plane;

    //碰撞体指针对象
    using ColliderPtr = std::shared_ptr<ICollider>;
    //AABB碰撞体指针
    using AABBPtr = std::shared_ptr<AABB>;
    //OBB碰撞体指针
    using OBBPtr = std::shared_ptr<OBB>;
    //球形碰撞体指针
    using SpherePtr = std::shared_ptr<Sphere>;
    //圆柱体碰撞指针
    using CylinderPtr = std::shared_ptr<Cylinder>;
    //胶囊体碰撞指针
    using CapsulePtr = std::shared_ptr<Capsule>;
    //平面胶囊体
    using PlanePtr = std::shared_ptr<Plane>;

    //类型
    enum class ColliderType : uint8_t {
        None = 0,
        //普通盒体包围盒子
        AABB = 0b01,
        //可旋转包围盒
        OBB = 0b10,
        //球体
        Sphere = 0b100,
        //圆柱体
        Cylinder = 0b1000,
        //胶囊体
        Capsule = 0b10000,
        //平面
        Plan = 0b100000
    };

     //盒体接口
    class API_ ICollider {
    public:
        //计算包围体积
        virtual float volume() const = 0;
        //包围盒大小
        virtual glm::vec3 size() const  = 0;
        //转换
        virtual void transform(const glm::mat4 &transform) = 0;
        //获取碰撞体类型
        virtual inline ColliderType getType() const {return ColliderType::None;}
        //获取框体顶点信息
        virtual std::shared_ptr<VertexArray> getVertices() const = 0;
        //获取位置
        inline const glm::vec3& getPosition() const {return mPosition;} 
        //设置位置
        inline void setPosition(const glm::vec3& pos) {mPosition = pos;}
    protected:
        //位置
        glm::vec3 mPosition{0.0};
    };

}