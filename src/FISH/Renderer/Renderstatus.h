#pragma once

#include "FISH/Core.h"

namespace FISH {
    //渲染状态类型
    enum class StatusType {
        NonestatusType,     //null
        DepthTest,          //深度检测
        StencilTest,        //模板测试
        CleanDepth,         //清理深度
        CleanColor,         //清理颜色
        CleanStencil,       //清理模板缓冲
        CullFace,           //面剔除
        Blend,              //渲染
        DepthOffsetFill,    //面深度偏移
        DepthOffsetLine     //线深度偏移
    };

    //渲染函数类型
    enum class SetType {
        NoneFunc,           //null
        CullFaceFunc,       //设置剔除面
        FrontFaceDIR,       //设置正面面方向
        DepthFunc,          //设置深度
        BlendFunc,           //设置颜色混合
        DepthMask,           //深度写入
        DepthOffset,          //深度偏移
        StencilFunc,        //模板测试条件
        StencilOperation,   //模板测试操作
        StencilMask,         //模板标记
        BlendFuncSeparate,  //RGB & A 通道分别设置
        BlendEquation       //设置混合颜色的计算方式
    };

    //渲染函数设置类型
    enum class FuncType {
        ZERO,           //Null
        FALSETyp,       //False
        TRUETyp,        //True
        ONE,            //one
        Less,           //小于
        Never,          //从不
        Always,         //总是
        Equal,          //相等
        NotEqual,       //不等
        Greater,        //大于
        Keep,           //保持
        Replace,        //替换
        Incr,           //加一
        Incr_wrap,      //取模增1，大于最大可表示范围则回到0
        Decr,           //减一(大于0的时候才会执行)
        Decr_wrap,      //取模减一，小于0会回到此数的最表示值
        Invert,         //按位取反
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
        DepthOffsetData, //偏移信息传入
        Add,              //加法
        Subtract,        //减法
        ReverseSubtract  //反向相减           
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

    using RernderStatusPtr = std::shared_ptr<Renderstatus>;
    
}