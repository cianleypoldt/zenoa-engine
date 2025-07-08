#include "IntegrationSystem.h"
#include "../../utility/Utility.h"
#include "../SystemContext.h"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

void IntegrationSystem::apply(SystemContext* cntx) {
    for (uint32_t i = 0; i < cntx->entity_manager.moveable_entities.count; i++) {
        updateConvexWorldspaceVerticies(cntx);
        integrate(cntx, cntx->entity_manager.moveable_entities.id[i]);
    }
}

void IntegrationSystem::integrate(SystemContext* cntx, uint32_t id) {
    auto& bodies = cntx->entity_manager.bodies;
    bodies.velocity[id] += (bodies.force[id] * bodies.invMass[id]) * cntx->fixed_timestep;
    bodies.position[id] += bodies.velocity[id] * cntx->fixed_timestep;
    bodies.force[id] = {0, 0};

    bodies.angular_velocity[id] += (bodies.torque[id] * bodies.invInertia[id]) * cntx->fixed_timestep;
    bodies.rotation[id] += bodies.angular_velocity[id] * cntx->fixed_timestep;
    bodies.torque[id] = 0;
}

void IntegrationSystem::applyGravity(SystemContext* cntx) {
    for (int index = 0; index < cntx->entity_manager.gravity_entities.count; index++)
        cntx->entity_manager.bodies.velocity[cntx->entity_manager.gravity_entities.id[index]].y +=
            gravity;
}

void IntegrationSystem::updateConvexWorldspaceVerticies(SystemContext* cntx) {
    for (int index = 0; index < cntx->entity_manager.convex_colliders.count; index++) {
        uint32_t this_id = cntx->entity_manager.convex_colliders.id[index];
        for (uint32_t vertex_index = cntx->entity_manager.bodies.collider[this_id].convex.begin;
             vertex_index < cntx->entity_manager.bodies.collider[this_id].convex.end;
             vertex_index++) {
            cntx->vertex_pool_worldspace[vertex_index] =
                glm::rotate(cntx->vertex_pool[vertex_index],
                            cntx->entity_manager.bodies.rotation[this_id]) +
                cntx->entity_manager.bodies.position[this_id];
        }
    }
}
