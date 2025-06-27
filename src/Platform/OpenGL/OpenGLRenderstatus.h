#pragma once

#include "FISH/Renderer/Renderstatus.h"

namespace FISH {
    class GLstatus : public Renderstatus {
    public:
        GLstatus();
        
        ~GLstatus() {}
        //启用状态
        virtual void enablestatus(StatusType typ) override;
        //禁用状态
        virtual void disablestatus(StatusType typ) override;
        //设置状态
        virtual void setstatusFunc(SetType func, FuncType typ, ...) override;
        //得到清除状态
        virtual int getCleanstatuss() const override;
    private:
        unsigned int  choicestatusTypeToGL(StatusType typ);
        unsigned int  choiceFuncTypeToGL(FuncType typ);
    private:
        int Cleanstatus{0};
    };

}