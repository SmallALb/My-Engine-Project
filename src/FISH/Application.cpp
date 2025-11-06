
#include "fspcs.h"
#include "Window.h"
#include "LayerStack.h"
#include "Keycodes.h"
#include "MouseButtonCodes.h"
#include "input.h"
#include "Timer.h"
#include <imgui.h>
#include "ImGui/ImGuiLayer.h"
#include "Events/Application.h"
#include "Log.h"
#include "FISH/Renderer/Buffer.h"
#include "Renderer/RenderElement.h"
#include "Renderer/Texture.h"
#include "Application.h"
#include "Time.h"


namespace FISH{

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

    Application* Application::Instance = nullptr;

    Application:: Application() {
        
        FS_CORE_ASSERT(!Instance, "Application alread exists!");
        Instance = this;
        mWindow.reset(Window::Create());
        mWindow->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
        FS_INFO("Start Application!");
        mImGuiLayer = new ImGuiLayer();
        PushLayer(mImGuiLayer);
        mTimer.reset(new Timer());
        mCleanfun = [this]() {
            mWindow->CleanBuffer(0);
        }; 
    }

    Application::~Application() {
        FS_INFO("Appliaction destruct!");
    }

    void Application::run() {
        Time::SetFPS(1000.0f);
        while(mRunning) {
            Time::updateDt();

            if (!IsLocked) mWindow->CursorUnLockInWindow(), HadBeenLocked = 0;
            else if (!HadBeenLocked && IsLocked) mWindow->CursorLockInWindow(), HadBeenLocked = 1;
            
            mCleanfun();

            for (auto layer : mLayerStack)
                layer->OnUpdate(Time::DeltaTime);
            mImGuiLayer->Begin(IsLocked);
            for (auto layer : mLayerStack)
                layer->OnImGuiRender();
            mImGuiLayer->End();

            mWindow->Onupdate();
        }
    }

    void Application::setClean(const CleanFUN &fun) {
        mCleanfun = fun;
    }

    void Application::OnEvent(Event &e) {
        EventDispatcher dispatcher(e);
        //检测是否为关闭事件，并执行app的关闭成员函数
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

        for (auto it = mLayerStack.end(); it != mLayerStack.begin();) {
            (*--it)->OnEvent(e);
            if (e.GetHandle()) break;
        }
    }


    bool Application::OnWindowClose(WindowCloseEvent &e)
    {
        mRunning = 0;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent &e) {
        mWindow->ViewPortResize(e.GetWidth(), e.GetHeight());
        return false;
    }

    void Application::PushLayer(Layer* layer) {
        mLayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer *layer) {
        mLayerStack.PushOverLay(layer);
    }

    void Application::LockCursor() {
        IsLocked = 1;
    }

    void Application::UnLockCursor() {
        IsLocked = 0;
    }

    void Application::SetCursorOnScreenMidle() {
        mWindow->SetCursorOnMid();
    }
}