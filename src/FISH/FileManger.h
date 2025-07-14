#pragma once

#include "FISH/Core.h"

namespace FISH {
    using Json = nlohmann::json;

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
}