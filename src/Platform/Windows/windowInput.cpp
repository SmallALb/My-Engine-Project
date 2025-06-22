#include "fspcs.h"
#include "FISH/Application.h"
#include "FISH/input.h"
#include "windowInput.h"
#include <GLFW/glfw3.h>


namespace FISH {

    Input &Input::GetInstance() {
        static WindowsInput Instance;
        return Instance;
    }

    bool WindowsInput::IsKeyPressedImpl(int keycode) {
        auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow()); 
        int state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT; 
    }

    bool WindowsInput::IsButtonPressedImpl(int button) {
        auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow()); 
        int state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    float WindowsInput::GetMouseXImpl() {
        auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow()); 
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return xpos;
    }

    float WindowsInput::GetMouseYImpl() {
        auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow()); 
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return ypos;
    }
    std::pair<float, float> WindowsInput::GetMousePosImpl(){
        auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow()); 
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return {xpos, ypos};
    }

}