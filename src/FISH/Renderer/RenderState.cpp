#include "fspcs.h"
#include "FISH/Log.h"
#include "API.h"
#include "Platform/OpenGL/OpenGLRenderState.h"
#include "RenderState.h"

namespace FISH {
    RenderState *RenderState::CreateRenderState() {
        switch (GetAPI()) {
            case RendererAPI::OpenGL: return new GLState();
        }
        FS_CORE_ERROR("Unknown API!");
        return nullptr;
    }
}