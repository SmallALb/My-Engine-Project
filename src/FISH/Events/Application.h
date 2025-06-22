#pragma once

#include "Events.h"

namespace FISH {
    //窗体大小重构事件
    class  WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height) : mWidth(width), mHeight(height) {}

        inline unsigned int GetWidth() const {return mWidth; } 
        inline unsigned int GetHeight() const {return mHeight; }

        string ToString() const override {
            return "WindowResizeEvent: " + std::to_string(mWidth) + ", " + std::to_string(mHeight);
        }

        EVENT_CLASS_TYPE(WindowResize);
        EVENT_CLASS_CATEGORY(EventCategoryAppliocation);
    protected:
        unsigned int mWidth{0};
        unsigned int mHeight{0};
    };

    //窗体关闭事件
    class  WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryAppliocation)
    };

    //App更新事件
    class  AppUpdateEvent : public Event {
    public:
        AppUpdateEvent() = default;

        EVENT_CLASS_TYPE(AppUpdate)
        EVENT_CLASS_CATEGORY(EventCategoryAppliocation)
    };

    //App渲染事件
    class  AppRenderEvent : public Event {
    public:
        AppRenderEvent() = default;

        EVENT_CLASS_TYPE(AppRender)
        EVENT_CLASS_CATEGORY(EventCategoryAppliocation)
    };

    //App单位事件事件
    class  AppTickEvent : public Event {
    public:
        AppTickEvent() = default;

        EVENT_CLASS_TYPE(AppTick)
        EVENT_CLASS_CATEGORY(EventCategoryAppliocation)
    };
}