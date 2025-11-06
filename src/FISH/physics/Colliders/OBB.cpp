#include "fspcs.h"
#include "FISH/Renderer/Buffer.h"
#include "Collider.h"
#include "OBB.h"

namespace FISH {
    float OBB::volume() const {
        return 8.0f * HalfExtents.x * HalfExtents.y * HalfExtents.z;
    }


    void OBB::transform(const glm::mat4 &transform) {
        mPosition = glm::vec3(transform * glm::vec4(mPosition, 1.0f));
        Rotation = glm::mat3(transform) * Rotation;
    }

    std::shared_ptr<VertexArray> OBB::getVertices() const {
           glm::vec3 axes[3] = { getAxisX(), getAxisY(), getAxisZ() };
    
            // 计算半长向量（缩放后的轴向）
            glm::vec3 halfExtents[3] = {
                axes[0] * HalfExtents.x,
                axes[1] * HalfExtents.y,
                axes[2] * HalfExtents.z
            };

            // 8 个顶点的相对偏移（组合 +/- 半长）
            glm::vec3 vertices[8] = {
                mPosition - halfExtents[0] - halfExtents[1] - halfExtents[2], // (-X, -Y, -Z)
                mPosition - halfExtents[0] - halfExtents[1] + halfExtents[2], // (-X, -Y, +Z)
                mPosition - halfExtents[0] + halfExtents[1] - halfExtents[2], // (-X, +Y, -Z)
                mPosition - halfExtents[0] + halfExtents[1] + halfExtents[2], // (-X, +Y, +Z)
                mPosition + halfExtents[0] - halfExtents[1] - halfExtents[2], // (+X, -Y, -Z)
                mPosition + halfExtents[0] - halfExtents[1] + halfExtents[2], // (+X, -Y, +Z)
                mPosition + halfExtents[0] + halfExtents[1] - halfExtents[2], // (+X, +Y, -Z)
                mPosition + halfExtents[0] + halfExtents[1] + halfExtents[2]  // (+X, +Y, +Z)
            };

            // 转换为 float 数组（仅位置，无颜色/UV）
            std::vector<float> result;
            result.reserve(8 * 3);
            for (const auto& v : vertices) {
                result.push_back(v.x);
                result.push_back(v.y);
                result.push_back(v.z);
            }
            std::shared_ptr<VertexBuffer> vbo;
            std::shared_ptr<IndexBuffer>  index;
            vbo.reset(VertexBuffer::Create(result.data(), 4 * result.size()));

            std::vector<unsigned int> ix{
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

            index.reset(IndexBuffer::Create(ix.data(), ix.size()));

            vbo->SetLayout({{ShaderDataType::Float3, VertexType::Position, "pos"}});
            std::shared_ptr<VertexArray> vao;
            vao.reset(VertexArray::Create());
            vao->AddVertexBuffer(vbo);
            vao->SetIndexBuffer(index);
            return vao;
    }

    void OBB::setRotation(const glm::mat3 &rotation) {
        Rotation = rotation;
        // 确保正交化
        Rotation[0] = glm::normalize(Rotation[0]);
        Rotation[1] = glm::normalize(Rotation[1]);
        Rotation[2] = glm::normalize(Rotation[2]);
    }
}