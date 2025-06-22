#pragma once

#include "Core.h"
#include "Events/Events.h"

namespace FISH {
    //ͼ��
    class API_ Layer {
    public:
        Layer(const string& name = "Layer");

        virtual ~Layer();
        //��ʼ��
        virtual void OnAttach() {}
        //����
        virtual void OnDetach() {}
        //����
        virtual void OnUpdate() {}
        //��ȾUI
        virtual void OnImGuiRender() {}
        //�¼�����
        virtual void OnEvent(Event& event) {}
        //��ȡ����
        inline const string& GetName() const {return mDebugName;}
    protected:
        string mDebugName;
    }; 

}
