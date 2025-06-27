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

        //ΪImgui�¿�һ������
        void Begin(bool KeyLocked = 0);
        //������ǰ��ImGui����
        void End();
    private:
        float mTime{0.0f};
        unsigned int main_dockspace_id{0};
    };

    //��ImguiUI��DLL��ķ�װ
    class API_ UI {
    public:
        static void Text(const char* fmt, ...);

        static bool InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
    };
}