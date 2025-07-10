#pragma once

#include "Core.h"
#include "Events/Events.h"

namespace FISH {
    //层
    class API_ Layer {
    public:
        Layer(const string& name = "Layer");

        virtual ~Layer();
        //初始化
        virtual void OnAttach() {}
        //销毁
        virtual void OnDetach() {}
        //更新
        virtual void OnUpdate(float dt) {}
        //渲染UI
        virtual void OnImGuiRender() {}
        //事件监听
        virtual void OnEvent(Event& event) {}
        //获取名字
        inline const string& GetName() const {return mDebugName;}
    protected:
        string mDebugName;
    }; 

}
