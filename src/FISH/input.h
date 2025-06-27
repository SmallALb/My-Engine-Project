#pragma once
#include <utility>
#include "Core.h"

namespace FISH {
    //�����ѯ�ӿ�
    class API_ Input {
    public:
        //�����Ƿ���
        inline static bool IsKeyPressed(int keycode) { return GetInstance().IsKeyPressedImpl(keycode); }
        //����Ƿ񱻰���
        inline static bool IsMouseButtonPressed(int button) {return GetInstance().IsButtonPressedImpl(button);}
        //����x
        inline static double GetMouseX() {return GetInstance().GetMouseXImpl();}
        //����y
        inline static double GetMouseY() {return GetInstance().GetMouseYImpl();}
        //����x,y
        inline static std::pair<double, double> GetMousePos() {return GetInstance().GetMousePosImpl(); }

        //��ȡ������һ������������ʵ��
        static Input& GetInstance();
    protected:
        virtual bool IsKeyPressedImpl(int keycode) = 0;
        virtual bool IsButtonPressedImpl(int button) = 0;
        virtual double GetMouseXImpl() = 0;
        virtual double GetMouseYImpl() = 0;
        virtual std::pair<double, double> GetMousePosImpl() = 0;
    private:
        
        
    };

}