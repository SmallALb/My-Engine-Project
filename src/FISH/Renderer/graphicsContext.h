#pragma once

#include "FISH/Core.h"

namespace FISH {
    //ͼ��API�����Ĵ���
    class API_ GraphicsContext {
    public:
        virtual void init() = 0;
        virtual void SwapBuffers() = 0;
    }; 

}