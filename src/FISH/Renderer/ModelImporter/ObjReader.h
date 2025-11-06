#pragma once

#include "ModelImporter.h"

namespace FISH {
    class ObjReader : public ModelImporter {
    public:
        Object3DPtr readModel(string path) override;

    };

}