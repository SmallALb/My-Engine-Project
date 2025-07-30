#pragma once

#include "fspcs.h"
#include "Events/Events.h"
#include "Events/Application.h"
#include "Core.h"

namespace FISH {
    //窗口的基本属性
    struct WindowProps {
        string Title;
        unsigned int Width;
        unsigned int Height;
        
        WindowProps(
            const string& title = "Fish Engin",
            unsigned int width = 1280,
            unsigned int height = 720
        ): Title(title), Width(width), Height(height)
        {}
    };

    //窗体的接口，用于对多个API做抽象层
    class API_ Window {
    public:
        using EventCallback = std::function<void(Event&)>;

        virtual ~Window() {}
        //更新窗体
        virtual void Onupdate() = 0;
        //获取宽高
        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;
        //事件回调函数
        virtual void SetEventCallback(const EventCallback& callback) = 0;
        //垂直同步
        virtual void SetVSync(bool enable) = 0;
        virtual bool IsVSync() const = 0;
        //清理缓冲
        virtual void CleanBuffer(int cleanState) = 0;

        //更新窗体绘制大小
        virtual void ViewPortResize(int width, int height) = 0;

        //锁定指针在窗口中
        virtual bool CursorLockInWindow() = 0;
        //取消锁定
        virtual bool CursorUnLockInWindow() = 0;

        //获取对应API的窗口
        virtual void* GetNativeWindow() const = 0;
        //鼠标居中
        virtual void SetCursorOnMid() const = 0;
        //唯一指定创建窗口的类内静态函数
        static Window* Create(const WindowProps& props = WindowProps());


    };
}