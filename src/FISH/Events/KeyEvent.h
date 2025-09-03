#pragma once
#include "FISH/Keycodes.h"
#include "Events.h"

namespace FISH {
    //按键事件
    class  KeyEvent: public Event {
    public:
        inline KeyCode GetKeyCode() const {return mKeyCode;}

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    protected:
        KeyEvent(KeyCode keyCode): mKeyCode(keyCode) {}

        KeyCode mKeyCode;
    };
    //按键按下
    class  KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(KeyCode Keycode, int repeatCount): KeyEvent(Keycode), mRepeatCount(repeatCount) {}  
        
        inline int GetRepeatCount() const {return mRepeatCount;}

        string ToString() const override {
            return "KeyPressedEvent: " + std::string(KeyCodeToString(mKeyCode)) + "(" + std::to_string(mRepeatCount) + " repeats)";
        }  

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int mRepeatCount;
    };
    //按键释放
    class  KeyReleasedEvent : public KeyEvent {
        public:
            KeyReleasedEvent(KeyCode KeyCode): KeyEvent(KeyCode) {}  

            string ToString() const override {
                return "KeyReleasedEvent: " + std::string(KeyCodeToString(mKeyCode));
            }  
    
            EVENT_CLASS_TYPE(KeyReleased)
    };

    class  KeyTypedEvent : public KeyEvent {
        public:
            KeyTypedEvent(KeyCode KeyCode): KeyEvent(KeyCode) {}  
            
    
            string ToString() const override {
                return "KeyTypedEvent: " + std::string(KeyCodeToString(mKeyCode)) ;
            }  
    
            EVENT_CLASS_TYPE(KeyTyped)
    };

}