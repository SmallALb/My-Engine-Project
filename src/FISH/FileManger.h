#pragma once

#include "FISH/Core.h"

//自定义Json数据类型存储
namespace nlohmann {
    template<>
    struct adl_serializer<glm::vec3> {
        static void to_json(nlohmann::json& j, const glm::vec3& vec) { 
            j = nlohmann::json::array({vec.x, vec.y, vec.z});
        }

        static void from_json(const nlohmann::json& j, glm::vec3& vec) {
            vec.x = j[0];
            vec.y = j[1];
            vec.z = j[2];
        }
    };

}


namespace FISH {
    using Json = nlohmann::json;
    using Ini = std::unordered_map<string, std::unordered_map<string, string>>;

    //数据存储模板接口
    template <class T>
    class DataStorage { 
    public:
        virtual ~DataStorage() = default;

        //加载
        virtual bool load(const string& id, T& data) = 0;
        //保存
        virtual bool save(const string& id, const T& data) = 0;
        //查看是否存在
        virtual bool exists(const string& id) const = 0;
        //移除
        virtual bool remove(const string& id) = 0;
    };

    class API_ JsonFileStorage : public DataStorage<Json> {
    public:
        explicit JsonFileStorage(const std::string& baseDir = "Datas");

        //加载
        virtual bool load(const string& id, Json& data) override;
        //保存
        virtual bool save(const string& id, const Json& data) override;
        //查看是否存在
        virtual bool exists(const string& id) const override;
        //移除
        virtual bool remove(const string& id) override;
    private:
        string getPath(const string& id) const;

    private:
        string mBaseDir;
        
    }; 

    class API_ IniFileStorage : public DataStorage<Ini> {
    public:
        explicit IniFileStorage(const std::string& baseDir = "Configs");

                //加载
        virtual bool load(const string& id, Ini& data) override;
        //保存
        virtual bool save(const string& id, const Ini& data) override;
        //查看是否存在
        virtual bool exists(const string& id) const override;
        //移除
        virtual bool remove(const string& id) override;
    
    private:
        string getPath(const string& id) const;

        void parseLine(const string& line, string& currentSection, Ini& data) const;

        void wirteInSection(std::ofstream& file, const string& section, const std::unordered_map<string, string>& keyValues) const;
    
    private:
        string mBaseDir;
    };

}