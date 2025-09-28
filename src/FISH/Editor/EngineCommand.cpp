#include "fspcs.h"
#include "FISH/Log.h"
#include "EngineCommand.h"

namespace FISH {
    CommandPool::CommandPool(int siz)  : MaxSize(siz){
        CommandStack.resize(MaxSize);
    }

    void CommandPool::PushCommand(const CommandPtr &ptr) {
        
        if (!ptr->execute()) return;
        front = Count == MaxSize ? (front + 1) % MaxSize : front + 1;
        Count = Count == MaxSize ? Count : Count + 1;
        back = (back + 1) % MaxSize;
        CommandStack[back] = ptr;
    }

    void CommandPool::setMaxSize(int siz) {
        if (siz <= 0) FS_ERROR("MaxSize must be positive!");

        if (siz < CommandStack.size()) {
            int rem = Count - siz;
            front = (front + rem) % MaxSize;
            Count = siz;
        }

        std::vector<CommandPtr> newBuffer(siz);
        
        for (int i = 0; i < Count; ++i) {
            newBuffer[i] = CommandStack[(front + i) % MaxSize];
        }
        
        CommandStack = std::move(newBuffer);
        MaxSize = siz;
        front = 0;
    }

    void CommandPool::cancel() {
        if (Count == 0) return;
        CommandStack[back]->cancel();
        back = (back - 1 + MaxSize) % MaxSize;
        Count--;
    }
}
