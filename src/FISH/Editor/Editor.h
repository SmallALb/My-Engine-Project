#pragma once
#include "FISH/Core.h"
#include "FISH/Layer.h"
#include "EngineCommand.h"

namespace FISH {

    class API_ Editor : public Layer {
    public:
        Editor() : Layer("Editor") {};

    protected:
        static CommandPool mEditorCommands;
        
    };

}