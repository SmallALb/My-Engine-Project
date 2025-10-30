#pragma once

namespace FISH {

    class Cylinder : public ICollider {
    public:
        friend class ColliderTest;

        Cylinder(): Radius(1.0f),  Height(1.0f), Rotation(1.0f) {}

        Cylinder(float r, float h):Radius(r),  Height(h), Rotation(1.0f) {}

        ColliderType getType() const override { return ColliderType::Cylinder; }

        float volume() const override;

        glm::vec3 size() const override;

        void transform(const glm::mat4& transform) override;

        std::shared_ptr<VertexArray> getVertices() const override;

        inline float getRadius() const { return Radius; }
        inline float getHeight() const { return Height; }
        inline const glm::mat3& getRotation() const { return Rotation; }
        
        inline void setRadius(float r) { Radius = r; }
        inline void setHeight(float h) { Height = h; }
        inline void setRotation(const glm::mat3& rotation) { 
            Rotation = rotation;
            // 确保正交化
            Rotation[0] = glm::normalize(Rotation[0]);
            Rotation[1] = glm::normalize(Rotation[1]);
            Rotation[2] = glm::normalize(Rotation[2]);
        }

        // 获取圆柱的轴向
        inline glm::vec3 getAxisX() const { return glm::normalize(Rotation[0]); }
        inline glm::vec3 getAxisY() const { return glm::normalize(Rotation[1]); }
        inline glm::vec3 getAxisZ() const { return glm::normalize(Rotation[2]); }
        inline glm::vec3 getAxis(int idx) const { return glm::normalize(Rotation[idx]); }

        // 获取圆柱的上下表面中心点
        glm::vec3 getTopCenter() const;
        glm::vec3 getBottomCenter() const;

        // 获取圆柱的主轴方向（高度方向）
        inline glm::vec3 getUpDirection() const { return getAxisY(); }
    private:
        float Radius;
        float Height;
        glm::mat3 Rotation{1.0f};
    };
}