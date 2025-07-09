#include "fspcs.h"
#include <GLFW/glfw3.h>
#include "FISH/Log.h"
#include "OpenGLController.h"

namespace FISH {
    static int GLControllerCount = 0;

    void GLController::init() {
        if (glfwJoystickPresent(GLControllerCount)) {
            mControllerID = GLControllerCount++;
            mName = glfwGetJoystickName(mControllerID);
            FS_INFO("Connecting GLController name:{0}", mName);
            ConnectTag = 1;
        }
        else {
            FS_WARN("UnSuccessed to connect GLController!");
            return;
        }

         // 按钮映射
        mButtonMappings = {
            {"A", 0},
            {"B", 1},
            {"X", 2},
            {"Y", 3},
            {"LB", 4},
            {"RB", 5},
            {"Back", 6},
            {"Start", 7},
            {"LeftStick", 8},
            {"RightStick", 9}
        };

        // 轴映射
        mAxisMappings = {
            {"LeftStickX", 0},
            {"LeftStickY", 1},
            {"RightStickX", 2},
            {"RightStickY", 3},
            {"LeftTrigger", 4},
            {"RightTrigger", 5},
            {"DPadX", 6},  // 方向键有时被映射为轴
            {"DPadY", 7}
        };

    }
    void GLController::update() {
        if (!ConnectTag) init();
        if (!ConnectTag) return;

        if (!glfwJoystickPresent(mControllerID)) return;
        
        int buttonCount;
        auto buttons = glfwGetJoystickButtons(mControllerID, &buttonCount);

        for (const auto& [name, idx] : mButtonMappings) if (idx < buttonCount)                                 {
            KeyInfoMap[name] = ControllerKeyInfo{idx, buttons[idx] == GLFW_PRESS};
        }

        int axisCount;
        auto axes = glfwGetJoystickAxes(mControllerID, &axisCount);

             
        for (const auto& [name, idx] : mAxisMappings) if (idx < axisCount) {
            float value = axes[idx];

            if (name == "LeftTrigger" || name == "RightTrigger") {
                value = (value + 1.0f) * 0.5f;
                KeyInfoMap[name] = ControllerKeyInfo{idx, value};
            }
            else if (name == "LeftStickX" || name == "LeftStickY") {
                KeyInfoMap[name] = ControllerKeyInfo{idx, glm::vec2(axes[0], axes[1])};
            }
            else if (name == "RightStickX" || name == "RightStickY") {
                KeyInfoMap[name] = ControllerKeyInfo{idx, glm::vec2(axes[2], axes[3])};
            }
            else if ((name == "DPadX" || name == "DPadY") && axisCount > 6) {
                glm::vec2 dpad(axes[6], axes[7]);
                KeyInfoMap["DPad"] = ControllerKeyInfo{6, dpad};
                
                // 同时提供单独的方向键按钮状态
                KeyInfoMap["DPad_Up"] = ControllerKeyInfo{10, dpad.y > 0.5f};
                KeyInfoMap["DPad_Down"] = ControllerKeyInfo{11, dpad.y < -0.5f};
                KeyInfoMap["DPad_Left"] = ControllerKeyInfo{12, dpad.x < -0.5f};
                KeyInfoMap["DPad_Right"] = ControllerKeyInfo{13, dpad.x > 0.5f};
            }
        }
    }

}