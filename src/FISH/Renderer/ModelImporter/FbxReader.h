#pragma once

#include "ModelImporter.h"

// class FbxNode;
// class FbxMesh;
// class FbxManager;
// class FbxScene;
// class FbxImporter;

namespace FISH {


    class FbxReader : public ModelImporter {
    public:
        FbxReader();

        ~FbxReader();
        
        Object3DPtr readModel(const string& path) override;

        ModelData readModelData(const string& path) override;

        
    private:
        class Impl;
        Impl* impl{nullptr};
    }; 

}