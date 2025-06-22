#pragma once

#ifdef _WIN32
    #ifdef FISH_DLL
        #define API_ __declspec(dllexport)
    #else
        #define API_ __declspec(dllimport)
    #endif
#else
    #ifdef FISH_DLL
        #define API_ __attribute__((visibility("default")))
    #else
        #define API_
    #endif
#endif

#define BIT(x) (1<<x)


#define FS_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)