#include "fspcs.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "OpenGLRenderState.h"



namespace FISH {
    GLState::GLState() {
    
    }

    void GLState::enableDepthTest() {
        glEnable(GL_DEPTH_TEST);
    }

    void GLState::enableCleanDepth() {
        CleanState |= GL_DEPTH_BUFFER_BIT;
    }

    void GLState::enableCleanColor() {
        CleanState |= GL_COLOR_BUFFER_BIT;
    }

    void GLState::enableCutFace() {
        glEnable(GL_CULL_FACE);
    }

    void GLState::enableBlend() {
        glEnable(GL_BLEND);
    }

    void GLState::disableDepthTest() {
        glDisable(GL_DEPTH_TEST);
    }

    void GLState::disableCleanDepth() {
        CleanState &= ~(uint16_t(GL_DEPTH_BUFFER_BIT));
    }

    void GLState::disableCleanColor() {
        CleanState &= ~(uint16_t(GL_COLOR_BUFFER_BIT));
    }

    void GLState::disableBlend() {
        glDisable(GL_BLEND);
    }

    void GLState::setCutFrontFace() {
        glCullFace(GL_FRONT);
    }

    void GLState::setCutBackFace() {
        glCullFace(GL_BACK);
    }

    void GLState::setFrontFaceToCW() {
        glFrontFace(GL_CW);
    }

    void GLState::setFrontFaceToCCW() {
        glFrontFace(GL_CCW);
    }

    void GLState::setDepthTestFuncLess() {
        glDepthFunc(GL_LESS);
    }

    void GLState::setDepthTestFuncGreater() {
        glDepthFunc(GL_GREATER);
    }

    void GLState::setBlendSrcFuncMulAlpha() {
        
    }

    void GLState::setBlendFactorFuncSubAlpha() {

    }

    int GLState::getCleanStates() const{
        return CleanState;
    }

    void GLState::disableCutFace() {
        glDisable(GL_CULL_FACE);
    }
}