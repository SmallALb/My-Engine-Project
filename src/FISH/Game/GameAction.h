#pragma once

#include "FISH/Core.h"

namespace FISH {
    class BTNode;

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
                FS_ERROR("Bad key cast: {0}", key);
                throw std::out_of_range(key);
            }
        }
        
        bool hasValue(const string& key) {
            return data.contains(key);
        }

        bool erase(const string& key) {
            if (data.contains(key)) data.erase(key);
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
        BTContext(BlackBlock& _b, float _t = 0):blackboard(_b), dt(_t) {}
        BlackBlock& blackboard;
        float dt;

        std::unordered_map<BTNode*, BTStatus> nodeCache;

        bool interrupted = false;

        void clearCache() {
            nodeCache.clear();
            interrupted = false;
        }

        void setInterrupted() {
            interrupted = true;
        }
    };

    //行为树节点接口
    class API_ BTNode {
    public:
        virtual ~BTNode() {}

        virtual void init(BTContext& context) {}

        virtual BTStatus update(BTContext& context) = 0;

        virtual void stop(BTContext& context, BTStatus status) {}

        inline string getName() const {return mName;}

        inline void setName(const string& Name) {mName = Name;}

        virtual BTStatus updateWithCache(BTContext& context);

        virtual void interrupt(BTContext& context);
    protected:
        string mName{"BTNode"};
    };
    using BTNodePtr = std::shared_ptr<BTNode>;
    
    //装饰器节点
    class DecoratorNode : public BTNode {
    public:
        DecoratorNode(const BTNodePtr& child, const string& Name = "Decorator") {
            Child = child;
            setName(Name);
        }

        void interrupt(BTContext& context) override;
    protected:
        BTNodePtr Child;
    };
    using DecoratorNodePtr = std::shared_ptr<DecoratorNode>;

    //行动节点
    class ActionNode : public BTNode {
    public:
        using ActionFunc = std::function<BTStatus(BTContext&)>;
        
        ActionNode(const string& Name, const ActionFunc& func): actionFunc(func) {
            setName(Name);
        }

        BTStatus update(BTContext& context) override;
        
    private:
        ActionFunc actionFunc;
    };

    //条件节点
    class ConditionNode : public BTNode {
    public:
        using ConditionFunc = std::function<bool(BTContext&)>;    

        ConditionNode(const string& name, const ConditionFunc& func): conditionFunc(func) {
            setName(name);
        }

        BTStatus update(BTContext& context) override;
        
    private:
        ConditionFunc conditionFunc;
    };

    //序列节点
    class SequenceNode : public BTNode {
    public:
        SequenceNode(const string& name = "Sequence") {
            setName(name);
        }

        void addChild(const BTNodePtr& node);

        void  init(BTContext& context) override;

        BTStatus update(BTContext& context) override;


        void interrupt(BTContext& context) override;

        void stop(BTContext& context, BTStatus status) override;
    
    private:
        std::vector<BTNodePtr>  Childs{};
        unsigned int            CurrentChild{0};
    };

    //选择节点
    class SelectNode : public BTNode {
    public:
        SelectNode(const string& name = "Select") {
            setName(name);
        }

        void addChild(const BTNodePtr& child);
        
        void init(BTContext& context) override;

        BTStatus update(BTContext& context) override;

        void interrupt(BTContext& context) override;

        void stop(BTContext& context, BTStatus status) override;
    private:
        std::vector<BTNodePtr>      Childs{};
        unsigned int                CurrentChild{0};

    };

    //并行节点
    class ParallelNode : public BTNode {
    public:
        enum class Policy {
            RequireAll,
            RequireOnce
        };

        ParallelNode(Policy successP, Policy failureP, const string& name = "Parallel"): 
        success(successP), failure(failureP) {
            setName(name);
        }

        void addChild(const BTNodePtr& child);

        void init(BTContext& context) override;

        virtual BTStatus update(BTContext& context) override;

        void interrupt(BTContext& context) override;

        virtual void stop(BTContext& context, BTStatus status) override;
    private:
        std::vector<BTNodePtr> Childs;
        Policy success;
        Policy failure;
    };
    //翻转
    class InverterNode : public DecoratorNode {
    public:
        InverterNode(const BTNodePtr& child): DecoratorNode(child, "Inverter") 
        {}

        BTStatus update(BTContext& context) override;
    };
    //反复
    class RepeaterNode : public DecoratorNode {
    public:
        RepeaterNode(const BTNodePtr& child): DecoratorNode(child, "Repeater") 
        {}

        void init(BTContext& context) override;

        BTStatus update(BTContext& context) override;

    private:
        int repeatCount;
        int currentCount;
    };

        //行为树
    class API_ BehaviorTree {
    public:
        BehaviorTree(const BTNodePtr& root): mRoot(root) {}

        void execute(BTContext& context);

        void interrupt(BTContext& context);

        inline BTStatus getStatus() const {return CurrentStatus;}
    private:
        BTNodePtr        mRoot{};            
        BTStatus        CurrentStatus{BTStatus::Invalid};
    };
}