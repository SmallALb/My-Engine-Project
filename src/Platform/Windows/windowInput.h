#pragma once

namespace FISH {
    //窗口输入查询
    class WindowsInput : public Input {
        std::unordered_map<int, bool> KeyMap;
        std::unordered_map<int, bool> MouseMap;
    protected:
        //查询按键
        virtual bool IsKeyPressedImpl(int keycode) override;
        //查询鼠标
        virtual bool IsButtonPressedImpl(int button) override;
        //鼠标是否被按下一次
        virtual bool IsButtonPressedImplOnce(int button) override;
        //查询是否被按下一次
        virtual bool IsKeyPressedOnceImpl(int keycode) override;
        //获取位置
        virtual double GetMouseXImpl() override;
        
        virtual double GetMouseYImpl() override;

        virtual std::pair<double, double> GetMousePosImpl() override;
        
    

    };

}