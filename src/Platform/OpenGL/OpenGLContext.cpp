#include "fspcs.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include "glad/glad.h"
#include "FISH/Log.h"

namespace FISH {
    OpenGLContext::OpenGLContext(GLFWwindow *WindowHandle) : mHandle(WindowHandle) {
        //FS_CORE_ASSERT(WindowHandle, "Window Handle为空");
    }

    OpenGLContext::~OpenGLContext() {

    }

    void OpenGLContext::init() {
        glfwMakeContextCurrent(mHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        //FS_CORE_ASSERT(status, "failed:无法初始化GLAD");
        auto version_info = (const char*)glGetString(GL_VERSION);
        auto render_info = (const char*)glGetString(GL_RENDERER);
        int maxUniforms = 0;
        glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxUniforms);
        FS_CORE_INFO("Max fragment uniforms: {0}", maxUniforms);
        FS_CORE_INFO("{0}",version_info);
        FS_CORE_INFO("{0}",render_info); 
        //glEnable(GL_DEPTH_TEST);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        glfwWindowHint(GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    void OpenGLContext::SwapBuffers() {
        glfwSwapBuffers(mHandle);
    }

}
