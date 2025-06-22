#include "fspcs.h"
#include "VertexArray.h"

#include "API.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "FISH/Log.h"

namespace FISH {

    VertexArray* VertexArray::Create() {
        switch (GetAPI())
        {
            case RendererAPI::None: return nullptr;
            case RendererAPI::OpenGL: return new GLVertexArray();
        }
        FS_CORE_ERROR("RendererAPI::Create() : 该API不支持!");
        return nullptr;
    }
}