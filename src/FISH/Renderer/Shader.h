#pragma once

#include "FISH/Core.h"


namespace FISH {
    //着色器
    class API_ Shader {
        public:
            //Shader(const string& ver, const string& frg);
        
        
            virtual ~Shader() {} 
            
            //编译链接
            virtual bool CompileLink () = 0;
            //读取顶点着色器
            virtual bool readVertexShader(const string& ver) = 0;
        
            //读取光栅着色器
            virtual bool readFragmentShader(const string& frg) = 0;
            //启动着色
            virtual void Begin() = 0;
            //关闭着色
            virtual void End() = 0;

            virtual void setFloat(const std::string& name, float value) = 0;
            virtual void setVector3(const std::string& name, float x, float y, float z) = 0;
            virtual void setVector3(const std::string& name, const float* values) = 0;
            virtual void setVector3(const std::string& name, const glm::vec3& vector) = 0;
            virtual void setInt(const std::string& name, int value) = 0;
            virtual void setBool(const std::string& name, bool value) = 0;
            virtual void setMat4(const std::string& name, glm::mat4 mat) = 0;
            virtual  void setTextureHandle(const std::string& name, uint64_t handle) = 0; 
            static Shader* CreateShader();
        private:
            //错误检查器
            //void checkShaderErrors(unsigned int target, std::string type);

        }; 

}
