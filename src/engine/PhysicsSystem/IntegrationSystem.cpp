#include "IntegrationSystem.h"
#include "../../utility/Utility.h"
#include "../SystemContext.h"

void IntegrationSystem::apply(SystemContext* cntx)
{
    for (uint32_t i = 0; i < cntx->entity_manager.gravity_entities.count; i++)
    {

        integrate(cntx, cntx->entity_manager.gravity_entities.id[i]);
    }
}

void IntegrationSystem::integrate(SystemContext* cntx, uint32_t id)
{
    auto& bodies = cntx->entity_manager.bodies;
    bodies.velocity[id] += (bodies.force[id] / bodies.mass[id]) * cntx->fixed_timestep;
    bodies.position[id] += bodies.velocity[id] * cntx->fixed_timestep;
    bodies.force[id] = {0, 0};
}

void IntegrationSystem::applyGravity(SystemContext* cntx)
{
    for (int id = 0; id < cntx->entity_manager.gravity_entities.count; id++)
        cntx->entity_manager.bodies.velocity[id].y += gravity;
}
