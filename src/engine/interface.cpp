#pragma once
#include "interface.h"
// #include "../utility/Utility.h"

SystemContext* rbs::make_context() {
    return new SystemContext{};
}
void rbs::drop(SystemContext* cntx) {
    delete cntx;
}

uint32_t rbs::getMaxEntityCount(SystemContext* cntx) {
    return cntx->entity_manager.MAX_ENTITY_COUNT;
}

void rbs::setBounds(SystemContext* cntx, glm::vec2 bottom_left, glm::vec2 top_right) {
    cntx->bounded = true;
    cntx->physics_system.m_border_system.bottom_left_corner = bottom_left;
    cntx->physics_system.m_border_system.top_right_corner = top_right;
}

void rbs::setGravity(SystemContext* cntx, float gravity) {
    cntx->physics_system.m_integration_system.gravity = gravity;
}

void rbs::step(SystemContext* cntx) {
    cntx->physics_system.step(cntx);
}

[[nodiscard]] unsigned int rbs::addEntity(SystemContext* cntx, glm::vec2 pos, float mass) {
    uint32_t id = cntx->entity_manager.addEntity();
    cntx->entity_manager.bodies.position[id] = pos;
    cntx->entity_manager.bodies.mass[id] = mass;
    cntx->entity_manager.makeMoveable(id);
    cntx->entity_manager.enableGravity(id);
    return id;
}
void rbs::addCircleCollider(SystemContext* cntx, unsigned int id, float radius) {
    if (!cntx->entity_manager.verifyID(id)) return;
    cntx->entity_manager.useCircleCollider(id);
    cntx->entity_manager.bodies.collider[id].circle.radius = radius;
    cntx->entity_manager.bodies.mass[id] = 3.14 * radius * radius * (0.0001);
    cntx->entity_manager.bodies.invMass[id] = 1 / cntx->entity_manager.bodies.mass[id];
    cntx->entity_manager.bodies.inertia[id] = 0.5 * cntx->entity_manager.bodies.mass[id] * radius * radius;
    cntx->entity_manager.bodies.invInertia[id] = 1 / cntx->entity_manager.bodies.inertia[id];
}
void rbs::addConvexCollider(SystemContext* cntx, unsigned int id, std::vector<glm::vec2> vertices, float density = 0.00000005) {

    if (!cntx->entity_manager.verifyID(id)) return;

    // 1) Compute centroid
    glm::vec2 centroid{0.0f};
    for (auto& v : vertices) centroid += v;
    centroid /= vertices.size();

    // 2) Translate verts to centroid
    for (auto& v : vertices) v -= centroid;

    // 3) Store in pool & compute bounding radius
    auto& em = cntx->entity_manager;
    em.useConvexCollider(id);
    auto& cc = em.bodies.collider[id].convex;
    cc.begin = cntx->vertex_pool.size();
    cntx->vertex_pool.insert(cntx->vertex_pool.end(), vertices.begin(), vertices.end());
    cntx->vertex_pool_worldspace.insert(
        cntx->vertex_pool_worldspace.end(), vertices.begin(), vertices.end());
    cc.end = cntx->vertex_pool.size();

    float maxR2 = 0.0f;
    for (auto& v : vertices)
        maxR2 = std::max(maxR2, glm::dot(v, v));
    cc.bounding_radius = std::sqrt(maxR2);

    // 4) Compute signed area & polygon mass properties
    float A2 = 0.0f;      // twice signed area
    float I_accum = 0.0f; // accumulator for inertia integral

    for (size_t i = cc.begin; i < cc.end; ++i) {
        const auto& p1 = cntx->vertex_pool[i];
        const auto& p2 = cntx->vertex_pool[(i + 1 < cc.end) ? i + 1 : cc.begin];

        float cross = p1.x * p2.y - p2.x * p1.y;
        A2 += cross;

        // for inertia: (p1·p1 + p1·p2 + p2·p2) * cross
        float term = glm::dot(p1, p1) + glm::dot(p1, p2) + glm::dot(p2, p2);
        I_accum += cross * term;
    }

    float area = 0.5f * std::abs(A2);
    float mass = density * area;
    em.bodies.mass[id] = mass;
    em.bodies.invMass[id] = (mass > 0.0f ? 1.0f / mass : 0.0f);

    // Polygon moment of inertia about centroid:
    // I = density/12 * ∑ (cross_ij * (||p_i||^2 + p_i·p_j + ||p_j||^2))
    float inertia = (density / 12.0f) * std::abs(I_accum);
    em.bodies.inertia[id] = inertia;
    em.bodies.invInertia[id] = (inertia > 0.0f ? 1.0f / inertia : 0.0f);
}

void rbs::killEntity(SystemContext* cntx, uint32_t id) {
    if (!cntx->entity_manager.verifyID(id))
        return;
    cntx->entity_manager.removeEntity(id);
}
void rbs::setPosition(SystemContext* cntx, uint32_t id, glm::vec2 pos) {
    if (!cntx->entity_manager.verifyID(id))
        return;
    cntx->entity_manager.bodies.position[id] = pos;
}
void rbs::setVelocity(SystemContext* cntx, uint32_t id, glm::vec2 vel) {
    if (!cntx->entity_manager.verifyID(id))
        return;
    cntx->entity_manager.bodies.velocity[id] = vel;
}
void rbs::applyForce(SystemContext* cntx, uint32_t id, glm::vec2 force) {
    // if (!cntx->entity_manager.verifyID(id)) return;
    cntx->entity_manager.bodies.force[id] += force;
}
void rbs::applyTorque(SystemContext* cntx, uint32_t id, float torque) {
    // if (!cntx->entity_manager.verifyID(id)) return;
    cntx->entity_manager.bodies.torque[id] += torque;
}

[[nodiscard]] glm::vec2 rbs::getPosition(SystemContext* cntx, uint32_t id) {
    if (!cntx->entity_manager.verifyID(id))
        return {};
    return cntx->entity_manager.bodies.position[id];
}
[[nodiscard]] float rbs::getRotation(SystemContext* cntx, uint32_t id) {
    if (!cntx->entity_manager.verifyID(id))
        return -1;
    return cntx->entity_manager.bodies.rotation[id];
}
[[nodiscard]] float rbs::getCircleRadius(SystemContext* cntx, uint32_t id) {
    if (!cntx->entity_manager.verifyID(id))
        return -1;
    return cntx->entity_manager.bodies.collider[id].circle.radius;
}
[[nodiscard]] const std::vector<glm::vec2> rbs::getConvexVerticies(SystemContext* cntx, uint32_t id) {
    if (!cntx->entity_manager.verifyID(id))
        return {};
    return std::vector<glm::vec2>(
        cntx->vertex_pool.begin() + cntx->entity_manager.bodies.collider[id].convex.begin,
        cntx->vertex_pool.begin() + cntx->entity_manager.bodies.collider[id].convex.end);
}
