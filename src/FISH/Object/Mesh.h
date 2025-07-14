#pragma once

#include "FISH/Core.h"
//#include "FISH/Renderer/BaseShape.h"
#include "Object.h"

namespace FISH {
    class Shape;
    //模型类
    class Mesh : public Object3D {
    public:
        Mesh(const string& name = "Mesh") {
            mName = name;
            mType = ObjType::Mesh;
        }

        Mesh(const std::shared_ptr<FISH::Shape>& shape, const string& name = "Mesh"):mShape(shape) {
            mName = name;
            mType = ObjType::Mesh;
        }

        ~Mesh();

        //得到该模型的形状和材质
        std::shared_ptr<Shape>& getShape();
        
        void renderMesh(int typ);

        OBJ_CLASS_TYPE(Mesh)
    private:
        std::shared_ptr<Shape> mShape{nullptr};
    };

}

