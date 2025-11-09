#include "fspcs.h"
#include "FISH/Log.h"
#include "FISH/Object/Object.h"
#include "ModelImporter.h"
#include "FbxReader.h"
#include "ObjReader.h"
namespace FISH {
    std::unordered_map<string, Object3DPtr> ModelImporter::ModelMap{}; 

    Object3DPtr ModelImporter::ImportModel(const string &path, ModelType type) {
        if (ModelMap.contains(path)) return ModelMap[path];
        
        ModelImporter* creater = nullptr;
        switch(type) {
            case ModelType::FBX: creater = new FbxReader(); break;
            case ModelType::OBJ: creater = new ObjReader(); break;
        }
        if (creater == nullptr) return FS_ERROR("Unable to found Model Type!");
        ModelMap[path] = creater->readModel(path);
        delete creater;
        return ModelMap[path];

    }
    ModelData ModelImporter::ImportModelData(const string &path, ModelType type) {
        ModelImporter* creater = nullptr;
        switch(type) {
            case ModelType::FBX: creater = new FbxReader(); break;
            case ModelType::OBJ: creater = new ObjReader(); break;
        }
        if (creater == nullptr) return FS_ERROR("Unable to found Model Type!"), ModelData();
        auto data = creater->readModelData(path);

        delete creater;
        return data;
    }
    MeshData::~MeshData() {
        FS_INFO("Release MeshData: {}", name);
    }

    ModelData::~ModelData() {
        FS_INFO("Release ModelData: {}", path);
    }
}