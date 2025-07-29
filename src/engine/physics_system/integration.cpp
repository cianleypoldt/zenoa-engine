#include "integration.h"

#include "../context.h"

#include <cmath>
#include <cmath>
#include <cstdint>
#include <glm/ext/vector_float2.hpp>
#define GlmEnableExperimental

void IntegrationSystem::apply(system_context * system_cntx) {
    for (uint32_t i = 0; i < system_cntx->entity_manager.moveable_entities.count; i++) {
        update_convex_worldspace_verticies(system_cntx);
        integrate(system_cntx, system_cntx->entity_manager.moveable_entities.id[i]);
    }
}

void IntegrationSystem::integrate(system_context * system_cntx, uint32_t id) {
    auto & bodies = system_cntx->entity_manager.bodies;
    bodies.velocity[id] += (bodies.force[id] * bodies.inv_mass[id]) * system_cntx->fixed_timestep;
    bodies.position[id] += bodies.velocity[id] * system_cntx->fixed_timestep;
    bodies.force[id] = { 0, 0 };

    bodies.angular_velocity[id] += (bodies.torque[id] * bodies.inv_inertia[id]) * system_cntx->fixed_timestep;
    bodies.rotation[id] += bodies.angular_velocity[id] * system_cntx->fixed_timestep;
    bodies.torque[id] = 0;
}

void IntegrationSystem::apply_gravity(system_context * system_cntx) const {
    for (int index = 0; index < system_cntx->entity_manager.gravity_entities.count; index++) {
        system_cntx->entity_manager.bodies.velocity[system_cntx->entity_manager.gravity_entities.id[index]].y +=
            gravity;
    }
}

void IntegrationSystem::update_convex_worldspace_verticies(system_context * system_cntx) {
    for (int index = 0; index < system_cntx->entity_manager.convex_colliders.count; index++) {
        uint32_t this_id = system_cntx->entity_manager.convex_colliders.id[index];
        for (uint32_t vertex_index = system_cntx->entity_manager.bodies.collider_shape[this_id].convex_col.begin;
             vertex_index < system_cntx->entity_manager.bodies.collider_shape[this_id].convex_col.end;
             vertex_index++) {
            float     angle          = system_cntx->entity_manager.bodies.rotation[this_id];
            glm::vec2 local_vertex   = system_cntx->vertex_pool[vertex_index];
            glm::vec2 rotated_vertex = glm::vec2(local_vertex.x * std::cos(angle) - local_vertex.y * std::sin(angle),
                                                 local_vertex.x * std::sin(angle) + local_vertex.y * std::cos(angle));
            system_cntx->vertex_pool_worldspace[vertex_index] =
                rotated_vertex + system_cntx->entity_manager.bodies.position[this_id];
        }
    }
}
