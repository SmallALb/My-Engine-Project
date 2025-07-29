#pragma once
#include "GameObject.h"
#include "FISH/Core.h"


namespace FISH {
    class API_ Box : public GameObject {
    public:
        Box(const AABBPtr& bounds, const string& name = "GameBox") : GameObject(bounds, name) {}
        //设置渲染模型
        inline void setRenderMesh(const MeshPtr& mesh) {mMesh = mesh;}
        //从类型中选择创建
        void setRenderMeshFromType(ShapeType typ, const glm::vec3& siz);

        inline MeshPtr getMesh() const {return mMesh;}

        virtual void update(float) override;
        //
        void setPosition(const glm::vec3& pos) override;

    private:
        MeshPtr mMesh;
    };

    using GameBoxPtr = std::shared_ptr<Box>;
}