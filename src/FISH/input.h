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
        inline static float GetMouseX() {return GetInstance().GetMouseXImpl();}
        //����y
        inline static float GetMouseY() {return GetInstance().GetMouseYImpl();}
        //����x,y
        inline static std::pair<float, float> GetMousePos() {return GetInstance().GetMousePosImpl(); }

        //��ȡ������һ������������ʵ��
        static Input& GetInstance();
    protected:
        virtual bool IsKeyPressedImpl(int keycode) = 0;
        virtual bool IsButtonPressedImpl(int button) = 0;
        virtual float GetMouseXImpl() = 0;
        virtual float GetMouseYImpl() = 0;
        virtual std::pair<float, float> GetMousePosImpl() = 0;
    private:
        
        
    };

}