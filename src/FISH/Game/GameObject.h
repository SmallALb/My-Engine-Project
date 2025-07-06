#pragma once

#include "FISH/Core.h"

namespace FISH {
    //游戏obj
    class API_ GameObject {
    public:
        GameObject() {
            needUpdate = 1;
            update();
            needUpdate = 0;
        }

        GameObject(const AABB& _bounds, const string& name = "GameObj") : mBounds(_bounds), mName(name) {}
        virtual ~GameObject() {}
        //物体更新
        virtual void update(float dt = Time::DeltaTime);
        
        //两个物体碰撞回调
        virtual void onCollision(const std::shared_ptr<GameObject>& other);

        string getName() const {return mName;}

        const AABB& getBounds() const;

        //设置物体位置
        void setPosition(const glm::vec3& pos);

        //获取物体位置
        inline const glm::vec3& getPosition() const {return mPosition;}

        //查看是否需要更新
        inline bool IsNeedUpdate() const { return needUpdate;}
    protected:
        AABB mBounds{};        
        string mName;
        glm::vec3 mPosition{0};
        bool needUpdate{0};
    };
    
}