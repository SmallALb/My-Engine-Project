#include "fspcs.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "OpenGLRenderstatus.h"



namespace FISH {
    GLstatus::GLstatus() {

    }

    void GLstatus::enablestatus(StatusType typ) {
        if (typ == StatusType::CleanDepth ||
            typ == StatusType::CleanColor ||
            typ == StatusType::CleanStencil
        ) 
            return Cleanstatus |= choicestatusTypeToGL(typ), void();
        glEnable(choicestatusTypeToGL(typ));
    }

    void GLstatus::disablestatus(StatusType typ) {
        if (typ == StatusType::CleanDepth || typ == StatusType::CleanColor) 
            return Cleanstatus &= ~(choicestatusTypeToGL(typ)), void();
        glDisable(choicestatusTypeToGL(typ));
    }
    
    void GLstatus::setstatusFunc(SetType func, FuncType typ, ...) {
        switch(func) {
            case SetType::NoneFunc: return;   
            case SetType::CullFaceFunc: {
                glCullFace(choiceFuncTypeToGL(typ));
                return;
            }
            case SetType::FrontFaceDIR: {
                glFrontFace(choiceFuncTypeToGL(typ));
                return;
            }
            case SetType::DepthFunc: {
                glDepthFunc(choiceFuncTypeToGL(typ));
                return;
            }
            case SetType::DepthMask: {
                glDepthMask(choiceFuncTypeToGL(typ));
                return;
            }
            case SetType::BlendFunc: {
                va_list args;
                va_start(args, typ);
                FuncType sfactor = typ;
                FuncType dfactor = va_arg(args, FuncType);
                glBlendFunc(choiceFuncTypeToGL(sfactor), choiceFuncTypeToGL(dfactor));
                va_end(args);
                return;
            }
            case SetType::DepthOffset: {
                va_list args;
                int factor = 0, units = 0;
                va_start(args, typ);
                factor = va_arg(args, int);
                units = va_arg(args, int);
                va_end(args);
                glPolygonOffset(factor, units);
                return;
            }
            case SetType::StencilFunc: {
                va_list args;
                int ref = 0;
                unsigned mask = 0;
                va_start(args, typ);
                ref = va_arg(args, int);
                mask = va_arg(args, unsigned int);
                glStencilFunc(choiceFuncTypeToGL(typ), ref, mask);
                va_end(args);
                return;
            }
            case SetType::StencilOperation: {
                va_list args;
                va_start(args, typ);
                FuncType sfail = typ;
                FuncType zfail = va_arg(args, FuncType);
                FuncType zpass = va_arg(args, FuncType);
                glStencilOp(
                    choiceFuncTypeToGL(sfail),
                    choiceFuncTypeToGL(zfail),
                    choiceFuncTypeToGL(zpass)
                );
                va_end(args);
                return;
            }
            case SetType::StencilMask: {
                va_list args;
                va_start(args, typ);
                unsigned int mask = va_arg(args, unsigned int);
                glStencilMask(mask);
                va_end(args);
                return;
            }
            case SetType::BlendFuncSeparate: {
                va_list args;
                va_start(args, typ);
                FuncType sc = typ;
                FuncType dc = va_arg(args, FuncType);
                FuncType sa = va_arg(args, FuncType);
                FuncType da = va_arg(args, FuncType);
                glBlendFuncSeparate(
                    choiceFuncTypeToGL(sc),
                    choiceFuncTypeToGL(dc),
                    choiceFuncTypeToGL(sa),
                    choiceFuncTypeToGL(da)
                );
                va_end(args);
                return;
            }

            case SetType::BlendEquation: {
                glBlendEquation(choiceFuncTypeToGL(typ));
                return;
            }
        }
    }
    
    int GLstatus::getCleanstatuss() const{
        return Cleanstatus;
    }
    unsigned int  GLstatus::choicestatusTypeToGL(StatusType typ) {
        switch(typ) {
            case StatusType::NonestatusType:    return GL_NONE;
            case StatusType::CleanDepth:        return GL_DEPTH_BUFFER_BIT;
            case StatusType::CleanColor:        return GL_COLOR_BUFFER_BIT;   
            case StatusType::CleanStencil:      return GL_STENCIL_BUFFER_BIT;
            case StatusType::DepthTest:         return GL_DEPTH_TEST;    
            case StatusType::CullFace:          return GL_CULL_FACE;
            case StatusType::Blend:             return GL_BLEND;    
            case StatusType::DepthOffsetFill:   return GL_POLYGON_OFFSET_FILL;
            case StatusType::DepthOffsetLine:   return GL_POLYGON_OFFSET_LINE; 
            case StatusType::StencilTest:       return GL_STENCIL_TEST;
        }
        return GL_NONE;
    }

    unsigned int  GLstatus::choiceFuncTypeToGL(FuncType typ){
        switch(typ) {
            case FuncType::ZERO:            return GL_ZERO;
            case FuncType::ONE:             return GL_ONE;
            case FuncType::Always:          return GL_ALWAYS;
            case FuncType::Never:           return GL_NEVER;
            case FuncType::Equal:           return GL_EQUAL;
            case FuncType::NotEqual:        return GL_NOTEQUAL;
            case FuncType::Less:            return GL_LESS;
            case FuncType::Greater:         return GL_GREATER;
            case FuncType::FALSETyp:        return GL_FALSE;
            case FuncType::Keep:            return GL_KEEP;
            case FuncType::Replace:         return GL_REPLACE;
            case FuncType::Incr:            return GL_INCR;
            case FuncType::Incr_wrap:       return GL_INCR_WRAP;
            case FuncType::Decr:            return GL_DECR;
            case FuncType::Decr_wrap:       return GL_DECR_WRAP;
            case FuncType::Invert:          return GL_INVERT;
            case FuncType::TRUETyp:         return GL_TRUE;
            case FuncType::Front:           return GL_FRONT;
            case FuncType::Back:            return GL_BACK;
            case FuncType::FaceCW:          return GL_CW;
            case FuncType::FaceCCW:         return GL_CCW;
            case FuncType::DepthLess:       return GL_LESS;
            case FuncType::DepthGreater:    return GL_GREATER;
            case FuncType::SrcColor:        return GL_SRC_COLOR;
            case FuncType::DstColor:        return GL_DST_COLOR;
            case FuncType::SrcAlpha:        return GL_SRC_ALPHA;
            case FuncType::DstAlpha:        return GL_DST_ALPHA;
            case FuncType::MinusSrcColor:   return GL_ONE_MINUS_SRC_COLOR;
            case FuncType::MinusDstColor:   return GL_ONE_MINUS_DST_COLOR;
            case FuncType::MinusSrcAlpha:   return GL_ONE_MINUS_SRC_ALPHA;
            case FuncType::MinusDstAlpha:   return GL_ONE_MINUS_DST_ALPHA;

        }
        return GL_NONE;
    }
}