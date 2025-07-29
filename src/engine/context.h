#pragma once
#include "entity_manager.h"
#include "physics_system/physics.h"

#include <vector>
#include <glm/ext/vector_float2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

struct system_context {
    PhysicsSystem          physics_system;
    EntityManager          entity_manager;
    std::vector<glm::vec2> vertex_pool;
    std::vector<glm::vec2> vertex_pool_worldspace;
    sf::RenderWindow       window;
    bool                   bounded        = false;
    float                  fixed_timestep = 100;
    unsigned int           iterations     = 0;
};
