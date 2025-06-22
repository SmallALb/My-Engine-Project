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
        virtual float GetMouseXImpl() override;
        
        virtual float GetMouseYImpl() override;

        virtual std::pair<float, float> GetMousePosImpl() override;
        
    

    };

}