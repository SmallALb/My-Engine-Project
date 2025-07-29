#pragma once
#include <utility>
#include "Core.h"

namespace FISH {
    //输入查询接口
    class API_ Input {
    public:
        //键盘是否按下
        inline static bool IsKeyPressed(int keycode) { return GetInstance().IsKeyPressedImpl(keycode); }
        //键盘是否被按下一次
        inline static bool IsKeyPressedOnce(int keycode) {return GetInstance().IsKeyPressedOnceImpl(keycode);}
        //鼠标是否被按下
        inline static bool IsMouseButtonPressed(int button) {return GetInstance().IsButtonPressedImpl(button);}
        //鼠标是否已经按下
        inline static bool IsMouseButtonPressedOnce(int button) {return GetInstance().IsButtonPressedImplOnce(button);}
        //鼠标的x
        inline static double GetMouseX() {return GetInstance().GetMouseXImpl();}
        //鼠标的y
        inline static double GetMouseY() {return GetInstance().GetMouseYImpl();}
        //鼠标的x,y
        inline static std::pair<double, double> GetMousePos() {return GetInstance().GetMousePosImpl(); }
        

        //获取单例，一般在派生类中实现
        static Input& GetInstance();
    protected:
        virtual bool IsKeyPressedImpl(int keycode) = 0;
        virtual bool IsKeyPressedOnceImpl(int keycode) = 0;
        virtual bool IsButtonPressedImpl(int button) = 0;
        virtual bool IsButtonPressedImplOnce(int button) = 0;
        virtual double GetMouseXImpl() = 0;
        virtual double GetMouseYImpl() = 0;
        virtual std::pair<double, double> GetMousePosImpl() = 0;
    private:
        
        
    };

}