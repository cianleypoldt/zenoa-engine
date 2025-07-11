#include "PhysicsSystem.h"
#include "../SystemContext.h"

void PhysicsSystem::step(SystemContext* cntx) {
    m_integration_system.applyGravity(cntx);
    m_integration_system.apply(cntx);
    m_border_system.apply(cntx);
    m_collision_system.apply(cntx);
}
