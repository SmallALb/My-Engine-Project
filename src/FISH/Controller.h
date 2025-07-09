#pragma once

#include "Core.h"

namespace FISH {
    //手柄信息
    struct ControllerKeyInfo {
        ControllerKeyInfo() {}

        ControllerKeyInfo(
            unsigned int code_, 
            std::variant<bool, glm::vec2, float> data_): code(code_), data(data_)
        {}
        
        unsigned int code;
        //要么为bool 要么为向量
        std::variant<bool, glm::vec2, float> data;
        //取得布尔
        bool getKeyBool() const {
            if (const auto val = std::get_if<bool>(&data)) {
                return *val;
            }
            return 0;
        }
        //取得向量
        glm::vec2 getKeyVec() const {
            if (const auto val = std::get_if<glm::vec2>(&data)) {
                return *val;
            }
            return glm::vec2(0.0);

        }

        float getKeyFloat() const {
            if (const auto val = std::get_if<float>(&data)) {
                return *val;
            }
            return 0.0;
        }
    };

    //手柄接口
    class API_ Controller {
    public:
        //初始化
        virtual void init() = 0;
        //更新
        virtual void update() = 0;
        //手柄震动
        virtual void setVibration(float leftMotor, float rightMotor) {};
        //获取按键信息
        const ControllerKeyInfo getKeyInfo(const string& key) {
            if (!ConnectTag) return ControllerKeyInfo();
            return KeyInfoMap[key];
        };
        
        bool IsConnected() const { return ConnectTag; }
        //获取名字
        string getName() const {return mName;}
        //创建手柄实例对象
        static Controller* CreateController();

    protected:
        std::unordered_map<string, ControllerKeyInfo> KeyInfoMap;
        bool ConnectTag{0};

        string mName;
    };


}