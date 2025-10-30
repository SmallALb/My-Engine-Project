#pragma once



namespace FISH {
     //碰撞盒体
    class AABB : public ICollider{
    public:
        friend class ColliderTest;

        AABB(): Min(0), Max(0) {}

        AABB(const glm::vec3& _min, const glm::vec3& _max): Min(_min), Max(_max) {}
        
        //计算包围体积
        float volume() const override;

        inline ColliderType getType() const {return ColliderType::AABB;}

        inline glm::vec3 size() const  override {return Max - Min;}

        inline glm::vec3 getMin() const {return Min;}

        inline glm::vec3 getMax() const {return Max;}


        void transform(const glm::mat4 &transform) override;

        std::shared_ptr<VertexArray> getVertices() const override;

        //得到带位置的盒体
        std::pair<glm::vec3, glm::vec3> getBoundingPos() const {return {mPosition + Min, mPosition + Max};}

        //设置盒体min值和max值
        inline void setBounding(const glm::vec3& mi, const glm::vec3& ma) {
            Min = mi;
            Max = ma;
        } 

    private:
        //包围盒
        glm::vec3 Min;
        glm::vec3 Max;
    };
}