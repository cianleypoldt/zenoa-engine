#pragma once
#include <cstddef>
#include <cstdint>
#include <glm/ext/vector_float2.hpp>
#include <vector>

struct circle_shape {
    float radius = 0;
};

struct convex_shape {
    uint32_t begin;
    uint32_t end;
    float    bounding_radius;
};

union collider_union {
    circle_shape circle{ 0 };
    convex_shape convex;
};

struct bodies_container {
    std::vector<uint16_t> flag;

    std::vector<glm::vec2> position;
    std::vector<glm::vec2> velocity;
    std::vector<glm::vec2> force;

    std::vector<float> rotation;
    std::vector<float> angular_velocity;
    std::vector<float> torque;

    std::vector<float> mass;

    std::vector<float> inv_mass;
    std::vector<float> inertia;
    std::vector<float> inv_inertia;
    std::vector<float> elasticity;
    std::vector<float> friction;

    std::vector<collider_union> collider;

    void resize(size_t size) {
        flag.resize(size);
        position.resize(size);
        velocity.resize(size);
        force.resize(size);
        rotation.resize(size);
        angular_velocity.resize(size);
        torque.resize(size);
        mass.resize(size);
        inv_mass.resize(size);
        inertia.resize(size);
        inv_inertia.resize(size);
        elasticity.resize(size);
        friction.resize(size);
        collider.resize(size);
    }

    uint32_t size() const { return flag.size(); }
};
