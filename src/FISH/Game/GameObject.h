#pragma once

#include "FISH/Core.h"

namespace FISH {
    enum class GameObjectType {
        None,
        Box
    };

    //游戏obj
    class API_ GameObject {
    public:
        GameObject() {
            needUpdate = 1;
            update();
            needUpdate = 0;
        }

        GameObject(const ColliderPtr& _bounds, const string& name = "GameObj") : mBounds(_bounds), mName(name) {}
        virtual ~GameObject() {}
        //物体更新
        virtual void update(float dt = Time::DeltaTime);
        
        //两个物体碰撞回调
        virtual void onCollision(const std::shared_ptr<GameObject>& other);

        virtual void setPosition(const glm::vec3& pos);

        string getName() const {return mName;}
        void setName(const string& name) {mName = name;}

        ColliderPtr getBounds() const;

        //设置物体位置

        //获取物体位置
        inline const glm::vec3& getPosition() const {return mPosition;}

        //查看是否需要更新
        inline bool IsNeedUpdate() const { return needUpdate;}

        inline void disableUpdate() {needUpdate = 0; }
    protected:
        ColliderPtr mBounds{nullptr};        
        string mName;
        glm::vec3 mPosition{0};
        bool needUpdate{0};
    };
    
    using GameObjPtr = std::shared_ptr<GameObject>;
}