#include "fspcs.h"

#include "DefaultShader.h"

namespace FISH {
    namespace Renderer {
            std::unordered_map<DefaultShaderName, ShaderPtr> DefaultShader::Shaders{};
            
            void DefaultShader::buildAlltheDefaultShader() {
                Shaders[DefaultShaderName::None] = nullptr;
                //OnlyColor
                Shaders[DefaultShaderName::OnlyColor3D].reset(Shader::CreateShader());
                Shaders[DefaultShaderName::OnlyColor3D]->readVertexShader("sharders/EnginRenderShader/3Dvertex.glsl");
                Shaders[DefaultShaderName::OnlyColor3D]->readFragmentShader("sharders/EnginRenderShader/OnlyColor.glsl");
                Shaders[DefaultShaderName::OnlyColor3D]->CompileLink();
                
                //OnlyColor2D
                Shaders[DefaultShaderName::OnlyColor2D].reset(Shader::CreateShader());
                Shaders[DefaultShaderName::OnlyColor2D]->readVertexShader("sharders/EnginRenderShader/2Dvertex.glsl");
                Shaders[DefaultShaderName::OnlyColor2D]->readFragmentShader("sharders/EnginRenderShader/OnlyColor.glsl");
                Shaders[DefaultShaderName::OnlyColor2D]->CompileLink();
                
                //Texture3D
                Shaders[DefaultShaderName::Texture3D].reset(Shader::CreateShader());
                Shaders[DefaultShaderName::Texture3D]->readVertexShader("sharders/EnginRenderShader/3Dvertex.glsl");
                Shaders[DefaultShaderName::Texture3D]->readFragmentShader("sharders/EnginRenderShader/Texture.glsl");
                Shaders[DefaultShaderName::Texture3D]->CompileLink();

                //Texture2D
                Shaders[DefaultShaderName::Texture2D].reset(Shader::CreateShader());
                Shaders[DefaultShaderName::Texture2D]->readVertexShader("sharders/EnginRenderShader/2Dvertex.glsl");
                Shaders[DefaultShaderName::Texture2D]->readFragmentShader("sharders/EnginRenderShader/Texture.glsl");
                Shaders[DefaultShaderName::Texture2D]->CompileLink();

                //PhoneLight
                Shaders[DefaultShaderName::PhongLight].reset(Shader::CreateShader());
                Shaders[DefaultShaderName::PhongLight]->readVertexShader("sharders/EnginRenderShader/3Dvertex.glsl");
                Shaders[DefaultShaderName::PhongLight]->readFragmentShader("sharders/EnginRenderShader/NormalFragment.glsl");
                Shaders[DefaultShaderName::PhongLight]->CompileLink();
            }

            ShaderPtr DefaultShader::getShaderFromName(DefaultShaderName name) {
                return Shaders[name];
            }
    }
}