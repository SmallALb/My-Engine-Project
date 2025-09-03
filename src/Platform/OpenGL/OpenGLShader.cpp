#include "fspcs.h"
#include "glad/glad.h"
#include <fstream>
#include <sstream>
#include "FISH/Log.h"
#include "OpenGLErrorCheck.h"
#include "OpenGLShader.h"


extern PFNGLUNIFORMHANDLEUI64ARBPROC glUniformHandleui64ARB;

namespace FISH {
    GLShader::~GLShader() {
        glDeleteProgram(mProgram);
    }
    
    bool GLShader::CompileLink() {

        uint32_t vertex, fragment;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
    
        const char* v = vercode.c_str();
        const char* f = fragcode.c_str();
        //2 为shader程序输入shader代码
        glShaderSource(vertex, 1, &v, NULL);
        glShaderSource(fragment, 1, &f, NULL);
        //编译
        glCompileShader(vertex);
        checkShaderErrors(vertex, "COMPILE");
        glCompileShader(fragment);
        checkShaderErrors(fragment, "COMPILE");
        //创建着色器程序
        mProgram = glCreateProgram();
        //添加
        glAttachShader(mProgram, vertex);
        glAttachShader(mProgram, fragment);    
        //链接
        glLinkProgram(mProgram);
        checkShaderErrors(mProgram, "LINK");
        //清理资源
        glDeleteShader(vertex);
        glDeleteShader(fragment);

        FS_INFO("Shader Had been complied!");
    
        return true;
    }
    
    bool GLShader::readVertexShader(const string &ver) {
        std::ifstream filestream;
        filestream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            filestream.open(ver);
            std::stringstream stream;
            stream << filestream.rdbuf();
            filestream.close();
    
            vercode = stream.str();
        }
        catch (std::ifstream::failure& e){

            //std::cout << "ERROR: Vertex Shader File Error: " << e.what() << std::endl;
            FS_ERROR("ERROR: Vertex Shader File Error: {0}",  e.what());

            return false;
        }
        return true;
    }
    
    bool GLShader::readFragmentShader(const string &frg) {
        std::ifstream filestream;
        filestream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            filestream.open(frg);
            std::stringstream stream;
            stream << filestream.rdbuf();
            filestream.close();
    
            fragcode = stream.str();
        }
        catch (std::ifstream::failure& e){

            //std::cout << "ERROR: Vertex Shader File Error: " << e.what() << std::endl;
            FS_ERROR("ERROR: Fragment Shader File Error: {0}",  e.what());
            return false;
        }
        return true;
    }
    
    void GLShader::Begin() {
        glUseProgram(mProgram);
    }
    
    void GLShader::End() {
        glUseProgram(0);
    }
    
    void GLShader::checkShaderErrors(GLuint target, std::string type) {
        int success = 0;
        char infoLog[1024];
    
        if (type == "COMPILE") {
            glGetShaderiv(target, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(target, 1024, NULL, infoLog);
                //std::cout << "Error: SHADER COMPILE ERROR" << "\n" << infoLog << std::endl;
                FS_ERROR("Error: SHADER COMPILE ERROR: {0}",  infoLog);

            }
        }
        else if (type == "LINK") {
            glGetProgramiv(target, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(target, 1024, NULL, infoLog);
                //std::cout << "Error: SHADER LINK ERROR " << "\n" << infoLog << std::endl;
                FS_ERROR("Error: SHADER LINK ERROR: {0}",  infoLog);
                
            }
        }
        else {
            //std::cout << "Error: Check shader errors Type is wrong" << std::endl;
            FS_ERROR("Error: Check shader errors Type is wrong");
            
        }
    }

    void GLShader::setFloat(const std::string& name, float value) {
        unsigned int location = (glGetUniformLocation(mProgram, name.c_str()));
        GL_ERRORCALL(glUniform1f(location, value));
    }
    
    void GLShader::setVector3(const std::string& name, float x, float y, float z) {
        unsigned int location = (glGetUniformLocation(mProgram, name.c_str()));
        GL_ERRORCALL(glUniform3f(location, x, y, z));
    }
    
    void GLShader::setVector3(const std::string& name, const float* values) {
        unsigned int location = (glGetUniformLocation(mProgram, name.c_str()));
        GL_ERRORCALL(glUniform3fv(location, 1, values));
    }
    
    void GLShader::setVector3(const std::string& name, const glm::vec3& vector) {
        unsigned int location = (glGetUniformLocation(mProgram, name.c_str()));
        GL_ERRORCALL(glUniform3fv(location, 1, glm::value_ptr(vector)));
    }

    void GLShader::setVector2(const std::string &name, const glm::vec2 &vector) {
        unsigned int location = (glGetUniformLocation(mProgram, name.c_str()));
        GL_ERRORCALL(glUniform2fv(location, 1, glm::value_ptr(vector)));
    }

    void GLShader::setInt(const std::string& name, int value) {
        unsigned int location = (glGetUniformLocation(mProgram, name.c_str()));
        GL_ERRORCALL(glUniform1i(location, value));
    }

    void GLShader::setBool(const std::string &name, bool value) {
        unsigned int location = (glGetUniformLocation(mProgram, name.c_str()));
        GL_ERRORCALL(glUniform1i(location, value));
    }

    void GLShader::setMat4(const std::string& name, glm::mat4 mat) {
        unsigned int location = (glGetUniformLocation(mProgram, name.c_str()));
        GL_ERRORCALL(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat)));
    }

    void GLShader::setTextureHandle(const std::string &name, uint64_t handle) {
        if (!GLAD_GL_ARB_bindless_texture) {
            static bool warned = false;
            if (!warned) {
                FS_CORE_ERROR("GL_ARB_bindless_texture not supported!");
                warned = true;
            }
            return;
        }
        unsigned int location = glGetUniformLocation(mProgram, name.c_str());
        GL_ERRORCALL(glUniformHandleui64ARB(location, handle));
        
    }
    unsigned int GLShader::getUniformLocation(const std::string &name) {
        return 0;
    }
}