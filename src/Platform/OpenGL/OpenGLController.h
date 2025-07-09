#pragma once
#include "FISH/Controller.h"

namespace FISH {
    class GLController : public Controller {
    public:
        GLController() {}

        virtual void init() override;
    
        virtual void update() override;

    private:
        int             mControllerID{0};
        std::unordered_map<std::string, unsigned int> mButtonMappings{};
        std::unordered_map<std::string, unsigned int> mAxisMappings{};
    };

}