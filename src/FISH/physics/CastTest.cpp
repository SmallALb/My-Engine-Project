#include "fspcs.h"
#include "FISH/Time.h"
#include "ObjectCast.h"
#include "FISH/Game/GameObject.h"
#include "CastTest.h"


namespace FISH {

    void ONode::subDivideNode() {
        if (!LeaveTag) return;

        auto center = 0.5f * (mBounds.Min + mBounds.Max);
        
        for (int i=0; i<8; i++) {
            auto newMin = mBounds.Min;
            auto newMax = center;

            if (i & 1) newMin.x = center.x;
            else newMax.x = center.x;

            if (i & 2) newMin.y = center.y;
            else newMax.y = center.y;

            if (i & 4) newMin.z = center.z;
            else newMax.z = center.z;

            
            mChilds.emplace_back(std::make_unique<ONode>(AABB(newMin, newMax)));
        }

        LeaveTag = 0;
        LazyTag = 1;
    }

    bool ONode::mergeNodes() {
        if (LeaveTag) return false;
        bool canMerge = !LazyTag;
        size_t Objs = 0;

        for (auto& child : mChilds) if (!child->IsLeave() || !child->mObjs.empty()) {
            canMerge = 0;
            break;
        }

        if (canMerge) {
            mChilds.clear();
            LeaveTag = 1;
            LazyTag = 0;
            return true;
        }

        return false;
    }

    void CollisionTest::update() {
        std::vector<std::shared_ptr<GameObject>> needUpdateObjs;

        auto func = [&](ONode* node, auto&& self)->void {
            if (node->IsLeave()) {
                for (auto& obj : node->mObjs) if (obj->IsNeedUpdate()) 
                    needUpdateObjs.push_back(obj);
            }
            else {
                for (auto& child : node->mChilds)
                    self(child.get(), self);
            }
        };

        func(root.get(), func);

        for(auto& obj : needUpdateObjs) {
            remove(obj);
            insert(obj);
            obj->update();
        }
        cleanUp(0);
    }

    void CollisionTest::insert(const std::shared_ptr<GameObject> &obj) {
        auto func = [&](
            ONode* node,
            const std::shared_ptr<GameObject> &obj, 
            const AABB &bounds,
            int depth,
            auto&& self
        )->void {
            //没有相交，直接返回
            if (!(node->mBounds & bounds)) return;
            //叶子直接添加
            if (node->LeaveTag) {
                node->mObjs.push_back(obj);
                //对象数量超过设定数量，且深度未超，则切割子节点
                if (node->mObjs.size() > MaxObjsPreNode && depth < Maxdepth) {
                    node->subDivideNode();
                    //重分配对象
                    for (auto& childObj : node->mObjs) for (auto& child : node->mChilds) { 
                        self(child.get(), childObj, childObj->getBounds(), depth+1, self);
                    }
                }
            }
            //非叶子直接递归，缩小盒体
            else for (auto& child : node->mChilds) self(child.get(), obj, child->mBounds, depth+1, self);
        };
        func(root.get(), obj, obj->getBounds(), 0, func);
    }
    
    void CollisionTest::remove(const std::shared_ptr<GameObject> &obj) {
        auto func = [&](ONode* node, std::shared_ptr<GameObject> obj, auto&& self)->bool {
            bool removed = 0;
            //没有与盒体相交不需要去除
            if (!(node->mBounds & obj->getBounds())) return false;
            //是叶子直接删除
            if (node->IsLeave()) {
                auto it = std::find(node->mObjs.begin(), node->mObjs.end(), obj);
                if (it != node->mObjs.end())
                    node->mObjs.erase(it), removed = 1;
            }
            //不是叶子继续递归
            else {
                for (auto& child : node->mChilds) 
                    removed = (self(child.get(), obj, self) || removed);    
                //被删除则将此节点融合
                if (removed && IsCanMerge(node)) node->LazyTag = 1;
            }
            return removed;
        };  
        func(root.get(), obj, func);
    }

    void CollisionTest::check() {
        auto func = [&] (ONode* node, auto&& self)->void {
            //是叶子，取出所有的盒内物体进行检测
            if (node->IsLeave()) {
                for (auto i = node->mObjs.begin(); i != node->mObjs.end(); i++) 
                    for (auto j = std::next(i); j != node->mObjs.end(); j++) {
                        if ((*i)->getBounds() & (*j)->getBounds())
                            (*i)->onCollision(*j), (*j)->onCollision(*i);
                }
            }
            //否则继续递归
            else for (auto& child : node->mChilds)
                    self(child.get(), self); 
        };
        func(root.get(), func);
    }

    void CollisionTest::cleanUp(bool force) {
        auto func = [&](ONode* node, auto&& self)->void {
            if (!node->IsLeave()) {
                for (auto& child : node->mChilds) self(child.get(), self);
                
                if (force || node->LazyTag) {
                    if (IsCanMerge(node)) node->mergeNodes();
                    else node->LazyTag = 0;
                }
            }
        };
        func(root.get(), func);
    }

    bool CollisionTest::IsCanMerge(ONode *node) const {
        if (node->IsLeave()) return false;

        for (auto& child : node->mChilds) if (!child->IsLeave() || !child->mObjs.empty()) return false;

        return true;
    }


}
