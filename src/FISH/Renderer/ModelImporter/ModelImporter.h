#pragma once

#include "FISH/Core.h"

namespace FISH {
    enum class ModelType {
        OBJ,
        FBX
    };

    //模型读取器
    class API_ ModelImporter {
    public:
        ModelImporter() {}

        ~ModelImporter() {}

        virtual Object3DPtr readModel(string path) = 0;
        
        static std::shared_ptr<ModelImporter> CreateImporter(ModelType type);
    };


}