#pragma once
#include <glm/glm.hpp>

struct Position
{
    glm::vec2 value;
};

struct PreviousPosition
{
    glm::vec2 value;
};

struct Rotation
{
    float value;
};

struct PreviousRotation
{
    float value;
};

struct Mass
{
    float value;
};
struct ConvexCollider
{
    float bounding_radius;
    unsigned int begin;
    unsigned int count;
};

struct CircleCollider
{
    float radius;
};

union Collider {
    ConvexCollider convex_collider;
    CircleCollider circle_collider;
};
