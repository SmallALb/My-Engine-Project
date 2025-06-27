#include "fspcs.h"
#include "Time.h"
#include "Renderer/API.h"
#include "Log.h"
#include <GLFW/glfw3.h>
#include <algorithm>

namespace FISH {
    namespace Time {
        float DeltaTime;     
        float FramePreSecond;
        float Step;



        float GetTime() {
            switch(GetAPI()) {
                case RendererAPI::OpenGL:
                    return (float)glfwGetTime();
            }

            FS_CORE_ERROR("找不到对应的API");
            return 0.0;
        }

        void SetFPS(float FPS) {
            FramePreSecond = FPS;
            Step = 1.0f / FPS;
        }

        void updateDt() {
            static float Lst = 0.0f;
            float current = GetTime();
            DeltaTime = current - Lst;
            Lst = current;
        }
    }
}

