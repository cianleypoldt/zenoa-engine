#pragma once
#include <glm/ext/vector_float2.hpp>
#include <cstdint>

class system_context;

class BorderSystem
{
  public:
    float     border_elasticity  = 1;
    float     border_friction    = 1;
    glm::vec2 bottom_left_corner = { -250, 500 };
    glm::vec2 top_right_corner   = { 250, 500 };

    void apply(system_context * cntx) const;

    void sphere_border_collision(system_context * cntx, uint32_t id) const;

    void convex_border_collision(system_context * cntx, uint32_t id) const;
};
