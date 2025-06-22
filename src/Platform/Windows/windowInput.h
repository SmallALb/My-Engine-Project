#pragma once

namespace FISH {
    //窗口输入查询
    class WindowsInput : public Input {
    protected:
        //查询按键
        virtual bool IsKeyPressedImpl(int keycode) override;
        //查询鼠标
        virtual bool IsButtonPressedImpl(int button) override;
        //获取位置
        virtual float GetMouseXImpl() override;
        
        virtual float GetMouseYImpl() override;

        virtual std::pair<float, float> GetMousePosImpl() override;
        
    

    };

}