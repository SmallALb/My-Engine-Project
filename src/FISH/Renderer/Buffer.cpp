#include "fspcs.h"
#include "API.h"
#include "Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
namespace FISH{
        VertexBuffer *VertexBuffer::Create(float *vertices, unsigned int size) {

            switch (GetAPI())
            {
                case RendererAPI::None: return nullptr;
                case RendererAPI::OpenGL: return new GLVertexBuffer(vertices, size);
            }
            return nullptr;
        }
        
        IndexBuffer *IndexBuffer::Create(unsigned int *indices, unsigned int count) {

            switch (GetAPI())
            {
                case RendererAPI::None: return nullptr;
                case RendererAPI::OpenGL: return new GLIndexBuffer(indices, count);
            }
            return nullptr;
        }
}