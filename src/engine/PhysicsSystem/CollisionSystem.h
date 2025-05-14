#include <glm/glm.hpp>
#pragma once

class SystemContext;

class CollisionSystem
{
  public:
    void apply(SystemContext* cntx);
    void circleCircleCollision(SystemContext* cntx, uint32_t id_a, uint32_t id_b);
    void circleConvexCollision(SystemContext* cntx, uint32_t id_a, uint32_t id_b);
    void convexConvexCollision(SystemContext* cntx, uint32_t id_a, uint32_t id_b);
};
