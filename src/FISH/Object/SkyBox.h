#pragma once

#include "Object.h"
#include "FISH/Core.h"

namespace FISH {
    class SkyBox : public Object3D {
    public:
        SkyBox(const string& Name = "SkyBox");

        ~SkyBox() {}

        void setTexture(const std::shared_ptr<Texture>& mTexture);

        void setTexture(Texture* mTexture);

        void renderSkyBox();

        OBJ_CLASS_TYPE(SkyBox)
    private:
        std::shared_ptr<Shape> mBall;
    };

}

