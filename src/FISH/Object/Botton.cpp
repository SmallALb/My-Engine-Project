#include "fspcs.h"
#include "Botton.h"


namespace FISH {

    Botton::Botton(const std::string& name) {
        mName = name;
        mType = ObjType::Botton;
        mHoverFunc = []() {};
        mClickFunc = []() {};
        mNoStatusFunc = []() {};
    }


    void Botton::update(float x, float y) {
        if (IsHover(x, y)) {
            
            if (clickTag) {
                mClickFunc();
            }
            else {
                mHoverFunc();
            }
        }
        else {
            mNoStatusFunc();
        }
        clickTag = 0;
    }

    Botton::~Botton() {
    
    }

    void Botton::setClick(bool status) {
        clickTag = status;    
    }

    void Botton::setBoxSize(const glm::vec2 &_size) {
        mBoxSize = _size;
    }

    bool Botton::IsHover(float x, float y)
    {
        float minX = mPosition.x;
        float maxX = mPosition.x + mBoxSize.x;

        float minY = mPosition.y;
        float maxY = mPosition.y + mBoxSize.y;

        return x >= minX && x <= maxX && y >= minY && y <= maxY;
    }
}
