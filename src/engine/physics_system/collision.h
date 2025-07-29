#include <cstdint>
#pragma once

class system_context;

class CollisionSystem
{
  public:
    static void apply(system_context * cntx);

    static void circle_circle_collision(system_context * cntx, uint32_t id_a, uint32_t id_b);
    static void circle_convex_collision(system_context * cntx, uint32_t id_circle, uint32_t id_convex);
    static void convex_convex_collision(system_context * cntx, uint32_t id_a, uint32_t id_b);
};
