#include "fspcs.h"
#include "Collider.h"
#include "Cylinder.h"

namespace FISH {
    float Cylinder::volume() const {
        return glm::pi<float>() * Radius * Radius * Height;
    }

    glm::vec3 Cylinder::size() const {
        return glm::vec3(Radius * 2.0f, Height, Radius * 2.0f);
    }

    void Cylinder::transform(const glm::mat4 &transform) {
        mPosition = glm::vec3(transform * glm::vec4(mPosition, 1.0f));
        
        // 更新旋转矩阵
        Rotation = glm::mat3(transform) * Rotation;
        
        // 提取缩放因子
        glm::vec3 scale = glm::vec3(
            glm::length(glm::vec3(transform[0])),
            glm::length(glm::vec3(transform[1])),
            glm::length(glm::vec3(transform[2]))
        );
        
        // 半径受XZ平面缩放影响，高度受Y轴缩放影响
        float radiusScale = (scale.x + scale.z) * 0.5f;
        Radius *= radiusScale;
        Height *= scale.y;
    }
    std::shared_ptr<VertexArray> Cylinder::getVertices() const {
                const int segments = 32;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        float halfHeight = Height * 0.5f;
        glm::vec3 upDir = getUpDirection();
        glm::vec3 topCenter = getTopCenter();
        glm::vec3 bottomCenter = getBottomCenter();

        // 计算切向和副切向
        glm::vec3 tangent, bitangent;
        if (std::abs(upDir.x) > std::abs(upDir.z)) {
            tangent = glm::normalize(glm::cross(upDir, glm::vec3(0.0f, 0.0f, 1.0f)));
        } else {
            tangent = glm::normalize(glm::cross(upDir, glm::vec3(1.0f, 0.0f, 0.0f)));
        }
        bitangent = glm::normalize(glm::cross(upDir, tangent));

        // 生成侧面顶点
        for (int i = 0; i <= segments; ++i) {
            float theta = 2.0f * glm::pi<float>() * i / segments;
            float cosTheta = cos(theta);
            float sinTheta = sin(theta);
            
            // 计算圆环上的点
            glm::vec3 circlePoint = (tangent * cosTheta + bitangent * sinTheta) * Radius;
            
            // 底部顶点
            glm::vec3 bottomVertex = bottomCenter + circlePoint;
            vertices.push_back(bottomVertex.x);
            vertices.push_back(bottomVertex.y);
            vertices.push_back(bottomVertex.z);

            // 顶部顶点
            glm::vec3 topVertex = topCenter + circlePoint;
            vertices.push_back(topVertex.x);
            vertices.push_back(topVertex.y);
            vertices.push_back(topVertex.z);
        }

        // 生成侧面索引
        for (int i = 0; i < segments; ++i) {
            int base = i * 2;
            indices.push_back(base);
            indices.push_back(base + 1);
            indices.push_back(base + 2);

            indices.push_back(base + 1);
            indices.push_back(base + 3);
            indices.push_back(base + 2);
        }

        // 生成顶部和底部圆盘顶点
        int centerTopIndex = vertices.size() / 3;
        vertices.push_back(topCenter.x);
        vertices.push_back(topCenter.y);
        vertices.push_back(topCenter.z);

        for (int i = 0; i <= segments; ++i) {
            float theta = 2.0f * glm::pi<float>() * i / segments;
            float cosTheta = cos(theta);
            float sinTheta = sin(theta);
            glm::vec3 circlePoint = (tangent * cosTheta + bitangent * sinTheta) * Radius;
            // 顶部圆盘外围顶点
            glm::vec3 topDiskVertex = topCenter + circlePoint;
            vertices.push_back(topDiskVertex.x);
            vertices.push_back(topDiskVertex.y);
            vertices.push_back(topDiskVertex.z);
        }

        
        int centerBottomIndex = vertices.size() / 3;
        vertices.push_back(bottomCenter.x);
        vertices.push_back(bottomCenter.y);
        vertices.push_back(bottomCenter.z);

        for (int i = 0; i <= segments; ++i) {
            float theta = 2.0f * glm::pi<float>() * i / segments;
            float cosTheta = cos(theta);
            float sinTheta = sin(theta);
            glm::vec3 circlePoint = (tangent * cosTheta + bitangent * sinTheta) * Radius;
            // 底部圆盘外围顶点
            glm::vec3 bottomDiskVertex = bottomCenter + circlePoint;
            vertices.push_back(bottomDiskVertex.x);
            vertices.push_back(bottomDiskVertex.y);
            vertices.push_back(bottomDiskVertex.z);
        }


        // 生成顶部和底部圆盘索引
        int bottomStart = centerBottomIndex + 2;
        int topStart = bottomStart + segments + 1;
        
        for (int i = 0; i < segments; ++i) {

            // 顶部圆盘
            indices.push_back(centerTopIndex);
            indices.push_back(topStart + i);
            indices.push_back(topStart + i + 1);
        }

        for (int i = 0; i < segments; ++i) {
            // 底部圆盘
            indices.push_back(centerBottomIndex);
            indices.push_back(bottomStart + i + 1);
            indices.push_back(bottomStart + i);
        }

        // 创建顶点缓冲区和索引缓冲区
        std::shared_ptr<VertexBuffer> vbo;
        vbo.reset(VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float)));
        
        std::shared_ptr<IndexBuffer> ibo;
        ibo.reset(IndexBuffer::Create(indices.data(), indices.size()));
        
        vbo->SetLayout({
            {ShaderDataType::Float3, VertexType::Position, "pos"}
        });
        
        std::shared_ptr<VertexArray> vao;
        vao.reset(VertexArray::Create());
        vao->AddVertexBuffer(vbo);
        vao->SetIndexBuffer(ibo);
        
        return vao;
    }
    glm::vec3 Cylinder::getTopCenter() const {
        return mPosition + getUpDirection() * (Height * 0.5f);
    }
    glm::vec3 Cylinder::getBottomCenter() const {
        return mPosition - getUpDirection() * (Height * 0.5f);
    }
}