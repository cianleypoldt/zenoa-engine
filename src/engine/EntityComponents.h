#pragma once
#include <glm/glm.hpp>

struct Body
{
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 force;

    float rotation;
    float angular_velocity;
    float torque;

    float mass;
    float invMass;
    float inertia;
    float invInertia;
    float elasticity;
};

struct ConvexCollider
{
    unsigned int buffer_begin;
    unsigned int buffer_end;
};

struct CircleCollider
{
    float radius;
};

union Collider {
    CircleCollider circle_collider;
    ConvexCollider convex_collider;
};
