#pragma once

#include "FISH/Renderer/RenderState.h"

namespace FISH {
    class GLState : public RenderState {
    public:
        GLState();
        
        ~GLState() {}
            //////////////////
            ////启用渲染状态///
        ///////////////////////////////////////
        //启用深度测试
        virtual void enableDepthTest() override;
        //启用深度清除
        virtual void enableCleanDepth() override;
        //启动颜色清除
        virtual void enableCleanColor() override;
        //启用面剔除
        virtual void enableCutFace() override;
        ///////////////////////////////////////
            //////////////////
            ////取消渲染状态///
        ///////////////////////////////////////////
        //禁用深度测试
        virtual void disableDepthTest() override;
        //禁用面剔除
        virtual void disableCutFace() override;
        //禁用深度清除
        virtual void disableCleanDepth() override;
        //禁用颜色清除
        virtual void disableCleanColor() override;
        ///////////////////////////////////////////
            //////////////////
            ////设置渲染状态///
        /////////////////////////////////////////
        //设置切除面
        //设置剔除前面
        virtual void setCutFrontFace() override;
        //设置剔除背面
        virtual void setCutBackFace() override;
        //设置正面为顺时针
        virtual void setFrontFaceToCW() override;
        //设置正面为逆时针
        virtual void setFrontFaceToCCW() override;
        //设置深度测试为优先最小
        virtual void setDepthTestFuncLess() override;
        //设置深度测试为优先最大
        virtual void setDepthTestFuncGreater() override;
        ////////////////////////////////////////
            //////////////////
            ////取得渲染状态///
        ///////////////////////////////////////
        //得到清除状态
        virtual int getCleanStates() const override;
        //////////////////////////////////////
    private:
        int CleanState{0};
    };

}