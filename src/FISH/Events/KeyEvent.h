#pragma once

#include "Events.h"

namespace FISH {
    //按键事件
    class  KeyEvent: public Event {
    public:
        inline int GetKeyCode() const {return mKeyCode;}

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    protected:
        KeyEvent(int keyCode): mKeyCode(keyCode) {}

        int mKeyCode;
    };
    //按键按下
    class  KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(int KeyCode, int repeatCount): KeyEvent(KeyCode), mRepeatCount(repeatCount) {}  
        
        inline int GetRepeatCount() const {return mRepeatCount;}

        string ToString() const override {
            return "KeyPressedEvent: " + std::to_string(mKeyCode) + "(" + std::to_string(mRepeatCount) + " repeats)";
        }  

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int mRepeatCount;
    };
    //按键释放
    class  KeyReleasedEvent : public KeyEvent {
        public:
            KeyReleasedEvent(int KeyCode): KeyEvent(KeyCode) {}  

            string ToString() const override {
                return "KeyReleasedEvent: " + std::to_string(mKeyCode);
            }  
    
            EVENT_CLASS_TYPE(KeyReleased)
    };

    class  KeyTypedEvent : public KeyEvent {
        public:
            KeyTypedEvent(int KeyCode): KeyEvent(KeyCode) {}  
            
    
            string ToString() const override {
                return "KeyTypedEvent: " + std::to_string(mKeyCode) ;
            }  
    
            EVENT_CLASS_TYPE(KeyTyped)
    };

}