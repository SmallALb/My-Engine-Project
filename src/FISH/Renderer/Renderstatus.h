#pragma once

#include "FISH/Core.h"

namespace FISH {
    //渲染状态类型
    enum class StatusType {
        NonestatusType,  //null
        DepthTest,      //深度检测
        CleanDepth,     //清理深度
        CleanColor,     //清理颜色
        CullFace,       //面剔除
        Blend           //渲染
    };

    //渲染函数类型
    enum class SetType {
        NoneFunc,           //null
        CullFaceFunc,       //设置剔除面
        FrontFaceDIR,       //设置正面面方向
        DepthFunc,          //设置深度
        BlendFunc           //设置颜色混合
    };

    //渲染函数设置类型
    enum class FuncType {
        ZERO,           //Null
        ONE,            //one
        DepthLess,      //深度优先最小检测
        DepthGreater,   //深度优先最大检测
        Front,          //正面
        Back,           //背面
        FaceCW,         //顺时针方向的面为正面
        FaceCCW,        //逆时针方向的面为正面
        SrcColor,       //源颜色
        DstColor,       //目标颜色
        SrcAlpha,       //源Alpha
        DstAlpha,       //目标Alpha
        MinusSrcColor,  //源色反相
        MinusDstColor,  //目标色反相
        MinusSrcAlpha,  //源透明度反相
        MinusDstAlpha,  //目标透明度反向
    }; 

    //渲染状态接口
    class API_ Renderstatus {
    public:
        //启用状态
        virtual void enablestatus(StatusType typ) = 0;
        //禁用状态
        virtual void disablestatus(StatusType typ) = 0;
        //设置状态
        virtual void setstatusFunc(SetType func, FuncType typ, ...) = 0;
        //得到清除状态
        virtual int getCleanstatuss() const = 0;

        static Renderstatus* CreateRenderstatus();
    };
}