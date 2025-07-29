#pragma once
#include <cstdint>

class system_context;

class IntegrationSystem
{
  public:
    float gravity = -1;

    static void        apply(system_context * cntx);
    static void integrate(system_context * cntx, uint32_t id);

    void        apply_gravity(system_context * cntx) const;
    static void update_convex_worldspace_verticies(system_context * cntx);
};
