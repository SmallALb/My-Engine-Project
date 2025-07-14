#include "fspcs.h"
#include "FISH/Renderer/Buffer.h"
#include "FISH/Renderer/VertexArray.h"
#include "FISH/Renderer/RenderElement.h"
#include "FISH/Renderer/TextureHandle.h"
#include "FISH/Renderer/BaseShape.h"
#include "Mesh.h"

namespace FISH {

    Mesh::~Mesh()
    {
    }
    std::shared_ptr<Shape>& Mesh::getShape() {return mShape; }
    
    void Mesh::renderMesh(int typ) {
        mShape->useShape();
        mShape->useTexture();
        mShape->render(typ);
        //mShape->unuseShape();
    }
}
