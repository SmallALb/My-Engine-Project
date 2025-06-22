#pragma once

#include "Events.h"

namespace FISH {
    //����ƶ��¼�
    class MouseMovedEvent : public Event {
    public:
        MouseMovedEvent(const float x, const float y ): mX(x), mY(y)  {}
        
        float GetX() const { return mX; }
        float GetY() const { return mY; }

        string ToString() const override {
            return "MouseMovedEvent: " + std::to_string(mX) + ", " + std::to_string(mY);
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float mX, mY;
    };

    //�����ִ����¼�
    class MouseScrolledEvent : public Event {
        public:
            MouseScrolledEvent(const float xoffset, const float yoffset ): mXoffset(xoffset), mYoffset(yoffset)  {}
            
            float GetX() const { return mXoffset; }
            float GetY() const { return mYoffset; }
    
            string ToString() const override {
                return "MouseScrolledEvent: " + std::to_string(mXoffset) + ", " + std::to_string(mYoffset);
            }
    
            EVENT_CLASS_TYPE(MouseScrolled)
            EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
        private:
            float mXoffset, mYoffset;
    };

    //��갴���¼�
    class MouseButtonEvent : public Event {
    public:
        int GetMouseBUtton() const { return mButton;}

        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton)
    protected:
        MouseButtonEvent(const int button) : mButton(button) {}

        int mButton;
    };

    //��갴���¼�
    class MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        MouseButtonPressedEvent(const int button): MouseButtonEvent(button) {}

        string ToString() const override {
            return "MouseButtonPressedEvent: " + std::to_string(mButton);
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };
    //����ͷ��¼�
    class MouseButtonReleasedEvent : public MouseButtonEvent {
        public:
            MouseButtonReleasedEvent(const int button): MouseButtonEvent(button) {}
    
            string ToString() const override {
                return "MouseButtonReleasedEvent: " + std::to_string(mButton);
            }
    
            EVENT_CLASS_TYPE(MouseButtonReleased)
    };

}