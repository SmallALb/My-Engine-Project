#pragma once

#include "Object.h"

namespace FISH {

    class Scene : public  Object3D {
    public:
        Scene();
        
        ~Scene();
        
        OBJ_CLASS_TYPE(Scene);
    };

}