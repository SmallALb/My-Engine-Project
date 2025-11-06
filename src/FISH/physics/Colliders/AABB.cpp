#include "fspcs.h"
#include "FISH/Renderer/Buffer.h"
#include "Collider.h"
#include "AABB.h"

namespace FISH {
    void AABB::transform(const glm::mat4 &transform) {
        //获取aabb盒体的八个顶点
         glm::vec3 corners[8] = {
            {Min.x, Min.y, Min.z},
            {Min.x, Min.y, Max.z},
            {Min.x, Max.y, Min.z},
            {Min.x, Max.y, Max.z},
            {Max.x, Min.y, Min.z},
            {Max.x, Min.y, Max.z},
            {Max.x, Max.y, Min.z},
            {Max.x, Max.y, Max.z}
        };
        
        // 变换所有顶点
        for (auto& corner : corners) {
            corner = glm::vec3(transform * glm::vec4(corner + mPosition, 1.0f));
        }
        
        // 计算新的AABB
        glm::vec3 newMin = corners[0];
        glm::vec3 newMax = corners[0];
        for (const auto& corner : corners) {
            newMin = (glm::min)(newMin, corner);
            newMax = (glm::max)(newMax, corner);
        }
        
        Min = newMin - mPosition;
        Max = newMax - mPosition;
    }

    std::shared_ptr<VertexArray> AABB::getVertices() const {
        glm::vec3 vertices[8] = {
            {Min.x, Min.y, Min.z}, // 0: min corner
            {Min.x, Min.y, Max.z}, // 1: min x,y; max z
            {Min.x, Max.y, Min.z}, // 2: min x,z; max y
            {Min.x, Max.y, Max.z}, // 3: min x; max y,z
            {Max.x, Min.y, Min.z}, // 4: max x; min y,z
            {Max.x, Min.y, Max.z}, // 5: max x,z; min y
            {Max.x, Max.y, Min.z}, // 6: max x,y; min z
            {Max.x, Max.y, Max.z}  // 7: max corner
        };

        for (auto& v : vertices) v += mPosition;

        std::vector<float> vertexData;
        vertexData.reserve(8 * 3);
        for (const auto& v : vertices) {
            vertexData.push_back(v.x);
            vertexData.push_back(v.y);
            vertexData.push_back(v.z);
        }

        std::vector<unsigned int> indices = {
                // 底面
                0, 1, 2, 2, 1, 3,
                // 顶面
                4, 6, 5, 5, 6, 7,
                // 前面
                0, 4, 1, 1, 4, 5,
                // 后面
                2, 3, 6, 6, 3, 7,
                // 左面
                0, 2, 4, 4, 2, 6,
                // 右面
                1, 5, 3, 3, 5, 7
        };

        //创建
        std::shared_ptr<VertexBuffer> vbo;
        vbo.reset(VertexBuffer::Create(vertexData.data(), 4 * vertexData.size()));
        
        //创建下标信息
        std::shared_ptr<IndexBuffer> ibo;
        ibo.reset(IndexBuffer::Create(indices.data(), (indices.size())));

        //创建顶点位置信息
        vbo->SetLayout({
            {ShaderDataType::Float3, VertexType::Position, "pos"}
        });

        //创建顶点数组信息
        std::shared_ptr<VertexArray> vao;
        vao.reset(VertexArray::Create());
        vao->AddVertexBuffer(vbo);
        vao->SetIndexBuffer(ibo);

        return vao;
    }
}