#pragma once

#include "FISH/Renderer/graphicsContext.h"

struct GLFWwindow;

namespace FISH {
    //OpenglAPI��������
    class OpenGLContext : public GraphicsContext {
    public:
        //����������
        OpenGLContext(GLFWwindow* WindowHandle);

        ~OpenGLContext();
        //��ʼ��
        virtual void init() override;
        //�л�˫����
        virtual void SwapBuffers() override;
    private:
        GLFWwindow* mHandle{nullptr};
    };

}