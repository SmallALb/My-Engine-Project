#include "fspcs.h"
#include "FISH/Log.h"
#include "GameAction.h"

namespace FISH {
    void BehaviorTree::execute(BTContext &context) {
        if (!mRoot) return;
            context.clearCache();
            mRoot->init(context);

        CurrentStatus = mRoot->updateWithCache(context);

        if (CurrentStatus != BTStatus::Running) {
            mRoot->stop(context, CurrentStatus);
            context.clearCache();
        }
        
    }

    void BehaviorTree::interrupt(BTContext& context) {
        context.setInterrupted();
        if (mRoot) {
            mRoot->interrupt(context);
        }
        CurrentStatus = BTStatus::Failure;
        context.clearCache();
    }

    //Action Node
    BTStatus ActionNode::update(BTContext & context) {
        if (actionFunc) return actionFunc(context);
        return BTStatus::Failure;
    }

    BTStatus BTNode::updateWithCache(BTContext &context) {
        if (context.nodeCache.find(this) != context.nodeCache.end()) {
                return context.nodeCache[this];
        }
        
        // 检查中断
        if (context.interrupted) {
            return BTStatus::Failure;
        }
        
        BTStatus status = update(context);
        context.nodeCache[this] = status;
        return status;
    }

    void BTNode::interrupt(BTContext &context) {
        stop(context, BTStatus::Failure);
    }

    //Condition Node
    BTStatus ConditionNode::update(BTContext &context) {
        if (conditionFunc) 
            return conditionFunc(context) ? BTStatus::Success : BTStatus::Failure;
        return BTStatus::Failure;
    }


    //Sequence Node
    void SequenceNode::addChild(const BTNodePtr &node) {
        Childs.push_back(node);
    }


    void SequenceNode::init(BTContext &context) {
        CurrentChild = 0;
    }

    BTStatus SequenceNode::update(BTContext &context) {
        if (Childs.empty()) return BTStatus::Success;
        CurrentChild = 0;
        while(CurrentChild < Childs.size()) {
            auto& child = Childs[CurrentChild];

            if (!child) {
                CurrentChild++;
                continue;
            }

            BTStatus status = child->updateWithCache(context);

            if (status != BTStatus::Success) return status;

            CurrentChild++;
        }
        return BTStatus::Success;
    }

    void SequenceNode::interrupt(BTContext &context) {
        if (CurrentChild < Childs.size()) {
            Childs[CurrentChild]->interrupt(context);
        }
        BTNode::interrupt(context);
    }

    void SequenceNode::stop(BTContext &context, BTStatus status)
    {
        if (CurrentChild < Childs.size()) Childs[CurrentChild]->stop(context, status);
        CurrentChild = 0;
    }

    //Select Node
    void SelectNode::addChild(const BTNodePtr& child) {
        Childs.push_back(child);
    }
    
    
    void SelectNode::init(BTContext &context) {
        CurrentChild = 0;
    }

    BTStatus SelectNode::update(BTContext &context) {
        if (Childs.empty()) return BTStatus::Failure;
        CurrentChild = 0;
        while(CurrentChild < Childs.size()) {
            auto& child = Childs[CurrentChild];

            if (!child) {
                CurrentChild++;
                continue;
            }

            BTStatus status = child->updateWithCache(context);

            if (status != BTStatus::Failure) return status;

            CurrentChild++;
        }
        return BTStatus::Failure;
    }

    void SelectNode::interrupt(BTContext &context) {
        if (CurrentChild < Childs.size()) {
                Childs[CurrentChild]->interrupt(context);
        }
        BTNode::interrupt(context);
    }

    void SelectNode::stop(BTContext &context, BTStatus status) {
        if (CurrentChild < Childs.size()) Childs[CurrentChild]->stop(context, status);
        CurrentChild = 0;
    }


    //Parallel Node
    void ParallelNode::init(BTContext &context) {
        for (auto& child : Childs) if (child)
            child->init(context);    
        
    }

    void ParallelNode::addChild(const BTNodePtr& child) {
        Childs.push_back(child);
    }


    BTStatus ParallelNode::update(BTContext & context) {
        if (Childs.empty()) return BTStatus::Success;

        unsigned int successCount = 0;
        unsigned int failureCount = 0;
        unsigned int runningCount = 0;
        for (auto& child : Childs) if (child){
            BTStatus status = child->updateWithCache(context);
            if (status == BTStatus::Success) successCount++;
            else if (status == BTStatus::Failure) failureCount++;
            else if (status == BTStatus::Running) runningCount++;
        }

        if (runningCount > 0) {
            return BTStatus::Running;
        }

        if (
            success == Policy::RequireAll && successCount == Childs.size() ||
            success ==Policy::RequireOnce && successCount > 0
        ) return BTStatus::Success;

        if (
            failure == Policy::RequireAll && failureCount == Childs.size() ||
            failure ==Policy::RequireOnce && failureCount > 0
        ) return BTStatus::Failure;


        return BTStatus::Running;
    }

    void ParallelNode::interrupt(BTContext &context) {
        for (auto& child : Childs) if (child)
             child->interrupt(context);
            
        BTNode::interrupt(context);
    }

    void ParallelNode::stop(BTContext & context, BTStatus status) {
        for (auto& child : Childs) if (child){
            child->stop(context, status);
        }
    }


    BTStatus InverterNode::update(BTContext & context) {
        if (!Child) return BTStatus::Failure;
        if (context.interrupted) return BTStatus::Failure;

        BTStatus status = Child->updateWithCache(context);

        if (status == BTStatus::Success) return BTStatus::Failure;
        else if (status == BTStatus::Failure) return BTStatus::Success;

        return status;
    }

    void RepeaterNode::init(BTContext &context) {
        currentCount = 0;
    }

    BTStatus RepeaterNode::update(BTContext &context) {
        if (!Child) return BTStatus::Failure;
        if (context.interrupted) return BTStatus::Failure;

        while(repeatCount < 0 || currentCount < repeatCount) {
            BTStatus status = Child->updateWithCache(context);
            if (status == BTStatus::Running) return BTStatus::Running;
            else if (status == BTStatus::Failure) return BTStatus::Failure;

            currentCount++;
        } 
        return BTStatus::Success;
    }

    void DecoratorNode::interrupt(BTContext &context) {
         if (Child) {
                Child->interrupt(context);
            }
            BTNode::interrupt(context);
    }
}
