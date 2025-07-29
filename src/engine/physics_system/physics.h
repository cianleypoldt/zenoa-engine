#pragma once
#include "border.h"
#include "collision.h"
#include "integration.h"

#include <glm/ext/vector_float2.hpp>
#include <vector>

class system_context;

class PhysicsSystem
{
  public:
    void step(system_context * cntx);

    [[nodiscard]] glm::vec2              get_position(system_context * cntx, unsigned int id);
    [[nodiscard]] float                  get_rotation(system_context * cntx, unsigned int id);
    [[nodiscard]] float                  get_circle_radius(system_context * cntx, unsigned int id);
    [[nodiscard]] std::vector<glm::vec2> get_convex_shape(system_context * cntx, unsigned int id);

    IntegrationSystem integration_system;
    BorderSystem      border_system;
    CollisionSystem   collision_system;
};
