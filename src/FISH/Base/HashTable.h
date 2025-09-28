#pragma once

#include <glm/glm.hpp>

struct Vec3Hash {
    std::size_t operator()(const glm::vec3& v) const {
        return std::hash<float>{}(v.x) ^ (std::hash<float>{}(v.y) << 1) ^ (std::hash<float>{}(v.z) << 2);
    }
};

struct Vec3Equal {
    bool operator()(const glm::vec3& a, const glm::vec3& b) const {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }
};