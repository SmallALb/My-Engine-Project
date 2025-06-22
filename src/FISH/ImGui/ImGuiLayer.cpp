#include "fspcs.h"
#include "Platform/OpenGL/glad/glad.h"
#include <glfw/glfw3.h>
#include "FISH/Application.h" 
#include "Platform/OpenGL/imgui_impl_glfw.h"
#include "Platform/OpenGL/imgui_impl_opengl3.h"
#include "ImGuiLayer.h"
#include "FISH/Log.h"

namespace FISH {

    ImGuiLayer::ImGuiLayer(): Layer("ImGuiLayer") {
        //OnAttach();
    }

    ImGuiLayer::~ImGuiLayer() {
        OnDetach();
    }

    void ImGuiLayer::OnAttach() {
        FS_INFO("Initializing ImGui...");
        IMGUI_CHECKVERSION();
        ImGuiContext* ctx =  ImGui::CreateContext();
        
        ImGuiIO& io = ImGui::GetIO();
        //允许键盘
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        //允许停靠窗口
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        //允许视口端口
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImFont* font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/simsun.ttc", 
            18.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
        if (!font) FS_ERROR("字体无法加载");
        io.Fonts->Build();
        ImGui::StyleColorsDark();
        
        //设置样式
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 3.0f;
        }

        ImGui::StyleColorsDark();

        Application& app = Application::GetInstance();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

        //为Opengl加载
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");
        FS_INFO("Imgui Has been Initialized!");

        //main_dockspace_id = ImGui::GetID("MainDockspace"); // 获取有效的ID

    }
    
    void ImGuiLayer::OnDetach() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }



    void ImGuiLayer::OnImGuiRender() {
        //FS_INFO("Render Imgui");
        //static bool show = true;
        //ImGui::ShowDemoWindow(&show);

        // ImGui::DockSpaceOverViewport(
        //     main_dockspace_id, 
        //     ImGui::GetMainViewport()
        // );
        
    }

    void ImGuiLayer::Begin(bool KeyLocked) {
        ImGuiIO& io = ImGui::GetIO();
        if (KeyLocked) {
            io.ConfigFlags |= ImGuiConfigFlags_NoMouse;     // �������
            io.ConfigFlags |= ImGuiConfigFlags_NavNoCaptureKeyboard; // ���ü���
            io.WantCaptureKeyboard = false; // ǿ�Ʋ��������
        } else {
            io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
            io.ConfigFlags &= ~ImGuiConfigFlags_NavNoCaptureKeyboard;
        }
        //FS_INFO("Begin Render");
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //ImGuizmo::BeginFrame();

    }
    
    void ImGuiLayer::End() {
        //FS_INFO("End Render");
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::GetInstance();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
        //渲染ImGUI
        ImGui::Render();
        //���� ��������
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            //得到之前的窗口
            GLFWwindow* backup_current_contex = glfwGetCurrentContext();
            //更新窗口
            ImGui::UpdatePlatformWindows();
            //渲染独立窗口
            ImGui::RenderPlatformWindowsDefault();
            //回调之前的窗口
            glfwMakeContextCurrent(backup_current_contex);
        }
    }

    void UI::Text(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        ImGui::TextV(fmt, args);  // 调用 ImGui 的实际实现
        va_end(args);
    }
}