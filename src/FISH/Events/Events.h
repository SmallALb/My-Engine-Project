#pragma once

#include "../Core.h"


namespace FISH {
    //事件类型
    enum class EventType{
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus,
        WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled  
    };
    //事件类别
    enum EvenCategory {
        None = 0,
        EventCategoryAppliocation = BIT(0),
        EventCategoryInput = BIT(1),
        EventCategoryKeyboard = BIT(2),
        EventCategoryMouse = BIT(3),
        EventCategoryMouseButton = BIT(4),
    };

#define EVENT_CLASS_TYPE(typ)   static EventType GetStaticType() {return EventType::typ; }\
                                virtual EventType GetEventType() const override {return GetStaticType(); }\
                                virtual const char* GetName() const override {return #typ; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCatgeoryFlags() const override {return category; }
    //立即响应式事件
    class Event {
        friend class EventDispatcher;
    public:
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCatgeoryFlags() const = 0;
        virtual string ToString() const {return GetName(); }

        inline bool IsInCategory(EvenCategory category) { return GetCatgeoryFlags() & category; }

        inline bool GetHandle() const {return m_handle;}
    protected:
        bool m_handle{0};
    };


    //事件调度器，用于选择事件发生
    class EventDispatcher {
        template<class T>
        using EventFn = std::function<bool(T&)>;
    public:
        template<class T>
        bool Dispatch(EventFn<T> func) {
            if (mEvent.GetEventType() == T::GetStaticType()) {
                mEvent.m_handle |= func(static_cast<T&>(mEvent));
                return true;
            }
            return false;
        }

        EventDispatcher(Event& event): mEvent(event) {}
    private:
        Event& mEvent;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e) {
        return os << e.ToString();
    };
}