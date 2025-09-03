#pragma once
#include <string_view>


#define FS_KEY_SPACE              32
#define FS_KEY_APOSTROPHE         39  
#define FS_KEY_COMMA              44  
#define FS_KEY_MINUS              45  
#define FS_KEY_PERIOD             46  
#define FS_KEY_SLASH              47  
#define FS_KEY_0                  48
#define FS_KEY_1                  49
#define FS_KEY_2                  50
#define FS_KEY_3                  51
#define FS_KEY_4                  52
#define FS_KEY_5                  53
#define FS_KEY_6                  54
#define FS_KEY_7                  55
#define FS_KEY_8                  56
#define FS_KEY_9                  57
#define FS_KEY_SEMICOLON          59  
#define FS_KEY_EQUAL              61  
#define FS_KEY_A                  65
#define FS_KEY_B                  66
#define FS_KEY_C                  67
#define FS_KEY_D                  68
#define FS_KEY_E                  69
#define FS_KEY_F                  70
#define FS_KEY_G                  71
#define FS_KEY_H                  72
#define FS_KEY_I                  73
#define FS_KEY_J                  74
#define FS_KEY_K                  75
#define FS_KEY_L                  76
#define FS_KEY_M                  77
#define FS_KEY_N                  78
#define FS_KEY_O                  79
#define FS_KEY_P                  80
#define FS_KEY_Q                  81
#define FS_KEY_R                  82
#define FS_KEY_S                  83
#define FS_KEY_T                  84
#define FS_KEY_U                  85
#define FS_KEY_V                  86
#define FS_KEY_W                  87
#define FS_KEY_X                  88
#define FS_KEY_Y                  89
#define FS_KEY_Z                  90
#define FS_KEY_LEFT_BRACKET       91  
#define FS_KEY_BACKSLASH          92  
#define FS_KEY_RIGHT_BRACKET      93  
#define FS_KEY_GRAVE_ACCENT       96  
#define FS_KEY_WORLD_1            161 
#define FS_KEY_WORLD_2            162 

/* Function keys */
#define FS_KEY_ESCAPE             256
#define FS_KEY_ENTER              257
#define FS_KEY_TAB                258
#define FS_KEY_BACKSPACE          259
#define FS_KEY_INSERT             260
#define FS_KEY_DELETE             261
#define FS_KEY_RIGHT              262
#define FS_KEY_LEFT               263
#define FS_KEY_DOWN               264
#define FS_KEY_UP                 265
#define FS_KEY_PAGE_UP            266
#define FS_KEY_PAGE_DOWN          267
#define FS_KEY_HOME               268
#define FS_KEY_END                269
#define FS_KEY_CAPS_LOCK          280
#define FS_KEY_SCROLL_LOCK        281
#define FS_KEY_NUM_LOCK           282
#define FS_KEY_PRINT_SCREEN       283
#define FS_KEY_PAUSE              284
#define FS_KEY_F1                 290
#define FS_KEY_F2                 291
#define FS_KEY_F3                 292
#define FS_KEY_F4                 293
#define FS_KEY_F5                 294
#define FS_KEY_F6                 295
#define FS_KEY_F7                 296
#define FS_KEY_F8                 297
#define FS_KEY_F9                 298
#define FS_KEY_F10                299
#define FS_KEY_F11                300
#define FS_KEY_F12                301
#define FS_KEY_F13                302
#define FS_KEY_F14                303
#define FS_KEY_F15                304
#define FS_KEY_F16                305
#define FS_KEY_F17                306
#define FS_KEY_F18                307
#define FS_KEY_F19                308
#define FS_KEY_F20                309
#define FS_KEY_F21                310
#define FS_KEY_F22                311
#define FS_KEY_F23                312
#define FS_KEY_F24                313
#define FS_KEY_F25                314
#define FS_KEY_KP_0               320
#define FS_KEY_KP_1               321
#define FS_KEY_KP_2               322
#define FS_KEY_KP_3               323
#define FS_KEY_KP_4               324
#define FS_KEY_KP_5               325
#define FS_KEY_KP_6               326
#define FS_KEY_KP_7               327
#define FS_KEY_KP_8               328
#define FS_KEY_KP_9               329
#define FS_KEY_KP_DECIMAL         330
#define FS_KEY_KP_DIVIDE          331
#define FS_KEY_KP_MULTIPLY        332
#define FS_KEY_KP_SUBTRACT        333
#define FS_KEY_KP_ADD             334
#define FS_KEY_KP_ENTER           335
#define FS_KEY_KP_EQUAL           336
#define FS_KEY_LEFT_SHIFT         340
#define FS_KEY_LEFT_CONTROL       341
#define FS_KEY_LEFT_ALT           342
#define FS_KEY_LEFT_SUPER         343
#define FS_KEY_RIGHT_SHIFT        344
#define FS_KEY_RIGHT_CONTROL      345
#define FS_KEY_RIGHT_ALT          346
#define FS_KEY_RIGHT_SUPER        347
#define FS_KEY_MENU               348


#pragma once

namespace FISH {

    // 按键枚举类
    enum class KeyCode {
        // 可打印键
        None = 0,
        Space = 32,
        Apostrophe = 39,
        Comma = 44,
        Minus = 45,
        Period = 46,
        Slash = 47,
        Num0 = 48,
        Num1 = 49,
        Num2 = 50,
        Num3 = 51,
        Num4 = 52,
        Num5 = 53,
        Num6 = 54,
        Num7 = 55,
        Num8 = 56,
        Num9 = 57,
        Semicolon = 59,
        Equal = 61,
        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,
        LeftBracket = 91,
        Backslash = 92,
        RightBracket = 93,
        GraveAccent = 96,
        World1 = 161,
        World2 = 162,

        // 功能键
        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,
        CapsLock = 280,
        ScrollLock = 281,
        NumLock = 282,
        PrintScreen = 283,
        Pause = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        F23 = 312,
        F24 = 313,
        F25 = 314,
        KP0 = 320,
        KP1 = 321,
        KP2 = 322,
        KP3 = 323,
        KP4 = 324,
        KP5 = 325,
        KP6 = 326,
        KP7 = 327,
        KP8 = 328,
        KP9 = 329,
        KPDecimal = 330,
        KPDivide = 331,
        KPMultiply = 332,
        KPSubtract = 333,
        KPAdd = 334,
        KPEnter = 335,
        KPEqual = 336,
        LeftShift = 340,
        LeftControl = 341,
        LeftAlt = 342,
        LeftSuper = 343,
        RightShift = 344,
        RightControl = 345,
        RightAlt = 346,
        RightSuper = 347,
        Menu = 348
    };

    #define KEYCODE_TO_STRING_CASE(code) case KeyCode::code: return #code

    constexpr std::string_view KeyCodeToString(KeyCode key)
    {
        switch (key)
        {
            KEYCODE_TO_STRING_CASE(None);
            KEYCODE_TO_STRING_CASE(Space);
            KEYCODE_TO_STRING_CASE(Apostrophe);
            KEYCODE_TO_STRING_CASE(Comma);
            KEYCODE_TO_STRING_CASE(Minus);
            KEYCODE_TO_STRING_CASE(Period);
            KEYCODE_TO_STRING_CASE(Slash);
            KEYCODE_TO_STRING_CASE(Num0);
            KEYCODE_TO_STRING_CASE(Num1);
            KEYCODE_TO_STRING_CASE(Num2);
            KEYCODE_TO_STRING_CASE(Num3);
            KEYCODE_TO_STRING_CASE(Num4);
            KEYCODE_TO_STRING_CASE(Num5);
            KEYCODE_TO_STRING_CASE(Num6);
            KEYCODE_TO_STRING_CASE(Num7);
            KEYCODE_TO_STRING_CASE(Num8);
            KEYCODE_TO_STRING_CASE(Num9);
            KEYCODE_TO_STRING_CASE(Semicolon);
            KEYCODE_TO_STRING_CASE(Equal);
            KEYCODE_TO_STRING_CASE(A);
            KEYCODE_TO_STRING_CASE(B);
            KEYCODE_TO_STRING_CASE(C);
            KEYCODE_TO_STRING_CASE(D);
            KEYCODE_TO_STRING_CASE(E);
            KEYCODE_TO_STRING_CASE(F);
            KEYCODE_TO_STRING_CASE(G);
            KEYCODE_TO_STRING_CASE(H);
            KEYCODE_TO_STRING_CASE(I);
            KEYCODE_TO_STRING_CASE(J);
            KEYCODE_TO_STRING_CASE(K);
            KEYCODE_TO_STRING_CASE(L);
            KEYCODE_TO_STRING_CASE(M);
            KEYCODE_TO_STRING_CASE(N);
            KEYCODE_TO_STRING_CASE(O);
            KEYCODE_TO_STRING_CASE(P);
            KEYCODE_TO_STRING_CASE(Q);
            KEYCODE_TO_STRING_CASE(R);
            KEYCODE_TO_STRING_CASE(S);
            KEYCODE_TO_STRING_CASE(T);
            KEYCODE_TO_STRING_CASE(U);
            KEYCODE_TO_STRING_CASE(V);
            KEYCODE_TO_STRING_CASE(W);
            KEYCODE_TO_STRING_CASE(X);
            KEYCODE_TO_STRING_CASE(Y);
            KEYCODE_TO_STRING_CASE(Z);
            KEYCODE_TO_STRING_CASE(LeftBracket);
            KEYCODE_TO_STRING_CASE(Backslash);
            KEYCODE_TO_STRING_CASE(RightBracket);
            KEYCODE_TO_STRING_CASE(GraveAccent);
            KEYCODE_TO_STRING_CASE(World1);
            KEYCODE_TO_STRING_CASE(World2);
            KEYCODE_TO_STRING_CASE(Escape);
            KEYCODE_TO_STRING_CASE(Enter);
            KEYCODE_TO_STRING_CASE(Tab);
            KEYCODE_TO_STRING_CASE(Backspace);
            KEYCODE_TO_STRING_CASE(Insert);
            KEYCODE_TO_STRING_CASE(Delete);
            KEYCODE_TO_STRING_CASE(Right);
            KEYCODE_TO_STRING_CASE(Left);
            KEYCODE_TO_STRING_CASE(Down);
            KEYCODE_TO_STRING_CASE(Up);
            KEYCODE_TO_STRING_CASE(PageUp);
            KEYCODE_TO_STRING_CASE(PageDown);
            KEYCODE_TO_STRING_CASE(Home);
            KEYCODE_TO_STRING_CASE(End);
            KEYCODE_TO_STRING_CASE(CapsLock);
            KEYCODE_TO_STRING_CASE(ScrollLock);
            KEYCODE_TO_STRING_CASE(NumLock);
            KEYCODE_TO_STRING_CASE(PrintScreen);
            KEYCODE_TO_STRING_CASE(Pause);
            KEYCODE_TO_STRING_CASE(F1);
            KEYCODE_TO_STRING_CASE(F2);
            KEYCODE_TO_STRING_CASE(F3);
            KEYCODE_TO_STRING_CASE(F4);
            KEYCODE_TO_STRING_CASE(F5);
            KEYCODE_TO_STRING_CASE(F6);
            KEYCODE_TO_STRING_CASE(F7);
            KEYCODE_TO_STRING_CASE(F8);
            KEYCODE_TO_STRING_CASE(F9);
            KEYCODE_TO_STRING_CASE(F10);
            KEYCODE_TO_STRING_CASE(F11);
            KEYCODE_TO_STRING_CASE(F12);
            KEYCODE_TO_STRING_CASE(F13);
            KEYCODE_TO_STRING_CASE(F14);
            KEYCODE_TO_STRING_CASE(F15);
            KEYCODE_TO_STRING_CASE(F16);
            KEYCODE_TO_STRING_CASE(F17);
            KEYCODE_TO_STRING_CASE(F18);
            KEYCODE_TO_STRING_CASE(F19);
            KEYCODE_TO_STRING_CASE(F20);
            KEYCODE_TO_STRING_CASE(F21);
            KEYCODE_TO_STRING_CASE(F22);
            KEYCODE_TO_STRING_CASE(F23);
            KEYCODE_TO_STRING_CASE(F24);
            KEYCODE_TO_STRING_CASE(F25);
            KEYCODE_TO_STRING_CASE(KP0);
            KEYCODE_TO_STRING_CASE(KP1);
            KEYCODE_TO_STRING_CASE(KP2);
            KEYCODE_TO_STRING_CASE(KP3);
            KEYCODE_TO_STRING_CASE(KP4);
            KEYCODE_TO_STRING_CASE(KP5);
            KEYCODE_TO_STRING_CASE(KP6);
            KEYCODE_TO_STRING_CASE(KP7);
            KEYCODE_TO_STRING_CASE(KP8);
            KEYCODE_TO_STRING_CASE(KP9);
            KEYCODE_TO_STRING_CASE(KPDecimal);
            KEYCODE_TO_STRING_CASE(KPDivide);
            KEYCODE_TO_STRING_CASE(KPMultiply);
            KEYCODE_TO_STRING_CASE(KPSubtract);
            KEYCODE_TO_STRING_CASE(KPAdd);
            KEYCODE_TO_STRING_CASE(KPEnter);
            KEYCODE_TO_STRING_CASE(KPEqual);
            KEYCODE_TO_STRING_CASE(LeftShift);
            KEYCODE_TO_STRING_CASE(LeftControl);
            KEYCODE_TO_STRING_CASE(LeftAlt);
            KEYCODE_TO_STRING_CASE(LeftSuper);
            KEYCODE_TO_STRING_CASE(RightShift);
            KEYCODE_TO_STRING_CASE(RightControl);
            KEYCODE_TO_STRING_CASE(RightAlt);
            KEYCODE_TO_STRING_CASE(RightSuper);
            KEYCODE_TO_STRING_CASE(Menu);
            
            default: return "Unknown";
        }
    }



} // namespace FISH