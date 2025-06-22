#pragma once

#include "FISH/Core.h"

namespace FISH {
    //图形API上下文创建
    class API_ GraphicsContext {
    public:
        virtual void init() = 0;
        virtual void SwapBuffers() = 0;
    }; 

}