// TracyProfiler.h
#pragma once

#ifdef FISH_ENABLE_PROFILING
    #define TRACY_ENABLE
    #include <tracy/Tracy.hpp>
    #include <tracy/TracyOpenGL.hpp>

    // 基础区域标记
    #define FS_PROFILE_SCOPE(name) TracyZoneScopedN(name)
    #define FS_PROFILE_FUNCTION() TracyZoneScoped
    #define FS_PROFILE_THREAD(name) tracy::SetThreadName(name)

    // 帧标记
    #define FS_PROFILE_FRAME() FrameMark
    #define FS_PROFILE_FRAME_START(name) FrameMarkStart(name)
    #define FS_PROFILE_FRAME_END(name) FrameMarkEnd(name)

    // 消息输出
    #define FS_PROFILE_MESSAGE(msg) TracyMessage(msg, strlen(msg))
    #define FS_PROFILE_MESSAGE_L(msg, len) TracyMessage(msg, len)
    #define FS_PROFILE_MESSAGE_COLOR(msg, color) TracyMessageC(msg, strlen(msg), color)

    // 数值记录
    #define FS_PROFILE_PLOT(name, value) TracyPlot(name, value)
    #define FS_PROFILE_COUNTER(name, value) TracyPlot(name, value)

    // GPU 相关 (OpenGL)
    #define FS_PROFILE_GPU_CONTEXT(name) TracyGpuContext(name)
    #define FS_PROFILE_GPU_SCOPE(name) TracyGpuZone(name)
    #define FS_PROFILE_GPU_COLLECT() TracyGpuCollect

    // 内存分配跟踪
    #define FS_PROFILE_ALLOC(ptr, size) TracyAlloc(ptr, size)
    #define FS_PROFILE_FREE(ptr) TracyFree(ptr)

    // 锁监控
    #define FS_PROFILE_LOCK_MUTEX(mutex) TracyLockable(mutex, std::mutex)
    #define FS_PROFILE_LOCK_BASE(type, name) TracyLockableN(type, name)
    #define FS_PROFILE_LOCK(mutex) LockMark(mutex)

#else
    // 空实现 - 发布版本中不包含任何性能分析代码
    #define FS_PROFILE_SCOPE(name)
    #define FS_PROFILE_FUNCTION()
    #define FS_PROFILE_THREAD(name)

    #define FS_PROFILE_FRAME()
    #define FS_PROFILE_FRAME_START(name)
    #define FS_PROFILE_FRAME_END(name)

    #define FS_PROFILE_MESSAGE(msg)
    #define FS_PROFILE_MESSAGE_L(msg, len)
    #define FS_PROFILE_MESSAGE_COLOR(msg, color)

    #define FS_PROFILE_PLOT(name, value)
    #define FS_PROFILE_COUNTER(name, value)

    #define FS_PROFILE_GPU_CONTEXT(name)
    #define FS_PROFILE_GPU_SCOPE(name)
    #define FS_PROFILE_GPU_COLLECT()

    #define FS_PROFILE_ALLOC(ptr, size)
    #define FS_PROFILE_FREE(ptr)

    #define FS_PROFILE_LOCK_MUTEX(mutex)
    #define FS_PROFILE_LOCK_BASE(type, name)
    #define FS_PROFILE_LOCK(mutex)
#endif