#include "fspcs.h"
#include "ObjectCast.h"

namespace FISH {
    bool AABB::contains(const glm::vec3 &p) const{
         return (p.x >= Min.x && p.x <= Max.x &&
                p.y >= Min.y && p.y <= Max.y &&
                p.z >= Min.z && p.z <= Max.z);
    }
    bool AABB::intersects(const AABB &other) const {
        return (Min.x <= other.Max.x && Max.x >= other.Min.x &&
                Min.y <= other.Max.y && Max.y >= other.Min.y &&
                Min.z <= other.Max.z && Max.z >= other.Min.z);
    }
    float AABB::volume() const {
        auto size = Max - Min;
        return size.x* size.y *size.z;
    }
    AABB AABB::merge(const AABB &A, const AABB &B) {
        return AABB(
            (glm::min)(A.Min, B.Min),
            (glm::max)(A.Max, B.Max)
        );
    }
}