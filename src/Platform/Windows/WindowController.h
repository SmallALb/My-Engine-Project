#pragma once

#ifdef _WIN32
#include <Windows.h>
#include <Xinput.h>
#endif
#include "FISH/Controller.h"

namespace FISH {
    class XController : public Controller {
    public:
        XController() {}
        ~XController() {}

        virtual void init() override;
        virtual void update() override;
        virtual void setVibration(float leftMotor, float rightMotor) override;
    private:
        void setupButtonMappings();
    private:
        unsigned long mControllerId{0};
        #ifdef _WIN32
            _XINPUT_STATE mCurrentState;
        #endif
        bool WARNTag{0};

    };
}