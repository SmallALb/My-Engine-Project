#pragma once

#include "FISH/Core.h"

namespace FISH {
    //八叉树节点
    class API_ ONode{
    public:
        //设置碰撞测试为友元
        friend class CollisionTest;
        
        ONode(const AABB& bounds): mBounds(bounds) {}

        //拷贝禁用
        ONode(const ONode&) = delete;
        ONode& operator=(const ONode&) = delete;

        //允许移动
        ONode(ONode&&) = default;
        ONode& operator=(ONode&&) = default;

        ~ONode() {}
        
        //节点切割
        void subDivideNode();

        //合并节点
        bool mergeNodes();
        inline bool IsLeave() const {return LeaveTag;}
    private:
        std::list<std::shared_ptr<GameObject>>        mObjs{};
        std::vector<std::unique_ptr<ONode>>             mChilds{};
        bool                                            LeaveTag{1};
        bool                                            LazyTag{0};
        AABB                                            mBounds;
    };

    //碰撞测试(八叉树)
    class API_ CollisionTest {

    public:
        CollisionTest(const AABB& worldBounds):
            root(std::make_unique<ONode>(worldBounds))
        {}

        //更新
        void update();
        //插入
        void insert(const std::shared_ptr<GameObject>& obj);
        //移除
        void remove(const std::shared_ptr<GameObject>& obj);
        //检测
        void check();
        //清除
        void cleanUp(bool force);
    private:
        //是否能够被合并
        bool IsCanMerge(ONode* node) const;
        
    private:
        std::unique_ptr<ONode>  root;
        int                     Maxdepth{100};
        int                     MaxObjsPreNode{10};
    };
}