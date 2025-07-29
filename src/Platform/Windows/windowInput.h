#pragma once

namespace FISH {
    //���������ѯ
    class WindowsInput : public Input {
        std::unordered_map<int, bool> KeyMap;
        std::unordered_map<int, bool> MouseMap;
    protected:
        //��ѯ����
        virtual bool IsKeyPressedImpl(int keycode) override;
        //��ѯ���
        virtual bool IsButtonPressedImpl(int button) override;
        //����Ƿ񱻰���һ��
        virtual bool IsButtonPressedImplOnce(int button) override;
        //��ѯ�Ƿ񱻰���һ��
        virtual bool IsKeyPressedOnceImpl(int keycode) override;
        //��ȡλ��
        virtual double GetMouseXImpl() override;
        
        virtual double GetMouseYImpl() override;

        virtual std::pair<double, double> GetMousePosImpl() override;
        
    

    };

}