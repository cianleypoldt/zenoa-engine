#include "physics.h"

#include "../context.h"

void PhysicsSystem::step(system_context * cntx) {
    integration_system.apply_gravity(cntx);
    integration_system.apply(cntx);
    border_system.apply(cntx);
    CollisionSystem::apply(cntx);
}
