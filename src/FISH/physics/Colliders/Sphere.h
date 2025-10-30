#pragma once

namespace FISH {

    class Sphere : public ICollider {
    public:
        friend class ColliderTest;

        Sphere(): Radius(1.0) {}

        Sphere(float r): Radius(r) {}

        ColliderType getType() const override {return ColliderType::Sphere;}

        float volume() const override;

        glm::vec3 size() const override;

        void transform(const glm::mat4& transform) override;

        std::shared_ptr<VertexArray> getVertices() const override;

        inline float getRadius() const {return Radius;}
        inline float setRadius(float r) { Radius = r;}

    private:
        //球体半径
        float Radius;
    };
}