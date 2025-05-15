#pragma once
#include <glm/glm.hpp>
#include <vector>

struct Circle
{
    float radius = 0;
};
struct Convex
{
    uint32_t begin;
    uint32_t end;
    float bounding_radius;
};

union Collider {
    Circle circle{0};
    Convex convex;
};

struct Bodies
{
    std::vector<uint16_t> flag;

    std::vector<glm::vec2> position;
    std::vector<glm::vec2> velocity;
    std::vector<glm::vec2> force;

    std::vector<float> rotation;
    std::vector<float> angular_velocity;
    std::vector<float> torque;

    std::vector<float> mass;
    std::vector<float> invMass;
    std::vector<float> inertia;
    std::vector<float> invInertia;
    std::vector<float> elasticity;

    std::vector<Collider> collider;

    void resize(size_t size)
    {
        flag.resize(size);
        position.resize(size);
        velocity.resize(size);
        force.resize(size);
        rotation.resize(size);
        angular_velocity.resize(size);
        torque.resize(size);
        mass.resize(size);
        invMass.resize(size);
        inertia.resize(size);
        invInertia.resize(size);
        elasticity.resize(size);
        collider.resize(size);
    }
    uint32_t size()
    {
        return flag.size();
    }
};
