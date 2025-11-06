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
        
        Object3DPtr readModel(string path) override;
        
    private:
        class Impl;
        Impl* impl{nullptr};
    }; 

}