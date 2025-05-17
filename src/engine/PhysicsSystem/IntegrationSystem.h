#pragma once
#include <glm/glm.hpp>

class SystemContext;

class IntegrationSystem
{
  public:
    float gravity = -1;
    void apply(SystemContext* cntx);
    void integrate(SystemContext* cntx, uint32_t id);

    void applyGravity(SystemContext* cntx);
    void updateConvexWorldspaceVerticies(SystemContext* cntx);
};
