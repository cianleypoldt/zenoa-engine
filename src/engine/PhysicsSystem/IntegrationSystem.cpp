#include "IntegrationSystem.h"
#include "../../graphics/Renderer.h"
#include "../../utility/Utility.h"
#include "../SystemContext.h"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

void IntegrationSystem::apply(SystemContext* cntx)
{
    for (uint32_t i = 0; i < cntx->entity_manager.moveable_entities.count; i++)
    {
        integrate(cntx, cntx->entity_manager.moveable_entities.id[i]);
    }
}

void IntegrationSystem::integrate(SystemContext* cntx, uint32_t id)
{
    auto& bodies = cntx->entity_manager.bodies;
    bodies.velocity[id] += (bodies.force[id] / bodies.mass[id]) * cntx->fixed_timestep;
    bodies.position[id] += bodies.velocity[id] * cntx->fixed_timestep;
    bodies.force[id] = {0, 0};
    bodies.angular_velocity[id] += (bodies.torque[id] / bodies.inertia[id]) * cntx->fixed_timestep;
    bodies.rotation[id] += bodies.angular_velocity[id] * cntx->fixed_timestep;
    bodies.torque[id] = 0;
}

void IntegrationSystem::applyGravity(SystemContext* cntx)
{
    for (int id = 0; id < cntx->entity_manager.gravity_entities.count; id++)
        cntx->entity_manager.bodies.velocity[id].y += gravity;
}
