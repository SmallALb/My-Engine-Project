#pragma once
#include <memory>
#include <functional>
#include <unordered_set>
#include <glm/glm.hpp>
#include "FISH/Log.h"
//空间大小
struct BlankSpace {
    glm::vec3 Min, Max;
};


template<class Ty, class Hash, class Equal>
class OcTree {
    using DivedeFunc = std::function<bool(const BlankSpace&, const Ty&)>;

    //八叉树节点
    class OcNode {
    public:
        OcNode(glm::vec3 mi, glm::vec3 mx ) {
            mSpace.Max = mx;
            mSpace.Min = mi;
        }

        
        //划分
        void dive() {
            if (!ChildNodes.empty()) return;
            
            auto [Min, Max] = mSpace;
            auto Center = 0.5f * (Min + Max);

            for (int i=0; i<8; i++) {
                glm::vec3 newMin = Min, newMax = Max;
                ((i & 1) ? newMin.x : newMax.x) =  Center.x;    
                ((i & 2) ? newMin.y : newMax.y) = Center.y;
                ((i & 4) ? newMin.z : newMax.z) = Center.z;
                ChildNodes.emplace_back(newMin, newMax);
            }
        }

        //合并
        bool merge() {
            if (ChildNodes.empty()) return false;
            for (auto& obj : ChildNodes) if (!obj.ChilddNodes.empty() && !obj.merge()) {
                return false;
            }
            ChildNodes.clear(); return true;
        }

        //是否是叶子
        bool isLeaf() const {return ChildNodes.empty();};
    
        BlankSpace mSpace;
        std::vector<OcNode> ChildNodes;
        std::unordered_set<Ty, Hash, Equal> ObjsInNode;
    };
public:
    OcTree(glm::vec3 Min, glm::vec3 Max, const DivedeFunc& Func = nullptr): divefunc(Func) {
        Root = new OcNode(Min, Max);
    }

    ~OcTree() {delete Root;}

    //回调查询
    template<class CALL, class... INER>
    bool query(const Ty& obj_, bool once ,CALL&& func, INER&&... args) {
        if (divefunc == nullptr) return false;
        auto dfs = [&, this](auto&& self, OcNode& node)->bool {
            bool diveTag = divefunc(node.mSpace, obj_);
            bool callbackTag = 0;
            
            if (!diveTag) return false;

            if (!node.ChildNodes.empty()) { 
                for (auto& child : node.ChildNodes) {
                    callbackTag |= self(self, child);
                    if (once && callbackTag) return true;

                }
            }
            else {
                for (auto& obj : node.ObjsInNode) {
                    callbackTag |= func(obj, std::forward<INER>(args)...);
                    if (callbackTag && once) return true;
                }
            }

            return callbackTag;
        };

        return dfs(dfs, *Root);

    }

    bool remove(const Ty& obj) {
        if (divefunc == nullptr) {
            return false;
        }

        auto dfs = [&, this] (auto&& self, OcNode& node) {
            if (!divefunc(node.mSpace, obj)) return;
            if (!node.isLeaf()) {
                node.ObjsInNode.erase(obj);
                for (auto& child : node.ChildNodes) self(self, child);
                if (node.ObjsInNode.size() <= MaxCount) node.merge();
            }
            else node.ObjsInNode.erase(obj);
        };
        dfs(dfs, *Root);
    }

    void add(const Ty& obj) {
        if (divefunc == nullptr) {
            return;
        }

        auto dfs = [&, this] (auto&& self, OcNode& node) {
            if (!divefunc(node.mSpace, obj)) return;
            if (!node.isLeaf()) {
                node.ObjsInNode.insert(obj);
                for (auto& child : node.ChildNodes) 
                    self(self, child);
            }          
            else {
                node.ObjsInNode.insert(obj);
                if (node.ObjsInNode.size() > MaxCount) {
                    node.dive();
                    for (auto& child : node.ChildNodes) self(self, child);
                }
            }
        };
        dfs(dfs, *Root);
    }

    void setMaxCountInNode(int c) {MaxCount = c;} 

    void clear() {
        auto [Min, Max] = Root->mSpace;
        delete Root;
        Root = new OcNode(Min, Max);
    }

    void setDividefunc(const DivedeFunc& Func) {divefunc = Func;}
private:
    OcNode* Root{nullptr};
    DivedeFunc divefunc;    
    int MaxCount{1000};
    
};