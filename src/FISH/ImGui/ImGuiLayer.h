#pragma once

#include "../Core.h"
#include "../Layer.h"
#include "../Events/MouseEvent.h"
#include "../Events/KeyEvent.h"
#include "../Events/Application.h"

//UIcolor
#ifndef IM_COL32_R_SHIFT
#ifdef IMGUI_USE_BGRA_PACKED_COLOR
#define IM_COL32_R_SHIFT    16
#define IM_COL32_G_SHIFT    8
#define IM_COL32_B_SHIFT    0
#define IM_COL32_A_SHIFT    24
#define IM_COL32_A_MASK     0xFF000000
#else
#define IM_COL32_R_SHIFT    0
#define IM_COL32_G_SHIFT    8
#define IM_COL32_B_SHIFT    16
#define IM_COL32_A_SHIFT    24
#define IM_COL32_A_MASK     0xFF000000
#endif
#endif
#define IM_COL32(R,G,B,A)    (((uint32_t)(A)<<IM_COL32_A_SHIFT) | ((uint32_t)(B)<<IM_COL32_B_SHIFT) | ((uint32_t)(G)<<IM_COL32_G_SHIFT) | ((uint32_t)(R)<<IM_COL32_R_SHIFT))
#define IM_COL32_WHITE       IM_COL32(255,255,255,255)  // Opaque white = 0xFFFFFFFF
#define IM_COL32_BLACK       IM_COL32(0,0,0,255)        // Opaque black
#define IM_COL32_BLACK_TRANS IM_COL32(0,0,0,0)          // Transparent black = 0x00000000

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
    };
}