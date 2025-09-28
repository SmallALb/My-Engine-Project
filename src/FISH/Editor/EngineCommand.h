#pragma once

#include "FISH/Core.h"

namespace FISH {
    class API_ Command {
    public:
        virtual ~Command() {}

        virtual bool execute() = 0;

        virtual bool cancel() = 0;


    };
    
    using CommandPtr = std::shared_ptr<Command>;

    class CommandPool {
    public:
        CommandPool(int siz = 100);

        ~CommandPool() {}

        void PushCommand(const CommandPtr& ptr);

        void setMaxSize(int siz);

        void cancel();

    private:
        std::vector<CommandPtr> CommandStack; 
        int front{0};
        int back{-1};
        int Count;
        int MaxSize {100};
    };
}