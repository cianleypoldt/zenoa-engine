#pragma once
#include "interface.h"

#include "context.h"
#include "entity_manager.h"

#include <cstdint>
#include <glm/ext/vector_float2.hpp>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <glm/geometric.hpp>
#include <vector>

system_context* rbs::make_context() {
    return new system_context{};
}

void rbs::drop(system_context* cntx) {
    delete cntx;
}

uint32_t rbs::get_max_entity_count() {
    return EntityManager::max_entity_count;
}

void rbs::set_bounds(system_context* cntx, glm::vec2 bottom_left, glm::vec2 top_right) {
    cntx->bounded                                         = true;
    cntx->physics_system.border_system.bottom_left_corner = bottom_left;
    cntx->physics_system.border_system.top_right_corner   = top_right;
}

void rbs::set_gravity(system_context* cntx, float gravity) {
    cntx->physics_system.integration_system.gravity = gravity;
}

void rbs::step(system_context* cntx) {
    cntx->physics_system.step(cntx);
}

[[nodiscard]] unsigned int rbs::add_entity(system_context* cntx, glm::vec2 pos, float mass) {
    uint32_t id                              = cntx->entity_manager.add_entity();
    cntx->entity_manager.bodies.position[id] = pos;
    cntx->entity_manager.bodies.mass[id]     = mass;
    cntx->entity_manager.make_moveable(id);
    cntx->entity_manager.enable_gravity(id);
    return id;
}

void rbs::add_circle_collider(system_context* cntx, unsigned int id, float radius, float density = 0.0005) {
    if (!cntx->entity_manager.verify_id(id)) {
        return;
    }
    cntx->entity_manager.use_circle_collider(id);
    cntx->entity_manager.bodies.collider[id].circle.radius = radius;
    cntx->entity_manager.bodies.mass[id]                   = 3.14 * radius * radius * (density);
    cntx->entity_manager.bodies.inv_mass[id]               = 1 / cntx->entity_manager.bodies.mass[id];
    cntx->entity_manager.bodies.inertia[id]     = 0.5 * cntx->entity_manager.bodies.mass[id] * radius * radius;
    cntx->entity_manager.bodies.inv_inertia[id] = 1 / cntx->entity_manager.bodies.inertia[id];
}

void rbs::add_convex_collider(system_context*        cntx,
                              unsigned int           id,
                              std::vector<glm::vec2> points,
                              float                  density = 0.00000005) {
    if (!cntx->entity_manager.verify_id(id)) {
        return;
    }

    // 1) Compute centroid
    glm::vec2 centroid{ 0.0f };
    for (auto& v : points) {
        centroid += v;
    }
    centroid /= points.size();

    // 2) Translate verts to centroid
    for (auto& v : points) {
        v -= centroid;
    }

    // 3) Store in pool & compute bounding radius
    auto& em = cntx->entity_manager;
    em.use_convex_collider(id);
    auto& cc = em.bodies.collider[id].convex;
    cc.begin = cntx->vertex_pool.size();
    cntx->vertex_pool.insert(cntx->vertex_pool.end(), points.begin(), points.end());
    cntx->vertex_pool_worldspace.insert(cntx->vertex_pool_worldspace.end(), points.begin(), points.end());
    cc.end = cntx->vertex_pool.size();

    float max_r2 = 0.0f;
    for (auto& v : points) {
        max_r2 = std::max(max_r2, glm::dot(v, v));
    }
    cc.bounding_radius = std::sqrt(max_r2);

    // 4) Compute signed area & polygon mass properties
    float a2      = 0.0f;  // twice signed area
    float i_accum = 0.0f;  // accumulator for inertia integral

    for (size_t i = cc.begin; i < cc.end; ++i) {
        const auto& p1 = cntx->vertex_pool[i];
        const auto& p2 = cntx->vertex_pool[(i + 1 < cc.end) ? i + 1 : cc.begin];

        float cross = p1.x * p2.y - p2.x * p1.y;
        a2 += cross;

        // for inertia: (p1·p1 + p1·p2 + p2·p2) * cross
        float term = glm::dot(p1, p1) + glm::dot(p1, p2) + glm::dot(p2, p2);
        i_accum += cross * term;
    }

    float area             = 0.5f * std::abs(a2);
    float mass             = density * area;
    em.bodies.mass[id]     = mass;
    em.bodies.inv_mass[id] = (mass > 0.0f ? 1.0f / mass : 0.0f);

    // Polygon moment of inertia about centroid:
    // I = density/12 * ∑ (cross_ij * (||p_i||^2 + p_i·p_j + ||p_j||^2))
    float inertia             = (density / 12.0f) * std::abs(i_accum);
    em.bodies.inertia[id]     = inertia;
    em.bodies.inv_inertia[id] = (inertia > 0.0f ? 1.0f / inertia : 0.0f);
}

void rbs::kill_entity(system_context* cntx, uint32_t id) {
    if (!cntx->entity_manager.verify_id(id)) {
        return;
    }
    cntx->entity_manager.remove_entity(id);
}

void rbs::set_position(system_context* cntx, uint32_t id, glm::vec2 pos) {
    if (!cntx->entity_manager.verify_id(id)) {
        return;
    }
    cntx->entity_manager.bodies.position[id] = pos;
}

void rbs::set_velocity(system_context* cntx, uint32_t id, glm::vec2 vel) {
    if (!cntx->entity_manager.verify_id(id)) {
        return;
    }
    cntx->entity_manager.bodies.velocity[id] = vel;
}

void rbs::apply_force(system_context* cntx, uint32_t id, glm::vec2 force) {
    // if (!cntx->entity_manager.verify_id(id)) return;
    cntx->entity_manager.bodies.force[id] += force;
}

void rbs::apply_torque(system_context* cntx, uint32_t id, float torque) {
    // if (!cntx->entity_manager.verify_id(id)) return;
    cntx->entity_manager.bodies.torque[id] += torque;
}

[[nodiscard]] glm::vec2 rbs::get_position(system_context* cntx, uint32_t id) {
    if (!cntx->entity_manager.verify_id(id)) {
        return {};
    }
    return cntx->entity_manager.bodies.position[id];
}

[[nodiscard]] float rbs::get_rotation(system_context* cntx, uint32_t id) {
    if (!cntx->entity_manager.verify_id(id)) {
        return -1;
    }
    return cntx->entity_manager.bodies.rotation[id];
}

[[nodiscard]] float rbs::get_circle_radius(system_context* cntx, uint32_t id) {
    if (!cntx->entity_manager.verify_id(id)) {
        return -1;
    }
    return cntx->entity_manager.bodies.collider[id].circle.radius;
}

[[nodiscard]] std::vector<glm::vec2> rbs::get_convex_verticies(system_context* cntx, uint32_t id) {
    if (!cntx->entity_manager.verify_id(id)) {
        return {};
    }
    return std::vector<glm::vec2>(cntx->vertex_pool.begin() + cntx->entity_manager.bodies.collider[id].convex.begin,
                                  cntx->vertex_pool.begin() + cntx->entity_manager.bodies.collider[id].convex.end);
}
