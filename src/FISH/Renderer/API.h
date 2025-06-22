#pragma once

#include "FISH/Core.h"

namespace FISH {
    enum class RendererAPI {
        None = 0,
        OpenGL
    };

    RendererAPI GetAPI();
}
