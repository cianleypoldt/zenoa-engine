#pragma once
#include "EntityManager.h"
#include "PhysicsSystem/PhysicsSystem.h"

struct SystemContext
{
    PhysicsSystem physics_system;
    EntityManager entity_manager;
    std::vector<glm::vec2> vertex_pool;

    bool bounded = false;
    float fixed_timestep = 0.1;
    unsigned int iterations = 0;
};
