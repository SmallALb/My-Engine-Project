#pragma once
#include "FISH/Window.h"
#include "FISH/Renderer/graphicsContext.h"


struct GLFWwindow;

namespace FISH {
    //Opengl 的窗口
    class GLwindow : public Window {
    public:
        //GL窗口构造函数
        GLwindow(const WindowProps& props);
        virtual ~GLwindow() ;
        
        //更新窗体
        void Onupdate() override;

        virtual void ViewPortResize(int width, int height) override;
        
        //创建GL窗口
        static GLwindow* CreateGL(const WindowProps& props = WindowProps());
        //获取窗体大小
        inline unsigned int GetWidth() const override {return mData.Width;}
        inline unsigned int GetHeight() const override {return mData.Height;}
        //设置回调函数
        inline void SetEventCallback(const EventCallback& callback) override {
            mData.eventcallback = callback;
        }
        //设置垂直同步
        void SetVSync(bool enable) override;
        bool IsVSync() const override;
        //获取本地窗口
        inline virtual void* GetNativeWindow() const { return mWindow; }
        //清理缓冲
        inline void CleanBuffer(int cleanState) override;
        //鼠标居中
        void SetCursorOnMid() const override;

        inline void SetCleanColor(float r, float g, float b, float a) override;

        virtual void SetWindowTitle(const string& title) override;

    private:
        //初始化
        virtual void Init(const WindowProps& props);
        //关闭
        virtual void Shutdown();
        //指针上锁
        virtual bool CursorLockInWindow() override;
        //指针取消上锁
        virtual bool CursorUnLockInWindow() override;
    private:
        //原GLwindow
        GLFWwindow* mWindow;
        GraphicsContext* mContext;
        int CursorLockInfo = GLFW_CURSOR_NORMAL;
        //窗口信息
        struct WindowData {
            string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallback eventcallback;
        } mData;

    };
}