#pragma once
#include "../Shader.h"
namespace FISH {
    namespace Renderer {
        //默认着色器名字
        enum class DefaultShaderName {
            None,
            OnlyColor3D,
            OnlyColor2D,
            Texture3D,
            Texture2D,
            PhongLight,
            PBRLight,
        };


        //默认着色器表
        class API_ DefaultShader{
            static std::unordered_map<DefaultShaderName, ShaderPtr> Shaders;
        public:
            static void buildAlltheDefaultShader();    

            static ShaderPtr getShaderFromName(DefaultShaderName name);
        };

    }

}