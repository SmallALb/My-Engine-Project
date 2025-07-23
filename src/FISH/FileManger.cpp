#include "fspcs.h"
#include "Log.h"
#include "FileManger.h"
#include <fstream>

namespace FISH {
    JsonFileStorage::JsonFileStorage(const std::string &baseDir) {
        mBaseDir = baseDir;
        if (!std::filesystem::exists(mBaseDir)) 
            std::filesystem::create_directories(mBaseDir);
    }

    bool JsonFileStorage::load(const string &id, Json &data) {
        auto filePath = getPath(id);

        if (!exists(id)) {
            FS_ERROR("JSON file:{0} not exists!", id);
            return false;
        }

        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
         try {
            file.open(filePath);
            file >> data;
        }
        catch (std::ifstream::failure& e){

            //std::cout << "ERROR: Vertex Shader File Error: " << e.what() << std::endl;
            FS_ERROR("Error loading Json: {0}",  e.what());
            return false;
        }
        return true;
    }

    bool JsonFileStorage::save(const string &id, const Json &data) {
        auto filePath = getPath(id);
        auto dir = std::filesystem::path(filePath).parent_path();
        if (!std::filesystem::exists(dir)) std::filesystem::create_directories(dir);
        std::ofstream file;
        file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
        try {
            file.open(filePath);
            file << data.dump(4);
        }catch(std::ofstream::failure& e) {
            FS_ERROR("Error saving Json: {0}",  e.what());
            return false;
        }

        return true;
    }

    bool JsonFileStorage::exists(const string &id) const {
        return std::filesystem::exists(getPath(id));
    }

    bool JsonFileStorage::remove(const string &id) {
        auto filePath = getPath(id);
        if (!exists(id)) {
            return false;
        }
        
        return std::filesystem::remove(filePath);
    }

    string JsonFileStorage::getPath(const string &id) const {
        return (std::filesystem::path(mBaseDir) / (id + ".json")).string();
    }

}