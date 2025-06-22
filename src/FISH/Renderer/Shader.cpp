#include "fspcs.h"
#include "API.h"
#include "FISH/Log.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Shader.h"




namespace FISH {
    Shader* Shader::CreateShader() {
        switch(GetAPI()) {
            case RendererAPI::OpenGL:
                return new GLShader();
        }
        return nullptr;
    }
    
}
