#ifndef APP
#define APP FISH::Application::GetInstance()
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Keycodes.h"
#include "MouseButtonCodes.h"
#include "input.h"
#include "Timer.h"
#include "FISH/ImGui/ImGuiLayer.h"


namespace FISH{
    using CleanFUN = std::function<void()>;

    //Appliacation
    class API_ Application {
    public:
        Application();
        
        virtual ~Application();

        void run();

        void setClean(const CleanFUN& fun);

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        void LockCursor();

        void UnLockCursor();

        inline Window& GetWindow() {return *mWindow; }

        inline static Application& GetInstance() {return *Instance; }

        inline Timer& GetTimer() {return *mTimer;}

    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
        bool                                                IsLocked{0};
        bool                                                HadBeenLocked{0};
        bool                                                mLastPressed{0};
        static Application*                                 Instance;
        std::unique_ptr<Window>                             mWindow{nullptr};
        ImGuiLayer*                                         mImGuiLayer;
        bool                                                mRunning{1};
        LayerStack                                          mLayerStack;
        std::unique_ptr<Timer>                              mTimer{nullptr};
        CleanFUN                                            mCleanfun;
    };


    Application* CreateApplication();
}
#endif