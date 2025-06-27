#include "fspcs.h"
#include "glad/glad.h"
#include "OpenGLErrorCheck.h"
#include "OpenGLBuffer.h"

namespace FISH {
    /////////////////////////////////////
        //VERTEX
    /////////////////////////////////////

    GLVertexBuffer::GLVertexBuffer(float *vertices, uint32_t size) {
        GL_ERRORCALL(glCreateBuffers(1, &mVbo));
        GL_ERRORCALL(glBindBuffer(GL_ARRAY_BUFFER, mVbo));
        GL_ERRORCALL(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
    }

    GLVertexBuffer::~GLVertexBuffer() {
        glDeleteBuffers(1, &mVbo);
    }

    void GLVertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    }

    void GLVertexBuffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }

    /////////////////////////////////////
    // INDEX
    /////////////////////////////////////
    GLIndexBuffer::GLIndexBuffer(unsigned int *indices, uint32_t count) : mCount(count) {
        glCreateBuffers(1, &mVbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    GLIndexBuffer::~GLIndexBuffer() {
        glDeleteBuffers(1, &mVbo);
    }


    void GLIndexBuffer::bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVbo);
    }

    void GLIndexBuffer::unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    }

}