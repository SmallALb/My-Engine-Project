#pragma once

#include "FISH/Core.h"

namespace FISH {
    class API_  TextureHandle {
    public:
        virtual unsigned long long getHandle() const = 0;

        virtual unsigned long long getTextureID() const = 0;

        inline string getPath() const {return mPath;}

        
    protected:
        string                  mPath;
    };

}