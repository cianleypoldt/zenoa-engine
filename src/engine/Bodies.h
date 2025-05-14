#pragma once
#include <glm/glm.hpp>

struct Circle
{
    float radius = 0;
};
struct Convex
{
    uint32_t begin;
    uint32_t end;
};

union Collider {
    Circle circle{0};
    Convex convex;
};

template <uint32_t max_entity_count>
struct Bodies
{
    uint16_t flag[max_entity_count];

    glm::vec2 position[max_entity_count];
    glm::vec2 velocity[max_entity_count];
    glm::vec2 force[max_entity_count];

    float rotation[max_entity_count];
    float angular_velocity[max_entity_count];
    float torque[max_entity_count];

    float mass[max_entity_count];
    float invMass[max_entity_count];
    float inertia[max_entity_count];
    float invInertia[max_entity_count];
    float elasticity[max_entity_count];

    Collider collider[max_entity_count];
};
