#include "fspcs.h"
#include "Collider.h"
#include "Sphere.h"

namespace FISH {
    float Sphere::volume() const {
        return (4.0f / 3.0f) * glm::pi<float>() * Radius * Radius * Radius;
    }
    
    glm::vec3 Sphere::size() const {
        return glm::vec3(Radius * 2.0f);
    }


    void Sphere::transform(const glm::mat4 &transform) {
        mPosition = glm::vec3(transform * glm::vec4(mPosition, 1.0f));
        glm::vec3 scale = glm::vec3(transform[0][0], transform[1][1], transform[2][2]);
        float avgScale = (scale.x + scale.y + scale.z) / 3.0f;
        Radius *= avgScale;
    }
    
    std::shared_ptr<VertexArray> Sphere::getVertices() const {
        const int segments = 16;
        const int rings = 16;
        
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        
        // 生成顶点
        for (int i = 0; i <= rings; ++i) {
            float phi = glm::pi<float>() * i / rings;
            for (int j = 0; j <= segments; ++j) {
                float theta = 2.0f * glm::pi<float>() * j / segments;
                
                float x = Radius * sin(phi) * cos(theta);
                float y = Radius * cos(phi);
                float z = Radius * sin(phi) * sin(theta);
                
                vertices.push_back(mPosition.x + x);
                vertices.push_back(mPosition.y + y);
                vertices.push_back(mPosition.z + z);
            }
        }
        
        // 生成索引
        for (int i = 0; i < rings; ++i) {
            for (int j = 0; j < segments; ++j) {
                int first = (i * (segments + 1)) + j;
                int second = first + segments + 1;
                
                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);
                
                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
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
}