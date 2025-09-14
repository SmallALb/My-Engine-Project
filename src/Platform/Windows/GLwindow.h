#pragma once
#include "FISH/Window.h"
#include "FISH/Renderer/graphicsContext.h"


struct GLFWwindow;

namespace FISH {
    //Opengl �Ĵ���
    class GLwindow : public Window {
    public:
        //GL���ڹ��캯��
        GLwindow(const WindowProps& props);
        virtual ~GLwindow() ;
        
        //���´���
        void Onupdate() override;

        virtual void ViewPortResize(int width, int height) override;
        
        //����GL����
        static GLwindow* CreateGL(const WindowProps& props = WindowProps());
        //��ȡ�����С
        inline unsigned int GetWidth() const override {return mData.Width;}
        inline unsigned int GetHeight() const override {return mData.Height;}
        //���ûص�����
        inline void SetEventCallback(const EventCallback& callback) override {
            mData.eventcallback = callback;
        }
        //���ô�ֱͬ��
        void SetVSync(bool enable) override;
        bool IsVSync() const override;
        //��ȡ���ش���
        inline virtual void* GetNativeWindow() const { return mWindow; }
        //������
        inline void CleanBuffer(int cleanState) override;
        //������
        void SetCursorOnMid() const override;

        inline void SetCleanColor(float r, float g, float b, float a) override;

        virtual void SetWindowTitle(const string& title) override;

    private:
        //��ʼ��
        virtual void Init(const WindowProps& props);
        //�ر�
        virtual void Shutdown();
        //ָ������
        virtual bool CursorLockInWindow() override;
        //ָ��ȡ������
        virtual bool CursorUnLockInWindow() override;
    private:
        //ԭGLwindow
        GLFWwindow* mWindow;
        GraphicsContext* mContext;
        int CursorLockInfo = GLFW_CURSOR_NORMAL;
        //������Ϣ
        struct WindowData {
            string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallback eventcallback;
        } mData;

    };
}