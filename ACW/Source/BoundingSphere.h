#pragma once
#include "glm/glm.hpp"

class BoundingSphere {
public:
    glm::vec3 m_Center;
    float m_Radius;

    BoundingSphere();
    BoundingSphere(glm::vec3, float);
    BoundingSphere(const BoundingSphere&);
    BoundingSphere& operator=(const BoundingSphere&);
    ~BoundingSphere();

    BoundingSphere merge(const BoundingSphere& other) {
        glm::vec3 direction = m_Center - other.m_Center;
        float distance = glm::length(direction);
        if (distance + other.m_Radius <= m_Radius)
            return *this;
        if (distance + m_Radius <= other.m_Radius)
            return other;
        float newRadius = (distance + m_Radius + other.m_Radius) / 2.0f;
        glm::vec3 newCenter = m_Center + (direction * ((newRadius - m_Radius) / distance));
        return BoundingSphere(newCenter, newRadius);
    };
    bool intersects(const BoundingSphere& other) {
        float distance = glm::length(m_Center - other.m_Center);
        return distance <= m_Radius + other.m_Radius;
    };
};