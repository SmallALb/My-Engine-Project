#include "fspcs.h"
#include "FISH/Log.h"
#include "FISH/Renderer/Buffer.h"
#include "FISH/Renderer/VertexArray.h"
#include "FISH/Renderer/RenderElement.h"
#include "FISH/Renderer/Texture.h"
#include "FISH/Renderer/BaseShape.h"
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

        auto abb_obb = std::make_shared<OBB>(
            halfSize,
            glm::mat3(1.0f)
        );
        abb_obb->setPosition(aabb->getPosition());

        return intersectsOBBOBB(obb, abb_obb);
    }

    bool Collider::intersectsOBBOBB(const ColliderPtr &a, const ColliderPtr &b) {
         // 需要检测的分离轴：两个OBB的6个轴向 + 9个叉积轴 = 15条轴
        glm::vec3 testAxes[15];

        auto boxA =  PtrCastTo<OBB>(a);
        auto boxB =  PtrCastTo<OBB>(b);


        // 填充boxA的3个轴向
        testAxes[0] = boxA->getAxisX(); // boxA的X轴
        testAxes[1] = boxA->getAxisY(); // boxA的Y轴
        testAxes[2] = boxA->getAxisZ(); // boxA的Z轴

        // 填充boxB的3个轴向
        testAxes[3] = boxB->getAxisX(); // boxB的X轴
        testAxes[4] = boxB->getAxisY(); // boxB的Y轴
        testAxes[5] = boxB->getAxisZ(); // boxB的Z轴

        int index = 6;
        //计算所有的叉积轴体
        for (int i=0; i<3; i++) {
            for (int j = 0; j < 3; j++) 
                testAxes[index++] = glm::cross(boxA->getAxis(i), boxB->getAxis(j));
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

            if (dis > rA + rB + + 0.001f) return false; 
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
                0, 1, 1, 2, 2, 3, 3, 0, // 底面
                4, 5, 5, 6, 6, 7, 7, 4, // 顶面
                0, 4, 1, 5, 2, 6, 3, 7  // 连接底面和顶面
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