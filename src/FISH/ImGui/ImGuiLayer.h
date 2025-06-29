#pragma once

#include "../Core.h"
#include "../Layer.h"
#include "../Events/MouseEvent.h"
#include "../Events/KeyEvent.h"
#include "../Events/Application.h"



namespace FISH {
    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();

        ~ImGuiLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnImGuiRender() override;

        //为Imgui新开一个窗口
        void Begin(bool KeyLocked = 0);
        //结束当前的ImGui窗口
        void End();
    private:
        float mTime{0.0f};
        unsigned int main_dockspace_id{0};
    };

    //对ImguiUI在DLL层的封装
    class API_ UI {
    public:
        static void Text(const char* fmt, ...);

        static bool InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
        
        static bool Checkbox(const char* label, bool* v);
    
        static bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags);

        static void Separator();

        static bool Button(const char* label, const ImVec2& size = ImVec2(0,0));
        
        static void SameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f);
        
        static void BeginGroup();
        
        static void EndGroup();
        
        static bool BeginMenuBar();
        
        static void EndMenuBar();
        
        static bool BeginMenu(const char* label, bool enabled = true);
        
        static void EndMenu();
        
        static bool MenuItem(const char* label, const char* shortcut = NULL, 
                            bool selected = false, bool enabled = true);
        
        static bool Begin(const char* name, bool* p_open = NULL, 
                         ImGuiWindowFlags flags = 0);
        
        static void End();
        
        static bool BeginChild(const char* str_id, const ImVec2& size = ImVec2(0,0), 
                              bool border = false, ImGuiWindowFlags flags = 0);
        
        static void EndChild();
        
        static void SetNextWindowPos(const ImVec2& pos, ImGuiCond cond = 0, 
                                    const ImVec2& pivot = ImVec2(0,0));
        
        static void SetNextWindowSize(const ImVec2& size, ImGuiCond cond = 0);
        
        static void SetNextWindowFocus();
        
        static void PushItemWidth(float item_width);
        
        static void PopItemWidth();
        
        static void PushStyleVar(ImGuiStyleVar idx, float val);
        
        static void PushStyleVar(ImGuiStyleVar idx, const ImVec2& val);
        
        static void PopStyleVar(int count = 1);
        
        static void PushStyleColor(ImGuiCol idx, const ImVec4& col);
        
        static void PopStyleColor(int count = 1);
        
        static void Columns(int count = 1, const char* id = NULL, bool border = true);
        
        static void NextColumn();
        
        static int GetColumnIndex();
        
        static float GetColumnWidth(int column_index = -1);
        
        static void SetColumnWidth(int column_index, float width);
        
        static void SetColumnOffset(int column_index, float offset_x);
        
        static int GetColumnsCount();
        
        static void PushID(const char* str_id);
        
        static void PushID(const void* ptr_id);
        
        static void PushID(int int_id);
        
        static void PopID();
        
        static ImGuiID GetID(const char* str_id);
        
        static void TextUnformatted(const char* text, const char* text_end = NULL);
        
        static void LabelText(const char* label, const char* fmt, ...);
        
        static bool Selectable(const char* label, bool selected = false, 
                              ImGuiSelectableFlags flags = 0, 
                              const ImVec2& size = ImVec2(0,0));
        
        static bool ListBox(const char* label, int* current_item, 
                           const char* const items[], int items_count, 
                           int height_in_items = -1);
        
        static bool ListBox(const char* label, int* current_item, 
                           bool (*items_getter)(void* data, int idx, const char** out_text), 
                           void* data, int items_count, int height_in_items = -1);
        
        static bool ColorEdit3(const char* label, float col[3], 
                              ImGuiColorEditFlags flags = 0);
        
        static bool ColorEdit4(const char* label, float col[4], 
                              ImGuiColorEditFlags flags = 0);
        
        static bool TreeNode(const char* label);
        
        static bool TreeNode(const char* str_id, const char* fmt, ...);
        
        static bool TreeNode(const void* ptr_id, const char* fmt, ...);
        
        static bool TreeNodeV(const char* str_id, const char* fmt, va_list args);
        
        static bool TreeNodeV(const void* ptr_id, const char* fmt, va_list args);
        
        static bool TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags = 0);
        
        static bool TreeNodeEx(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, ...);
        
        static bool TreeNodeEx(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...);
        
        static void TreePush(const char* str_id = NULL);
        
        static void TreePush(const void* ptr_id = NULL);
        
        static void TreePop();
        
        static float GetTreeNodeToLabelSpacing();
        
        static void SetNextItemOpen(bool is_open, ImGuiCond cond = 0);
        
        static bool CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags);
        
        static void SetTooltip(const char* fmt, ...);
        
        static bool BeginTooltip();
        
        static void EndTooltip();
        
        static bool BeginPopup(const char* str_id, ImGuiWindowFlags flags = 0);
        
        static bool BeginPopupModal(const char* name, bool* p_open = NULL, 
                                   ImGuiWindowFlags flags = 0);
        
        static void EndPopup();
        
        static void OpenPopup(const char* str_id, ImGuiPopupFlags popup_flags = 0);
        
        static void CloseCurrentPopup();
        
        static bool IsPopupOpen(const char* str_id, ImGuiPopupFlags flags = 0);
        
        static bool BeginTabBar(const char* str_id, ImGuiTabBarFlags flags = 0);
        
        static void EndTabBar();
        
        static bool BeginTabItem(const char* label, bool* p_open = NULL, 
                                ImGuiTabItemFlags flags = 0);
        
        static void EndTabItem();
        
        static void SetTabItemClosed(const char* tab_or_docked_window_label);
        
        static void DockSpace(ImGuiID id, const ImVec2& size = ImVec2(0, 0), 
                             ImGuiDockNodeFlags flags = 0, 
                             const ImGuiWindowClass* window_class = NULL);
        static ImGuiID DockSpaceOverViewport(ImGuiID dockspace_id = 0,
                                   const ImGuiViewport* viewport = nullptr,
                                   ImGuiDockNodeFlags flags = 0,
                                   const ImGuiWindowClass* window_class = nullptr);
        
        static void SetNextWindowDockID(ImGuiID dock_id, ImGuiCond cond = 0);
        
        static ImGuiID GetWindowDockID();
        
        static bool IsWindowDocked();
    };
}