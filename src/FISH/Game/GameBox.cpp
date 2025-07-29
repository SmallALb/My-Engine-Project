#include "fspcs.h"
#include "FISH/Object/Mesh.h"
#include "FISH/Log.h"
#include "FISH/Renderer/Buffer.h"
#include "FISH/Renderer/TextureHandle.h"
#include "FISH/Renderer/VertexArray.h"
#include "FISH/Renderer/RenderElement.h"
#include "FISH/Renderer/BaseShape.h"
#include "FISH/physics/ObjectCast.h"
#include "FISH/Time.h"
#include "GameBox.h"

namespace FISH {
    void Box::setRenderMeshFromType(ShapeType typ, const glm::vec3 &siz) {
        ShapePtr mode =nullptr;
        switch (typ) {
            case ShapeType::Box: {
                mode.reset(Shape::CreateBox(1.0));
                break;
            }
            case ShapeType::Plan: {
                mode.reset(Shape::CreatePlan(1.0, 1.0));
                break;
            }
            case ShapeType::Sphere: {
                mode.reset(Shape::CreateSphere(1.0f));
                break;
            }
        }
        if (mode == nullptr) {
            FS_ERROR("Unknown Shape Type!");
            return;
        }

        mMesh.reset(new Mesh("Box Mesh"));
        mMesh->getShape() = mode;
        mMesh->setScale(siz);
    }

    void Box::update(float) {

    }

    void Box::setPosition(const glm::vec3 &pos) {
        GameObject::setPosition(pos);
        if (mBounds) {
            auto aabb = PtrCastTo<AABB>(mBounds);
            if (aabb) {
                glm::vec3 centerOffset = (aabb->getMin() + aabb->getMax()) * 0.5f; 
                mBounds->setPosition(pos - centerOffset);
            }
        }

        if (mMesh) mMesh->setPosition(pos);
    }
}
