#include "fspcs.h"
#include "Log.h"
#include "FileManger.h"
#include <fstream>

namespace FISH {
//Json
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

    string JsonFileStorage::getPath(const string &id) const
    {
        return (std::filesystem::path(mBaseDir) / (id + ".json")).string();
    }



//ini

    IniFileStorage::IniFileStorage(const std::string &baseDir) {
        mBaseDir = baseDir;
        if (!std::filesystem::exists(mBaseDir)) 
            std::filesystem::create_directories(mBaseDir);
    }

    bool IniFileStorage::load(const string &id, Ini &data) {
        auto filePath = getPath(id);
        if (!exists(id)) {
            FS_ERROR("INI file:{0} not exists!", id);
            return false;
        }

        std::ifstream file(filePath);
        if (!file.is_open()) {
            FS_ERROR("Can not open INI file: {0}", id);
            return false;
        }

        data.clear();
        std::string line;
        std::string currentSection;

        while(std::getline(file, line)) parseLine(line, currentSection, data);

            return true;
        }

        bool IniFileStorage::save(const string &id, const Ini &data) {
        auto filePath = getPath(id);
        auto dir = std::filesystem::path(filePath).parent_path();
        
        if (!std::filesystem::exists(dir)) {
            std::filesystem::create_directories(dir);
        }

        std::ofstream file(filePath);
        if (!file.is_open()) {
            FS_ERROR("Fialed to Create INI file: {0}", filePath);
            return false;
        }

        // 先写入全局节区（空节区名）
        if (data.contains("")) {
            wirteInSection(file, "", data.at(""));
        }

        // 写入其他节区
        for (const auto& [section, keyValues] : data) {
            if (!section.empty()) {
                wirteInSection(file, section, keyValues);
            }
        }
        return true;
    }

    bool IniFileStorage::exists(const string &id) const {
        return std::filesystem::exists(getPath(id));
    }

    bool IniFileStorage::remove(const string &id) {
        auto filePath = getPath(id);
        if (!exists(id)) {
            return false;
        }
        return std::filesystem::remove(filePath);
    }

    string IniFileStorage::getPath(const string &id) const {
        return (std::filesystem::path(mBaseDir) / (id + ".ini")).string();
    }
    void IniFileStorage::parseLine(const string &line, string &currentSection, Ini &data) const {
        string trimmed = line;
        //删除空白字符
        trimmed.erase(trimmed.begin(), std::find_if(trimmed.begin(), trimmed.end(), [](int ch) {
            return !std::isspace(ch);
        }));
        //跳过注释，空行
        if (trimmed.empty() || trimmed[0] == ';' || trimmed[0] == '#') {
            return;
        }

        // 检查是否是节区头
        if (trimmed[0] == '[' && trimmed.back() == ']') {
            currentSection = trimmed.substr(1, trimmed.size() - 2);
            return;
        }

        // 解析键值对
        size_t equalsPos = trimmed.find('=');
        if (equalsPos != std::string::npos) {
            std::string key = trimmed.substr(0, equalsPos);
            std::string value = trimmed.substr(equalsPos + 1);
            
            // 修剪键和值
            key.erase(key.begin(), std::find_if(key.begin(), key.end(), [](int ch) {
                return !std::isspace(ch);
            }));
            key.erase(std::find_if(key.rbegin(), key.rend(), [](int ch) {
                return !std::isspace(ch);
            }).base(), key.end());
            
            value.erase(value.begin(), std::find_if(value.begin(), value.end(), [](int ch) {
                return !std::isspace(ch);
            }));
            value.erase(std::find_if(value.rbegin(), value.rend(), [](int ch) {
                return !std::isspace(ch);
            }).base(), value.end());
            
            data[currentSection][key] = value;
        }

    }

    void IniFileStorage::wirteInSection(std::ofstream &file, const string &section, const std::unordered_map<string, string> &keyValues) const {
        if (!section.empty()) file <<"[" << section << "]\n";
        
    
        for (const auto& [key, value] : keyValues) 
            file << key << "=" << value << "\n";

        file << "\n";
    }
}