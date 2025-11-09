#pragma once

#include "ModelImporter.h"

namespace FISH {
    class ObjReader : public ModelImporter {
    public:
        Object3DPtr readModel(const string& path) override;

        ModelData readModelData(const string& path) override;

    };

}