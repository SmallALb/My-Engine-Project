#include "fspcs.h"
#include "FISH/Time.h"
#include "FISH/Renderer/Buffer.h"
#include "FISH/Renderer/VertexArray.h"
#include "FISH/Renderer/RenderElement.h"
#include "FISH/Renderer/Texture.h"
#include "FISH/Renderer/BaseShape.h"
#include "ObjectCast.h"
#include "FISH/Game/GameObject.h"
#include "CastTest.h"

#include "FISH/Renderer/API.h"
#include "FISH/Renderer/Shader.h"
#include "FISH/Renderer/RenderElement.h"
#include "FISH/Renderer/Buffer.h"
#include "FISH/Renderer/VertexArray.h"
#include "FISH/Renderer/Texture.h"
#include "FISH/Renderer/BaseShape.h"
#include "../Object/Object.h"
#include "../Object/SpotLight.h"
#include "../Object/PointLight.h"
#include "../Object/DirectionLight.h"
#include "../Object/Camera.h"
#include "../Object/Mesh.h"
#include "../Object/SkyBox.h"
#include "../physics/ObjectCast.h"
#include "FISH/Renderer/Renderstatus.h"
#include "FISH/Renderer/Renderer.h"


namespace FISH {

    void ONode::subDivideNode() {
        if (!LeaveTag) return;

        auto [Min, Max] = mBounds->getBoundingPos();
        auto center = 0.5f * (Min + Max);
        
        mChilds.clear(); 

        for (int i=0; i<8; i++) {
            glm::vec3 newMin = Min;  // 从父节点Min开始初始化
            glm::vec3 newMax = Max;  // 从父节点Max开始初始化

            if ((i & 1)) { // 右半
                newMin.x = center.x;
            } else {     // 左半
                newMax.x = center.x;
            }

            if ((i & 2)) { // 上半
                newMin.y = center.y;
            } else {     // 下半
                newMax.y = center.y;
            }

            if ((i & 4)) { // 前半
                newMin.z = center.z;
            } else {     // 后半
                newMax.z = center.z;
            }

            auto aabb = std::make_shared<AABB>(newMin, newMax);
            mChilds.emplace_back(std::make_unique<ONode>(aabb));
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
            if (node == nullptr) return;

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
            obj->disableUpdate();
        }
        cleanUp(0);
    }

    void CollisionTest::insert(const std::shared_ptr<GameObject> &obj) {
        auto func = [&](
            ONode* node,
            const std::shared_ptr<GameObject> &obj, 
            const ColliderPtr& bounds,
            int depth,
            auto&& self
        )->void {
            //没有相交，直接返回
            if (!Collider::intersects(node->mBounds, bounds)) return;
            //叶子直接添加
            if (node->LeaveTag) {
                if (std::find(node->mObjs.begin(), node->mObjs.end(), obj) != node->mObjs.end()) return;
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
            if (node == nullptr || !Collider::intersects(node->mBounds, obj->getBounds())) return false;
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

    bool CollisionTest::query(const std::shared_ptr<GameObject> &obj) {
        auto func = [&](ONode* node, auto&& self)->bool {
            if (!Collider::intersects(node->mBounds, obj->getBounds())) return false;
            bool tag = false;
            if (node->IsLeave()) {
                for (const auto& nobj : node->mObjs) if (Collider::intersects(nobj->getBounds(), obj->getBounds())) 
                    obj->onCollision(nobj), tag = true;
            }
            else for (auto& child : node->mChilds)
                tag = self(child.get(), self);
            return tag;
        };

        return func(root.get(), func);
    }

    void CollisionTest::check() {
        auto func = [&] (ONode* node, auto&& self)->void {
            if (node == nullptr) return;
            //是叶子，取出所有的盒内物体进行检测
            if (node->IsLeave()) {
                if (renderBoxTag) Renderer::renderColliderBox(node->mBounds, {1.0, 0.0, 0.0});
                if (renderBoxTag) for (auto& obj : node->mObjs)  Renderer::renderColliderBox(obj->getBounds(), {0.0, 0.0, 1.0});

                for (auto i = node->mObjs.begin(); i != node->mObjs.end(); i++) 
                    for (auto j = std::next(i); j != node->mObjs.end(); j++) {
                        if (Collider::intersects((*i)->getBounds(), (*j)->getBounds()))
                            (*i)->onCollision(*j), (*j)->onCollision(*i);
                }
            }
            //否则继续递归
            else {
                if (renderBoxTag) Renderer::renderColliderBox(node->mBounds, {0.0, 1.0, 0.0});
                for (auto& child : node->mChilds)
                    self(child.get(), self); 
            }
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
