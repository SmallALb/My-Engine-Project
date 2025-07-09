#include "fspcs.h"
#include <variant>
#include "Platform/OpenGL/OpenGLController.h"
#include "Platform/Windows/WindowController.h"
#include "Renderer/API.h"
#include "Controller.h"

namespace FISH {
    Controller* Controller::CreateController() {
        switch(GetAPI()) {
            #ifdef _WIN32
                case RendererAPI::OpenGL: return new XController();
            #else
                case RendererAPI::OpenGL: return new GLController();
            #endif
        }

        return nullptr;
    }
} 