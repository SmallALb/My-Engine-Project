#pragma once
#include "Object.h"
#include "FISH/Core.h"

namespace FISH {

    using BottonBottonFunc = std::function<void()>;
    //游戏内部按键
    class Botton : public Object2D {
    public:
        Botton(const std::string& name = "Botton");

        ~Botton();

        void update(float x, float y);
        
        inline void setHoverFunc(const BottonBottonFunc& func) {mHoverFunc = func;}

        inline void setClickFunc(const BottonBottonFunc& func) {mClickFunc = func;}

        void setClick(bool status);

        void setBoxSize(const glm::vec2& _size);

        OBJ_CLASS_TYPE(Botton)
    private:
        bool IsHover(float x, float y);
    private:
        glm::vec2               mBoxSize{0.0, 0.0};
        //点击触发函数
        BottonBottonFunc        mClickFunc;
        //放在盒体内的函数
        BottonBottonFunc        mHoverFunc;
        //运行函数
        BottonBottonFunc        mNoStatusFunc;
        //按下
        bool                    clickTag{0};
    };

}