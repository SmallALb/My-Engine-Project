#pragma once

#include "FISH/Renderer/graphicsContext.h"

struct GLFWwindow;

namespace FISH {
    //OpenglAPI的上下文
    class OpenGLContext : public GraphicsContext {
    public:
        //设置上下文
        OpenGLContext(GLFWwindow* WindowHandle);

        ~OpenGLContext();
        //初始化
        virtual void init() override;
        //切换双缓冲
        virtual void SwapBuffers() override;
    private:
        GLFWwindow* mHandle{nullptr};
    };

}