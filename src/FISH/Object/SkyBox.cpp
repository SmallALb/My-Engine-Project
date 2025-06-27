#include "fspcs.h"
#include "FISH/Renderer/Shader.h"
#include "FISH/Renderer/Buffer.h"
#include "FISH/Renderer/VertexArray.h"
#include "FISH/Renderer/RenderElement.h"
#include "FISH/Renderer/Texture.h"
#include "FISH/Renderer/BaseShape.h"
#include "SkyBox.h"

namespace FISH{

    SkyBox::SkyBox(const string &Name){
        mName = Name;
        mBall.reset(Shape::CreateSphere(950.0f));
        mType = ObjType::SkyBox;
    }

    void SkyBox::setTexture(const std::shared_ptr<Texture>& mTexture) {
        mBall->setTexture(mTexture);
    }

    void SkyBox::setTexture(Texture *mTexture) {
        std::shared_ptr<Texture> ptr(mTexture);
        mBall->setTexture(ptr);
    }

    void SkyBox::renderSkyBox() {
        mBall->useTexture();
        mBall->render(TRIANGLES);
    }
    uint64_t SkyBox::useTexture() const {
        return mBall->useTexture();
    }
}
