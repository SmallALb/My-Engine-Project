#pragma once

namespace FISH {

    //可旋转盒体
    class OBB : public ICollider {
    public:
        friend class ColliderTest;


        OBB() {}

        OBB(const glm::vec3& extents, const glm::mat3& rotation) 
            : HalfExtents(extents), Rotation(rotation) {}

        ColliderType getType() const override { return ColliderType::OBB; }

        float volume() const override;

        glm::vec3 size() const override { return HalfExtents * 2.0f; }

        void transform(const glm::mat4& transform) override;

        void setRotation(const glm::mat3& rotation);

        const glm::mat3& getRotation() const {return Rotation;}

        const glm::vec3& getHalfExtens() const {return HalfExtents;}

        //轴向量获取
        inline glm::vec3 getAxisX() const { return glm::normalize(Rotation[0]); }
        inline glm::vec3 getAxisY() const { return glm::normalize(Rotation[1]); }
        inline glm::vec3 getAxisZ() const { return glm::normalize(Rotation[2]); }
        inline glm::vec3 getAxis(int idx) const {return glm::normalize(Rotation[idx]);}
        //8个顶点获取
        std::shared_ptr<VertexArray> getVertices() const override;


    private:
        // 半长宽高
        glm::vec3 HalfExtents{1.0}; 
        // 旋转矩阵
        glm::mat3 Rotation{1.0};    
    };
}