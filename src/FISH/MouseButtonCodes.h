#pragma once
#include <string_view>

#define FS_MOUSE_BUTTON_1         0
#define FS_MOUSE_BUTTON_2         1
#define FS_MOUSE_BUTTON_3         2
#define FS_MOUSE_BUTTON_4         3
#define FS_MOUSE_BUTTON_5         4
#define FS_MOUSE_BUTTON_6         5
#define FS_MOUSE_BUTTON_7         6
#define FS_MOUSE_BUTTON_8         7
#define FS_MOUSE_BUTTON_LAST      FS_MOUSE_BUTTON_8
#define FS_MOUSE_BUTTON_LEFT      FS_MOUSE_BUTTON_1
#define FS_MOUSE_BUTTON_RIGHT     FS_MOUSE_BUTTON_2
#define FS_MOUSE_BUTTON_MIDDLE    FS_MOUSE_BUTTON_3


#pragma once

namespace FISH {

    // 鼠标按键枚举类
    enum class MouseCode {
        Button1 = 0,
        Button2 = 1,
        Button3 = 2,
        Button4 = 3,
        Button5 = 4,
        Button6 = 5,
        Button7 = 6,
        Button8 = 7,
        
        Last    = Button8,
        Left    = Button1,
        Right   = Button2,
        Middle  = Button3
    };

    #define MOUSECODE_TO_STRING_CASE(code) case MouseCode::code: return #code


  constexpr std::string_view MouseCodeToString(MouseCode button)
    {
        switch (button)
        {
            MOUSECODE_TO_STRING_CASE(Button1);
            MOUSECODE_TO_STRING_CASE(Button2);
            MOUSECODE_TO_STRING_CASE(Button3);
            MOUSECODE_TO_STRING_CASE(Button4);
            MOUSECODE_TO_STRING_CASE(Button5);
            MOUSECODE_TO_STRING_CASE(Button6);
            MOUSECODE_TO_STRING_CASE(Button7);
            MOUSECODE_TO_STRING_CASE(Button8);
            
            default: return "Unknown";
        }
    }

} // namespace FISH