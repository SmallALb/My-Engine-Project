#include "fspcs.h"
#include "FISH/Object/Object.h"
#include "FISH/physics/ObjectCast.h"
#include "FISH/Time.h"
#include "FISH/Log.h"
#include "GameObject.h"


namespace FISH {
    void GameObject::update(float dt) {

    }

    void GameObject::setPosition(const glm::vec3& pos) {
        if (mPosition == pos) return;
        auto dvec = pos - mPosition;
        mPosition = pos;
        mBounds.Min += dvec;
        mBounds.Max += dvec;
        needUpdate = 1;
    }

    void GameObject::onCollision(const std::shared_ptr<GameObject> &other) {
        //FS_INFO("Object collided: {0}, {1}", mName.c_str(), other->getName().c_str());
    }

    const AABB& GameObject::getBounds() const { return mBounds;}
}