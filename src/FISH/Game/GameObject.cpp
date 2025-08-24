#include "fspcs.h"
#include "FISH/Object/Object.h"
#include "FISH/Renderer/Buffer.h"
#include "FISH/Renderer/VertexArray.h"
#include "FISH/Renderer/RenderElement.h"
#include "FISH/Timer.h"
#include "FISH/Renderer/TextureHandle.h"
#include "FISH/Renderer/Texture.h"
#include "FISH/Renderer/Animation.h"
#include "FISH/Renderer/BaseShape.h"
#include "FISH/physics/ObjectCast.h"
#include "FISH/Time.h"
#include "FISH/Log.h"
#include "GameObject.h"


namespace FISH {
    void GameObject::update(float dt) {

    }

    void GameObject::setPosition(const glm::vec3& pos) {
        if (mPosition == pos) return;
        mPosition = pos;
        mBounds->setPosition(pos);
        needUpdate = 1;
    }

    void GameObject::onCollision(const std::shared_ptr<GameObject> &other) {
        //FS_INFO("Object collided: {0}, {1}", mName.c_str(), other->getName().c_str());
    }

    ColliderPtr GameObject::getBounds() const { return mBounds;}
}