#pragma once

#include "FISH/Core.h"

namespace FISH {
    
    //碰撞盒体
    struct AABB {
        //包围盒
        glm::vec3 Min;
        glm::vec3 Max;

        AABB(): Min(0), Max(0) {}

        AABB(const glm::vec3& _min, const glm::vec3& _max): Min(_min), Max(_max) {}

        //点是否被此盒体包含
        bool contains(const glm::vec3& p) const;

        //两盒体是否相交
        bool intersects(const AABB& other) const;
        
        //计算包围体积
        float volume() const;
        
        static AABB merge(const AABB& A, const AABB& B);

        //重载用于判断点是否包含
        inline bool operator<<(const glm::vec3& p) const {
            return contains(p);
        }

        inline glm::vec3 size() const {return Max - Min;}

        //重载用于判断盒体是否相交
        inline bool operator&(const AABB& other) const {
            return intersects(other);
        }

    };

    //合并运算符重载
    inline AABB operator+(const AABB& A, const AABB& B) {
        return AABB::merge(A, B);
    }


}