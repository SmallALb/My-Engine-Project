#include "fspcs.h"
#include "FISH/Log.h"
#include "API.h"
#include "Platform/OpenGL/OpenGLRenderstatus.h"
#include "Renderstatus.h"

namespace FISH {
    Renderstatus *Renderstatus::CreateRenderstatus() {
        switch (GetAPI()) {
            case RendererAPI::OpenGL: return new GLstatus();
        }
        FS_CORE_ERROR("Unknown API!");
        return nullptr;
    }
}