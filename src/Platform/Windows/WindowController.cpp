#include "fspcs.h"
#include "FISH/Log.h"
#include "WindowController.h"

namespace FISH {
    void XController::init() {
    #ifdef _WIN32
        ZeroMemory(&mCurrentState, sizeof(XINPUT_STATE));
        if (XInputGetState(mControllerId, &mCurrentState) == ERROR_SUCCESS) {
            mName = "XInput Controller";
            setupButtonMappings();
            FS_INFO("XInput Controller connected: {0}", mName);
            ConnectTag = 1;
        } else {
            if (!WARNTag) FS_WARN("Xbox Controller not connected!"), WARNTag = 1;
        }
    #endif
    }

    void XController::update() {
        if (!ConnectTag) init();
        if (!ConnectTag) return;
    #ifdef _WIN32
        ZeroMemory(&mCurrentState, sizeof(XINPUT_STATE));
        if (XInputGetState(mControllerId, &mCurrentState) != ERROR_SUCCESS) {
            return;
        }

        KeyInfoMap["A"].data = (mCurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
        KeyInfoMap["B"].data = (mCurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
        KeyInfoMap["X"].data = (mCurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
        KeyInfoMap["Y"].data = (mCurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;
        KeyInfoMap["LB"].data = (mCurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
        KeyInfoMap["RB"].data = (mCurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
        KeyInfoMap["Back"].data = (mCurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
        KeyInfoMap["Start"].data = (mCurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
        KeyInfoMap["LeftStick"].data = (mCurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0;
        KeyInfoMap["RightStick"].data = (mCurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0;
        KeyInfoMap["DPad_Up"].data = (mCurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
        KeyInfoMap["DPad_Down"].data = (mCurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
        KeyInfoMap["DPad_Left"].data = (mCurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
        KeyInfoMap["DPad_Right"].data = (mCurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;
        
        //归一化
        float leftX = mCurrentState.Gamepad.sThumbLX / 32767.0f;
        float leftY = mCurrentState.Gamepad.sThumbLY / 32767.0f;
        float rightX = mCurrentState.Gamepad.sThumbRX / 32767.0f;
        float rightY = mCurrentState.Gamepad.sThumbRY / 32767.0f;

        // 应用死区过滤
        if (fabs(leftX) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / 32767.0f) leftX = 0;
        if (fabs(leftY) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / 32767.0f) leftY = 0;
        if (fabs(rightX) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE / 32767.0f) rightX = 0;
        if (fabs(rightY) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE / 32767.0f) rightY = 0;

        KeyInfoMap["LeftStickX"].data = leftX;
        KeyInfoMap["LeftStickY"].data = leftY;
        KeyInfoMap["RightStickX"].data = rightX;
        KeyInfoMap["RightStickY"].data = rightY;

        // 更新触发器状态
        KeyInfoMap["LeftTrigger"].data = mCurrentState.Gamepad.bLeftTrigger / 255.0f;
        KeyInfoMap["RightTrigger"].data = mCurrentState.Gamepad.bRightTrigger / 255.0f;
    #endif

    }
    void XController::setVibration(float leftMotor, float rightMotor) {
    #ifdef _WIN32
        XINPUT_VIBRATION vibration;
        ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
        //FS_INFO("lef:{0}, rig:{1}", leftMotor, rightMotor);
        vibration.wLeftMotorSpeed = static_cast<WORD>(leftMotor * 65535.0f);
        vibration.wRightMotorSpeed = static_cast<WORD>(rightMotor * 65535.0f);
        
        XInputSetState(mControllerId, &vibration);
    #endif
    }
    void XController::setupButtonMappings() {
        KeyInfoMap["A"] = ControllerKeyInfo{0, false};
        KeyInfoMap["B"] = ControllerKeyInfo{1, false};
        KeyInfoMap["X"] = ControllerKeyInfo{2, false};
        KeyInfoMap["Y"] = ControllerKeyInfo{3, false};
        KeyInfoMap["LB"] = ControllerKeyInfo{4, false};
        KeyInfoMap["RB"] = ControllerKeyInfo{5, false};
        KeyInfoMap["Back"] = ControllerKeyInfo{6, false};
        KeyInfoMap["Start"] = ControllerKeyInfo{7, false};
        KeyInfoMap["LeftStick"] = ControllerKeyInfo{8, false};
        KeyInfoMap["RightStick"] = ControllerKeyInfo{9, false};
        KeyInfoMap["DPad_Up"] = ControllerKeyInfo{10, false};
        KeyInfoMap["DPad_Down"] = ControllerKeyInfo{11, false};
        KeyInfoMap["DPad_Left"] = ControllerKeyInfo{12, false};
        KeyInfoMap["DPad_Right"] = ControllerKeyInfo{13, false};

        // 摇杆和触发器映射
        KeyInfoMap["LeftStickX"] = ControllerKeyInfo{14, 0.0f};
        KeyInfoMap["LeftStickY"] = ControllerKeyInfo{15, 0.0f};
        KeyInfoMap["RightStickX"] = ControllerKeyInfo{16, 0.0f};
        KeyInfoMap["RightStickY"] = ControllerKeyInfo{17, 0.0f};
        KeyInfoMap["LeftTrigger"] = ControllerKeyInfo{18, 0.0f};
        KeyInfoMap["RightTrigger"] = ControllerKeyInfo{19, 0.0f};
    }
}   