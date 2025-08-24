#pragma once

#include "FISH/Core.h"

namespace FISH {
    class ICollider;
    class AABB;
    class OBB;
        //碰撞体指针对象
    using ColliderPtr = std::shared_ptr<ICollider>;
    //AABB碰撞体指针
    using AABBPtr = std::shared_ptr<AABB>;
    //OBB碰撞体指针
    using OBBPtr = std::shared_ptr<OBB>;


    //类型
    enum class ColliderType {
        None,
        //普通盒体包围盒子
        AABB,
        //可旋转包围盒
        OBB,
        //球体
        Sphere
    };

    //碰撞面
    enum class CollisionFace {
        None,       //无碰撞面
        Top,
        Bottom,
        Front,
        Back,
        Left,
        Right
    };

        //碰撞信息
    struct CollisionInfo {
        CollisionInfo() {
            contactPoints[0] = glm::vec3{0.0};
            contactPoints[1] = glm::vec3{0.0};
            contactPoints[2] = glm::vec3{0.0};
            contactPoints[3] = glm::vec3{0.0};
        }
        bool IsColliding;           //是否被碰撞
        float Depth;                //碰撞深度
        float penetrationDepth;     //穿透深度
        CollisionFace collisionFace{CollisionFace::None};
        glm::vec3 collisionNormal;     //碰撞法线
        glm::vec3 contactPoints[4];    //触点
        glm::vec3 collisionPoint;      //主要触点
        string  colliderType;          //碰撞类型 
    };

    //盒体接口
    class ICollider {
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

        //获取碰撞信息
        virtual CollisionInfo getCollisionInfo(const ColliderPtr& other) const = 0;

    protected:
        virtual CollisionInfo CollisionWithAABB(const AABBPtr& other) const {
            return CollisionInfo();
        }

        
        virtual CollisionInfo CollisionWithOBB(const OBBPtr& other) const {
            return CollisionInfo();
        }

    protected:
        //位置
        glm::vec3 mPosition{0.0};
    };

    //碰撞盒体
    class AABB : public ICollider{
    public:
        friend class Collider;

        AABB(): Min(0), Max(0) {}

        AABB(const glm::vec3& _min, const glm::vec3& _max): Min(_min), Max(_max) {}
        
        //计算包围体积
        float volume() const override;

        inline ColliderType getType() const {return ColliderType::AABB;}

        inline glm::vec3 size() const  override {return Max - Min;}

        inline glm::vec3 getMin() const {return Min;}

        inline glm::vec3 getMax() const {return Max;}


        void transform(const glm::mat4 &transform) override;

        std::shared_ptr<VertexArray> getVertices() const override;

        //得到带位置的盒体
        std::pair<glm::vec3, glm::vec3> getBoundingPos() const {return {mPosition + Min, mPosition + Max};}

        //设置盒体min值和max值
        inline void setBounding(const glm::vec3& mi, const glm::vec3& ma) {
            Min = mi;
            Max = ma;
        } 

        CollisionInfo getCollisionInfo(const ColliderPtr& other) const override;
    
    public:
        CollisionInfo CollisionWithAABB(const AABBPtr& other) const override;
        
        CollisionInfo CollisionWithOBB(const OBBPtr& other) const override;
    private:
        //包围盒
        glm::vec3 Min;
        glm::vec3 Max;
    };


    //可旋转盒体
    class OBB : public ICollider {
    public:
        friend class Collider;


        OBB() {}

        OBB(const glm::vec3& extents, const glm::mat3& rotation) 
            : HalfExtents(extents), Rotation(rotation) {}

        ColliderType getType() const override { return ColliderType::OBB; }

        float volume() const override;

        glm::vec3 size() const override { return HalfExtents * 2.0f; }

        void transform(const glm::mat4& transform) override;

        void setRotation(const glm::mat3& rotation);

        //轴向量获取
        inline glm::vec3 getAxisX() const { return glm::normalize(Rotation[0]); }
        inline glm::vec3 getAxisY() const { return glm::normalize(Rotation[1]); }
        inline glm::vec3 getAxisZ() const { return glm::normalize(Rotation[2]); }
        inline glm::vec3 getAxis(int idx) const {return glm::normalize(Rotation[idx]);}
        //8个顶点获取
        std::shared_ptr<VertexArray> getVertices() const;

        CollisionInfo getCollisionInfo(const ColliderPtr& other) const override;
    public:
        CollisionInfo CollisionWithAABB(const AABBPtr& other) const override;
        
        CollisionInfo CollisionWithOBB(const OBBPtr& other) const override;
    private:
        // 半长宽高
        glm::vec3 HalfExtents{1.0}; 
        // 旋转矩阵
        glm::mat3 Rotation{1.0};    
    };


    //函数计算
    class Collider {
    public:
        static bool intersects(const ColliderPtr& A, const ColliderPtr& B);
        //碰撞面计算
        static CollisionFace determineCollisionFace(const glm::vec3& normal, const glm::vec3& Upvec = {0, 1, 0});
    private:
        // AABB与AABB碰撞检测
        static bool intersectsAABBAABB(const ColliderPtr& a, const ColliderPtr& b);
        
        // AABB与OBB碰撞检测
        static bool intersectsAABBOBB(const ColliderPtr& a, const ColliderPtr& b);
        
        // OBB与OBB碰撞检测
        static bool intersectsOBBOBB(const ColliderPtr& a, const ColliderPtr& b);
    };

}