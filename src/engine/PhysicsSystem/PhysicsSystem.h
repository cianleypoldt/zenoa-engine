#pragma once
#include "BorderSystem.h"
#include "CollisionSystem.h"
#include "IntegrationSystem.h"
#include <vector>

class SystemContext;

class PhysicsSystem
{
  public:
    void step(SystemContext* cntx);

    [[nodiscard]] glm::vec2 getPosition(SystemContext* cntx, unsigned int id);
    [[nodiscard]] float getRotation(SystemContext* cntx, unsigned int id);
    [[nodiscard]] float getCircleRadius(SystemContext* cntx, unsigned int id);
    [[nodiscard]] std::vector<glm::vec2> getConvexShape(SystemContext* cntx, unsigned int id);

    IntegrationSystem m_integration_system;
    BorderSystem m_border_system;
    CollisionSystem m_collision_system;
};
