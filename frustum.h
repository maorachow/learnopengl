#pragma once
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
struct Plane {
    glm::vec3 normal = { 0.f, 1.f, 0.f };

    
    float     distance = 0.0f;
    float getSignedDistanceToPlane(const glm::vec3& point) const
    {
        return glm::dot(normal, point) - distance;
    }
};
struct AABB {
    glm::vec3 center{ 0.f, 0.f, 0.f };
    glm::vec3 extents{ 0.f, 0.f, 0.f };

    AABB(const glm::vec3& min, const glm::vec3& max)
       
       :  center{ (max + min) * 0.5f },
        extents{ max.x - center.x, max.y - center.y, max.z - center.z }
    {}

    AABB(const glm::vec3& inCenter, float iI, float iJ, float iK)
        :  center{ inCenter }, extents{ iI, iJ, iK }
    {}
    bool isOnOrForwardPlane(const Plane& plane) const
    {
        // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
        const float r = extents.x * std::abs(plane.normal.x) +
            extents.y * std::abs(plane.normal.y) + extents.z * std::abs(plane.normal.z);

        return -r <= plane.getSignedDistanceToPlane(center);
    }
   
};