#pragma once

#include "FISH/Core.h"

namespace FISH {
    //黑板类(共享游戏数据)
    class BlackBlock {
    public:
        //设置值
        template<class T>
        void setValue(const string& key, const T& value) {
            data[key] = value;
        }

        template<class T>
        T getValue(const string& key) const {
            try {
                return std::any_cast<T>(data.at(key));
            }catch(const std::bad_any_cast&) {
                throw FS_ERROR("Bad key cast:", {key});
            }
        }
        
        bool hasValue(const string& key) {
            return data.contains(key);
        }
    private:
        std::unordered_map<string, std::any>    data;
    };


    //节点状态
    enum class BTStatus {
        Invalid,
        Running,
        Success,
        Failure,
    };

    //行为树上下文
    struct BTContext {
        BlackBlock& blackboard;
        float dt;
    };

    //行为树节点接口
    class API_ BTNode {
    public:
        virtual ~BTNode() {}

        virtual void init(BTContext& context) {}

        virtual BTStatus update(BTContext& context) = 0;

        virtual void stop(BTContext& context, BTStatus status) {}

        virtual string getName() const {return "BTNode";}
    };
}