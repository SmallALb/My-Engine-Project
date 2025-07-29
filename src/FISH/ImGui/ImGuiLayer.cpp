#include "fspcs.h"
#include "Platform/OpenGL/glad/glad.h"
#include <glfw/glfw3.h>
#include <imgui.h>
#include <imgui_internal.h>
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
        ImGui::TextV(fmt, args);  
        va_end(args);
    }

    bool UI::InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data) {
        return ImGui::InputText(label, buf, buf_size, flags, callback, user_data);
    }

    bool UI::CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags) {
        return ImGui::CollapsingHeader(label, flags);
    }

    bool UI::Checkbox(const char* label, bool* v) {
        return ImGui::Checkbox(label, v);
    }

    bool UI::SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags) {
       return ImGui::SliderFloat(label, v, v_min, v_max, format, flags);
    }

    void UI::Separator() { ImGui::Separator(); }
    bool UI::Button(const char* label, const ImVec2& size) { return ImGui::Button(label, size); }
    void UI::SameLine(float offset_from_start_x, float spacing) { ImGui::SameLine(offset_from_start_x, spacing); }
    void UI::BeginGroup() { ImGui::BeginGroup(); }
    void UI::EndGroup() { ImGui::EndGroup(); }
    bool UI::BeginMenuBar() { return ImGui::BeginMenuBar(); }
    void UI::EndMenuBar() { ImGui::EndMenuBar(); }
    bool UI::BeginMenu(const char* label, bool enabled) { return ImGui::BeginMenu(label, enabled); }
    void UI::EndMenu() { ImGui::EndMenu(); }
    bool UI::MenuItem(const char* label, const char* shortcut, bool selected, bool enabled) { 
        return ImGui::MenuItem(label, shortcut, selected, enabled); 
    }
    bool UI::Begin(const char* name, bool* p_open, ImGuiWindowFlags flags) { 
        return ImGui::Begin(name, p_open, flags); 
    }
    void UI::End() { ImGui::End(); }
    bool UI::BeginChild(const char* str_id, const ImVec2& size, bool border, ImGuiWindowFlags flags) { 
        return ImGui::BeginChild(str_id, size, border, flags); 
    }
    void UI::EndChild() { ImGui::EndChild(); }
    void UI::SetNextWindowPos(const ImVec2& pos, ImGuiCond cond, const ImVec2& pivot) { 
        ImGui::SetNextWindowPos(pos, cond, pivot); 
    }
    void UI::SetNextWindowSize(const ImVec2& size, ImGuiCond cond) { 
        ImGui::SetNextWindowSize(size, cond); 
    }
    void UI::SetNextWindowFocus() { ImGui::SetNextWindowFocus(); }
    void UI::PushItemWidth(float item_width) { ImGui::PushItemWidth(item_width); }
    void UI::PopItemWidth() { ImGui::PopItemWidth(); }
    void UI::PushStyleVar(ImGuiStyleVar idx, float val) { ImGui::PushStyleVar(idx, val); }
    void UI::PushStyleVar(ImGuiStyleVar idx, const ImVec2& val) { ImGui::PushStyleVar(idx, val); }
    void UI::PopStyleVar(int count) { ImGui::PopStyleVar(count); }
    void UI::PushStyleColor(ImGuiCol idx, const ImVec4& col) { ImGui::PushStyleColor(idx, col); }
    void UI::PopStyleColor(int count) { ImGui::PopStyleColor(count); }
    void UI::Columns(int count, const char* id, bool border) { ImGui::Columns(count, id, border); }
    void UI::NextColumn() { ImGui::NextColumn(); }
    int UI::GetColumnIndex() { return ImGui::GetColumnIndex(); }
    float UI::GetColumnWidth(int column_index) { return ImGui::GetColumnWidth(column_index); }
    void UI::SetColumnWidth(int column_index, float width) { ImGui::SetColumnWidth(column_index, width); }
    void UI::SetColumnOffset(int column_index, float offset_x) { ImGui::SetColumnOffset(column_index, offset_x); }
    int UI::GetColumnsCount() { return ImGui::GetColumnsCount(); }
    void UI::PushID(const char* str_id) { ImGui::PushID(str_id); }
    void UI::PushID(const void* ptr_id) { ImGui::PushID(ptr_id); }
    void UI::PushID(int int_id) { ImGui::PushID(int_id); }
    void UI::PopID() { ImGui::PopID(); }
    ImGuiID UI::GetID(const char* str_id) { return ImGui::GetID(str_id); }
    void UI::TextUnformatted(const char* text, const char* text_end) { ImGui::TextUnformatted(text, text_end); }
    
    void UI::LabelText(const char* label, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        ImGui::LabelTextV(label, fmt, args);
        va_end(args);
    }
    
    bool UI::Selectable(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size) { 
        return ImGui::Selectable(label, selected, flags, size); 
    }
    bool UI::ListBox(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items) { 
        return ImGui::ListBox(label, current_item, items, items_count, height_in_items); 
    }
    bool UI::ListBox(const char* label, int* current_item, bool (*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items) { 
        return ImGui::ListBox(label, current_item, items_getter, data, items_count, height_in_items); 
    }
    bool UI::ColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags) { 
        return ImGui::ColorEdit3(label, col, flags); 
    }
    bool UI::ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags) { 
        return ImGui::ColorEdit4(label, col, flags); 
    }
    bool UI::TreeNode(const char* label) { return ImGui::TreeNode(label); }
    bool UI::TreeNode(const char* str_id, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        bool result = ImGui::TreeNodeV(str_id, fmt, args);
        va_end(args);
        return result;
    }
    bool UI::TreeNode(const void* ptr_id, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        bool result = ImGui::TreeNodeV(ptr_id, fmt, args);
        va_end(args);
        return result;
    }
    bool UI::TreeNodeV(const char* str_id, const char* fmt, va_list args) { 
        return ImGui::TreeNodeV(str_id, fmt, args); 
    }
    bool UI::TreeNodeV(const void* ptr_id, const char* fmt, va_list args) { 
        return ImGui::TreeNodeV(ptr_id, fmt, args); 
    }
    bool UI::TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags) { 
        return ImGui::TreeNodeEx(label, flags); 
    }
    bool UI::TreeNodeEx(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        bool result = ImGui::TreeNodeExV(str_id, flags, fmt, args);
        va_end(args);
        return result;
    }
    bool UI::TreeNodeEx(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        bool result = ImGui::TreeNodeExV(ptr_id, flags, fmt, args);
        va_end(args);
        return result;
    }
    void UI::TreePush(const char* str_id) { ImGui::TreePush(str_id); }
    void UI::TreePush(const void* ptr_id) { ImGui::TreePush(ptr_id); }
    void UI::TreePop() { ImGui::TreePop(); }
    float UI::GetTreeNodeToLabelSpacing() { return ImGui::GetTreeNodeToLabelSpacing(); }
    void UI::SetNextItemOpen(bool is_open, ImGuiCond cond) { ImGui::SetNextItemOpen(is_open, cond); }
    void UI::SetTooltip(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        ImGui::SetTooltipV(fmt, args);
        va_end(args);
    }
    bool UI::BeginTooltip() { return ImGui::BeginTooltip(); }
    void UI::EndTooltip() { ImGui::EndTooltip(); }
    bool UI::BeginPopup(const char* str_id, ImGuiWindowFlags flags) { 
        return ImGui::BeginPopup(str_id, flags); 
    }
    bool UI::BeginPopupModal(const char* name, bool* p_open, ImGuiWindowFlags flags) { 
        return ImGui::BeginPopupModal(name, p_open, flags); 
    }
    void UI::EndPopup() { ImGui::EndPopup(); }
    void UI::OpenPopup(const char* str_id, ImGuiPopupFlags popup_flags) { 
        ImGui::OpenPopup(str_id, popup_flags); 
    }
    void UI::CloseCurrentPopup() { ImGui::CloseCurrentPopup(); }
    bool UI::IsPopupOpen(const char* str_id, ImGuiPopupFlags flags) { 
        return ImGui::IsPopupOpen(str_id, flags); 
    }
    bool UI::BeginTabBar(const char* str_id, ImGuiTabBarFlags flags) { 
        return ImGui::BeginTabBar(str_id, flags); 
    }
    void UI::EndTabBar() { ImGui::EndTabBar(); }
    bool UI::BeginTabItem(const char* label, bool* p_open, ImGuiTabItemFlags flags) { 
        return ImGui::BeginTabItem(label, p_open, flags); 
    }
    void UI::EndTabItem() { ImGui::EndTabItem(); }
    void UI::SetTabItemClosed(const char* tab_or_docked_window_label) { 
        ImGui::SetTabItemClosed(tab_or_docked_window_label); 
    }
    void UI::DockSpace(ImGuiID id, const ImVec2& size, ImGuiDockNodeFlags flags, const ImGuiWindowClass* window_class) { 
        ImGui::DockSpace(id, size, flags, window_class); 
    }
    
    ImGuiID UI::DockSpaceOverViewport(ImGuiID dockspace_id, const ImGuiViewport* viewport,ImGuiDockNodeFlags flags,const ImGuiWindowClass* window_class) {
        return ImGui::DockSpaceOverViewport(dockspace_id,viewport ? viewport : ImGui::GetMainViewport(),flags,window_class);
    }

    void UI::SetNextWindowDockID(ImGuiID dock_id, ImGuiCond cond) { 
        ImGui::SetNextWindowDockID(dock_id, cond); 
    }
    ImGuiID UI::GetWindowDockID() { return ImGui::GetWindowDockID(); }
    
    bool UI::IsWindowDocked() { return ImGui::IsWindowDocked(); }

    bool UI::DragFloat(const char* label, float* v, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags) {
        return ImGui::DragFloat(label, v, v_speed, v_min, v_max, format, flags);
    }

    bool UI::DragFloat2(const char* label, float v[2], float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags) {
        return ImGui::DragFloat2(label, v, v_speed, v_min, v_max, format, flags);
    }

    bool UI::DragFloat3(const char* label, float v[3], float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags) {
        return ImGui::DragFloat3(label, v, v_speed, v_min, v_max, format, flags);
    }

    bool UI::DragFloat4(const char* label, float v[4], float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags) {
        return ImGui::DragFloat4(label, v, v_speed, v_min, v_max, format, flags);
    }

    bool UI::DragFloatRange2(const char* label, float* v_current_min, float* v_current_max, float v_speed, float v_min, float v_max, const char* format, const char* format_max, ImGuiSliderFlags flags) {
        return ImGui::DragFloatRange2(label, v_current_min, v_current_max, v_speed, v_min, v_max, format, format_max, flags);
    }

    bool UI::DragInt(const char* label, int* v, float v_speed, int v_min, int v_max, const char* format, ImGuiSliderFlags flags) {
        return ImGui::DragInt(label, v, v_speed, v_min, v_max, format, flags);
    }

    bool UI::DragInt2(const char* label, int v[2], float v_speed, int v_min, int v_max, const char* format, ImGuiSliderFlags flags) {
        return ImGui::DragInt2(label, v, v_speed, v_min, v_max, format, flags);
    }

    bool UI::DragInt3(const char* label, int v[3], float v_speed, int v_min, int v_max, const char* format, ImGuiSliderFlags flags) {
        return ImGui::DragInt3(label, v, v_speed, v_min, v_max, format, flags);
    }

    bool UI::DragInt4(const char* label, int v[4], float v_speed, int v_min, int v_max, const char* format, ImGuiSliderFlags flags) {
        return ImGui::DragInt4(label, v, v_speed, v_min, v_max, format, flags);
    }

    bool UI::DragIntRange2(const char* label, int* v_current_min, int* v_current_max, float v_speed, int v_min, int v_max, const char* format, const char* format_max, ImGuiSliderFlags flags) {
        return ImGui::DragIntRange2(label, v_current_min, v_current_max, v_speed, v_min, v_max, format, format_max, flags);
    }

    bool UI::DragScalar(const char* label, ImGuiDataType data_type, void* p_data, float v_speed, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags) {
        return ImGui::DragScalar(label, data_type, p_data, v_speed, p_min, p_max, format, flags);
    }

    bool UI::DragScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, float v_speed, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags) {
        return ImGui::DragScalarN(label, data_type, p_data, components, v_speed, p_min, p_max, format, flags);
    }
}
