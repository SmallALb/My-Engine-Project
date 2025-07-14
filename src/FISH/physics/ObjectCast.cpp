#include "fspcs.h"
#include "FISH/Log.h"
#include "FISH/Renderer/Buffer.h"
#include "FISH/Renderer/VertexArray.h"
#include "FISH/Renderer/RenderElement.h"
#include "FISH/Renderer/Shader.h"
#include "ObjectCast.h"

namespace FISH {
    
    float AABB::volume() const {
        auto size = Max - Min;
        return size.x* size.y *size.z;
    }


    void AABB::transform(const glm::mat4 &transform) {
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

        // Create vertex buffer
        std::shared_ptr<VertexBuffer> vbo;
        vbo.reset(VertexBuffer::Create(vertexData.data(), 4 * vertexData.size()));
        
        // Create index buffer
        std::shared_ptr<IndexBuffer> ibo;
        ibo.reset(IndexBuffer::Create(indices.data(), (indices.size())));

        // Set vertex layout
        vbo->SetLayout({
            {ShaderDataType::Float3, VertexType::Position, "pos"}
        });

        // Create vertex array
        std::shared_ptr<VertexArray> vao;
        vao.reset(VertexArray::Create());
        vao->AddVertexBuffer(vbo);
        vao->SetIndexBuffer(ibo);

        return vao;
    }

    bool Collider::intersects(const ColliderPtr &A, const ColliderPtr &B) {
        auto TyA = A->getType(), TyB = B->getType();

        // AABB AABB
        if (TyA == ColliderType::AABB && TyB == ColliderType::AABB) {
            return intersectsAABBAABB(A, B);
        }
        // AABB OBB 或 OBB AABB
        else if ((TyA == ColliderType::AABB && TyB == ColliderType::OBB) ||
                 (TyA == ColliderType::OBB && TyB == ColliderType::AABB)) {
                return TyA == ColliderType::AABB ? intersectsAABBOBB(A, B) : intersectsAABBOBB(B, A);
        }
        // OBB OBB
        else if (TyA == ColliderType::OBB && TyB == ColliderType::OBB) {
            return intersectsOBBOBB(A, B);
        }
        
        FS_INFO("Unkonwn CastBox Type!");
        return false;
    }


    bool Collider::intersectsAABBAABB(const ColliderPtr &a, const ColliderPtr &b) {
        auto aPos = PtrCastTo<AABB>(a)->getBoundingPos(); 
        auto bPos = PtrCastTo<AABB>(b)->getBoundingPos(); 

        return (aPos.first.x <= bPos.second.x && aPos.second.x >= bPos.first.x) &&
            (aPos.first.y <= bPos.second.y && aPos.second.y >= bPos.first.y) &&
            (aPos.first.z <= bPos.second.z && aPos.second.z >= bPos.first.z);
    }

    bool Collider::intersectsAABBOBB(const ColliderPtr &a, const ColliderPtr &b) {
        auto aabb = PtrCastTo<AABB>(a);
        auto obb = PtrCastTo<OBB>(b);

        glm::vec3 halfSize = (aabb->Max - aabb->Min) * 0.5f;

        glm::vec3 aabbCenter = aabb->getPosition() + (aabb->Min + aabb->Max) * 0.5f;

        auto abb_obb = std::make_shared<OBB>(
            halfSize,
            glm::mat3(1.0f)
        );
        abb_obb->setPosition(aabbCenter);

        return intersectsOBBOBB(obb, abb_obb);
    }

    bool Collider::intersectsOBBOBB(const ColliderPtr &a, const ColliderPtr &b) {
         // 需要检测的分离轴：两个OBB的6个轴向 + 9个叉积轴 = 15条轴
        
         //使用无序set来去重
        std::unordered_set<glm::vec3, decltype([](const glm::vec3& v){
            return std::hash<float>()(v.x) ^ 
                    (std::hash<float>()(v.y) << 1) ^ 
                    (std::hash<float>()(v.z) << 2);
        })> testAxes;

        auto boxA =  PtrCastTo<OBB>(a);
        auto boxB =  PtrCastTo<OBB>(b);


        // 填充boxA的3个轴向
        testAxes.insert(boxA->getAxisX()); // boxA的X轴
        testAxes.insert(boxA->getAxisY()); // boxA的Y轴
        testAxes.insert(boxA->getAxisZ()); // boxA的Z轴

        // 填充boxB的3个轴向
        testAxes.insert(boxB->getAxisX()); // boxB的X轴
        testAxes.insert(boxB->getAxisY()); // boxB的Y轴
        testAxes.insert(boxB->getAxisZ()); // boxB的Z轴

        int index = 6;
        //计算所有的叉积轴体
        for (int i=0; i<3; i++) {
            for (int j = 0; j < 3; j++) 
                testAxes.insert(glm::cross(boxA->getAxis(i), boxB->getAxis(j)));
        }

        //轴分离计算投影半径
        for (const auto& axis : testAxes) {
            //0向量跳过
            if (glm::length(axis) < (std::numeric_limits<float>::min)()) continue;
            
            glm::vec3 normAxis = glm::normalize(axis);

            //A
            float rA = 0.0f;
            rA += boxA->HalfExtents.x * std::abs(glm::dot((boxA->getAxisX()), normAxis));
            rA += boxA->HalfExtents.y * std::abs(glm::dot((boxA->getAxisY()), normAxis));
            rA += boxA->HalfExtents.z * std::abs(glm::dot((boxA->getAxisZ()), normAxis));

            //B;
            float rB = 0.0f;
            rB += boxB->HalfExtents.x * std::abs(glm::dot((boxB->getAxisX()), normAxis));
            rB += boxB->HalfExtents.y * std::abs(glm::dot((boxB->getAxisY()), normAxis));
            rB += boxB->HalfExtents.z * std::abs(glm::dot((boxB->getAxisZ()), normAxis));

            //中心点距离
            float dis = std::abs(glm::dot(boxB->getPosition() - boxA->getPosition(), normAxis));

            if (dis > rA + rB + 0.001f) return false; 
        }

        return true;
    }


    float OBB::volume() const {
        return 8.0f * HalfExtents.x * HalfExtents.y * HalfExtents.z;
    }

    void OBB::transform(const glm::mat4 &transform) {
        mPosition = glm::vec3(transform * glm::vec4(mPosition, 1.0f));
        Rotation = glm::mat3(transform) * Rotation;
    }

    void OBB::setRotation(const glm::mat3 &rotation) {
        Rotation = rotation;
        // 确保正交化
        Rotation[0] = glm::normalize(Rotation[0]);
        Rotation[1] = glm::normalize(Rotation[1]);
        Rotation[2] = glm::normalize(Rotation[2]);
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
}