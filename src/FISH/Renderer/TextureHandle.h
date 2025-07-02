#pragma once

#include "FISH/Core.h"

namespace FISH {
    class API_  TextureHandle {
    public:
        virtual unsigned long long getHandle() const = 0;
    };

}