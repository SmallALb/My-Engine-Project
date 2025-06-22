#pragma once

#include "FISH/Renderer/Shader.h"

namespace FISH {
    class GLShader: public Shader {
    public:
        //GLShader(const std::string& ver, const std::string& frg);
        GLShader() {}
        ~GLShader();

        bool CompileLink() override;
        virtual bool readVertexShader(const string& ver);
        virtual bool readFragmentShader(const string& frg);
        void Begin() override;
        void End() override;
        void setFloat(const std::string& name, float value) override;
        void setVector3(const std::string& name, float x, float y, float z) override;
        void setVector3(const std::string& name, const float* values) override;
        void setVector3(const std::string& name, const glm::vec3& vector) override;
        void setInt(const std::string& name, int value) override;
        void setBool(const std::string& name, bool value) override;
        void setMat4(const std::string& name, glm::mat4 mat) override;
        private:
            //GL的着色器错误检查器
            void checkShaderErrors(unsigned int target, std::string type);
    
        private:
            //编译好的着色器程序
            uint32_t                    mProgram{0};
            //顶点着色器代码
            string                      vercode;
            //光栅化代码
            string                      fragcode;
    }; 


}