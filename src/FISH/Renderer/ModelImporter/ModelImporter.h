#pragma once

#include "FISH/Core.h"

namespace FISH {
    enum class ModelType {
        OBJ,
        FBX
    };

    struct MeshData {
        string name;
        std::vector<float> positions;
        std::vector<float> normals;
        std::vector<float> uvs;
        std::vector<float> colors;
        std::vector<unsigned int> indices;
        glm::vec3 rotation{0.0f};
        glm::vec3 scale{1.0f};
        glm::vec3 position{0.0f};

        glm::vec3 diffuseColor{1.0f};
        glm::vec3 specularColor{0.0f};
        glm::vec3 emissiveColor{0.0f};
        float shininess{32.0f};
        float opacity{1.0f};
        int  valid{-1};
        std::string diffuseTexturePath;
        //下一节点
        std::vector<std::shared_ptr<MeshData>> childs;
        
        MeshData() = default;

        ~MeshData();
    };

    struct ModelData {
        string path;
        std::shared_ptr<MeshData> head{nullptr};
        bool valid {0};
        ModelData() = default;

        ~ModelData();
    };

    //模型读取器
    class API_ ModelImporter {
        friend class ModelManager;
    public:
        ModelImporter() {}

        ~ModelImporter() {}

        virtual Object3DPtr readModel(const string& path) = 0;
        
        virtual ModelData readModelData(const string& path) = 0;
        
        static Object3DPtr ImportModel(const string& path, ModelType type);

        static ModelData ImportModelData(const string& path, ModelType type);

    private:
        static std::unordered_map<string, Object3DPtr> ModelMap; 

    };


}