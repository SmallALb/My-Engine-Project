#pragma once

#include "FISH/Core.h"

namespace FISH {
    //渲染状态接口
    class API_ RenderState {
    public:
            //////////////////
            ////启用渲染状态///
        ///////////////////////////////////////
        //启用深度测试
        virtual void enableDepthTest() = 0;
        //启用深度清除
        virtual void enableCleanDepth() = 0;
        //启动颜色清除
        virtual void enableCleanColor() = 0;
        //启用面剔除
        virtual void enableCutFace() = 0;
        //启用颜色混合
        virtual void enableBlend() = 0;
        ///////////////////////////////////////

            //////////////////
            ////取消渲染状态///
        ///////////////////////////////////////////
        //禁用深度测试
        virtual void disableDepthTest() = 0;
        //禁用深度清除
        virtual void disableCleanDepth() = 0;
        //禁用颜色清除
        virtual void disableCleanColor() = 0;
        //禁用面剔除
        virtual void disableCutFace() = 0;
        //禁用颜色混合
        virtual void disableBlend() = 0;
        ///////////////////////////////////////////

            //////////////////
            ////设置渲染状态///
        /////////////////////////////////////////
        //设置剔除前面
        virtual void setCutFrontFace() = 0;
        //设置剔除背面
        virtual void setCutBackFace() = 0;
        //设置前面为顺时针
        virtual void setFrontFaceToCW() = 0;
        //设置前面为逆时针
        virtual void setFrontFaceToCCW() = 0;
        //设置深度检测为最小优先
        virtual void setDepthTestFuncLess() = 0;
        //设置深度检测为最大优先
        virtual void setDepthTestFuncGreater() = 0;
        //设置颜色混合源模式为与Alpha通道相乘
        virtual void setBlendSrcFuncMulAlpha() = 0;
        //设置颜色混合目标模式减去Alpha
        virtual void setBlendFactorFuncSubAlpha() = 0;
        ////////////////////////////////////////
            //////////////////
            ////取得渲染状态///
        ///////////////////////////////////////
        //得到清除状态
        virtual int getCleanStates() const = 0;
        //////////////////////////////////////

        static RenderState* CreateRenderState();
    };
}