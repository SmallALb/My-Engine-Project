#pragma once
#define FDT 
#include "Core.h"

namespace FISH {

    class API_ FrameDeltaTime {
        FrameDeltaTime() {}
        ~FrameDeltaTime() {}
    public:
        


        double getDeltaTime() const;

        static FrameDeltaTime& GetInstance();
    private:
        static FrameDeltaTime* DeltaTimeGetter;
        double PreFrameTime;
        double StartFrameTime;
    };
}