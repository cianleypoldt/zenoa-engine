#include "physics.h"

#include "../context.h"
#include "collision.h"

void PhysicsSystem::step(system_context * cntx) {
    integration_system.apply_gravity(cntx);
    IntegrationSystem::apply(cntx);
    border_system.apply(cntx);
    CollisionSystem::apply(cntx);
}
