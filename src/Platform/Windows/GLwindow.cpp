#include "fspcs.h"

#include "FISH/Log.h"
#include "Platform/OpenGL/glad/glad.h"
#include <GLFW/glfw3.h>
#include "GLwindow.h"

#include "FISH/Events/KeyEvent.h"
#include "FISH/Events/MouseEvent.h"
#include "FISH/Events/Application.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace FISH {
    static bool s_GLFWInitalized = false;

    static void GLFWErrorCallback(int error, const char* description) {
        FS_CORE_ERROR("GLW Error ({0}): {1}", error, description);
    }

    Window* Window::Create(const WindowProps& props) {
        FS_CORE_INFO("Create OpenGL_window");
        return new GLwindow(props);
    }

    GLwindow::GLwindow(const WindowProps& props) {
        Init(props);
    }

    GLwindow::~GLwindow() {
        Shutdown();
        FS_CORE_INFO("GLwindow destruct!");
    }

    void GLwindow::Onupdate() {
        
        glfwPollEvents();
        mContext->SwapBuffers();

    }

    void GLwindow::ViewPortResize(int width, int height) {
        glViewport(0, 0, width, height);
    }

    GLwindow* GLwindow::CreateGL(const WindowProps& props) {
        return new GLwindow(props);
    }

    void GLwindow::SetVSync(bool enable) {
        if (enable) glfwSwapInterval(1);
        else glfwSwapInterval(0);

        mData.VSync = enable;
    }

    bool GLwindow::IsVSync() const {
        return mData.VSync;
    }

    inline void GLwindow::CleanBuffer(int cleanState) {
        glClear(cleanState);
    }

    void GLwindow::Init(const WindowProps &props)
    {
        mData.Title = props.Title;
        mData.Width = props.Width;
        mData.Height = props.Height;

        FS_CORE_INFO("Create Window(Title:{0} Size:({1}, {2}))", mData.Title, mData.Width, mData.Height);
        
        
        if (!s_GLFWInitalized) {
            int success = glfwInit();
            FS_CORE_ASSERT(success, "failed:无法初始化Opengl!");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitalized = 1;
            
        }
        //OpenGL的窗口创建
        mWindow = glfwCreateWindow((int)mData.Width, (int)mData.Height, mData.Title.c_str(), nullptr, nullptr);
        mContext = new OpenGLContext(mWindow);
        mContext->init();
        
        
       
        glfwSetWindowUserPointer(mWindow, &mData);
        //垂直同步
        SetVSync(true);        


        //更改大小回调
        glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* window, int width, int height) {
            auto& ptr =  *(WindowData*)glfwGetWindowUserPointer(window);
            WindowResizeEvent event(width, height);
            ptr.Width = width;
            ptr.Height = height;
            ptr.eventcallback(event);
        });
        //窗口关闭回调
        glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* window) {
            auto& ptr =  *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            ptr.eventcallback(event);
        });
        
        //键盘回调
        glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            auto& ptr =  *(WindowData*)glfwGetWindowUserPointer(window);
            switch(action) {
                case GLFW_PRESS:{
                    KeyPressedEvent event(key, 0);
                    ptr.eventcallback(event);
                    break;
                }
                case GLFW_RELEASE:{
                    KeyReleasedEvent event(key);
                    ptr.eventcallback(event);
                    break;
                }
                case GLFW_REPEAT:{
                    KeyPressedEvent event(key, 1);
                    ptr.eventcallback(event);
                    break;
                }
            }
        });
        //鼠标回调
        glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* window, int botton, int action, int mods) {
            auto& ptr =  *(WindowData*)glfwGetWindowUserPointer(window);
            switch(action) {
                case GLFW_PRESS:{
                    MouseButtonPressedEvent event(botton);
                    ptr.eventcallback(event);
                    break;
                }
                case GLFW_RELEASE:{
                    MouseButtonReleasedEvent event(botton);
                    ptr.eventcallback(event);
                    break;
                }
            }
        });

        //滚轮回调
        glfwSetScrollCallback(mWindow, [](GLFWwindow* window, double xOffset, double yOffset) {
            auto& ptr =  *(WindowData*)glfwGetWindowUserPointer(window);
            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            ptr.eventcallback(event);
        });

        //指针回调
        glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, double x, double y) {
            auto& ptr =  *(WindowData*)glfwGetWindowUserPointer(window);
            MouseMovedEvent event((float)x, float(y));
            ptr.eventcallback(event);
        });

        glfwSetCharCallback(mWindow, [](GLFWwindow* window, unsigned int character) {
            auto& ptr =  *(WindowData*)glfwGetWindowUserPointer(window);
            KeyTypedEvent event(character);
            ptr.eventcallback(event);
        });
    }
    void GLwindow::Shutdown() {
        //FS_INFO("�����Ѿ��ر�");
        glfwDestroyWindow(mWindow);
        
    }
    bool GLwindow::CursorLockInWindow() {
        CursorLockInfo =  GLFW_CURSOR_DISABLED;
        glfwSetCursorPos(mWindow,  mData.Width/ 2.0, mData.Height/ 2.0);
        glfwSetInputMode(mWindow, GLFW_CURSOR, CursorLockInfo);
        return CursorLockInfo == GLFW_CURSOR_DISABLED;
    }
    bool GLwindow::CursorUnLockInWindow() {
        CursorLockInfo =  GLFW_CURSOR_NORMAL;
        glfwSetInputMode(mWindow, GLFW_CURSOR, CursorLockInfo);
        return CursorLockInfo == GLFW_CURSOR_NORMAL;
    }
}