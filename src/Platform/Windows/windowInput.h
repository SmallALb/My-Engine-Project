#pragma once

namespace FISH {
    //���������ѯ
    class WindowsInput : public Input {
    protected:
        //��ѯ����
        virtual bool IsKeyPressedImpl(int keycode) override;
        //��ѯ���
        virtual bool IsButtonPressedImpl(int button) override;
        //��ȡλ��
        virtual double GetMouseXImpl() override;
        
        virtual double GetMouseYImpl() override;

        virtual std::pair<double, double> GetMousePosImpl() override;
        
    

    };

}