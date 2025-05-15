#pragma once
#include "interface.h"
// #include "../utility/Utility.h"

SystemContext* rbs::make_context()
{
    return new SystemContext{};
}
void rbs::drop(SystemContext* cntx)
{
    delete cntx;
}

uint32_t rbs::getMaxEntityCount(SystemContext* cntx)
{
    return cntx->entity_manager.MAX_ENTITY_COUNT;
}

void rbs::setBounds(SystemContext* cntx, glm::vec2 bottom_left, glm::vec2 top_right)
{
    cntx->bounded = true;
    cntx->physics_system.m_border_system.bottom_left_corner = bottom_left;
    cntx->physics_system.m_border_system.top_right_corner = top_right;
}

void rbs::setGravity(SystemContext* cntx, float gravity)
{
    cntx->physics_system.m_integration_system.gravity = gravity;
}

void rbs::step(SystemContext* cntx)
{
    cntx->physics_system.step(cntx);
}

[[nodiscard]] unsigned int rbs::addEntity(SystemContext* cntx, glm::vec2 pos, float mass)
{
    uint32_t id = cntx->entity_manager.addEntity();
    cntx->entity_manager.bodies.position[id] = pos;
    cntx->entity_manager.bodies.mass[id] = mass;
    cntx->entity_manager.makeMoveable(id);
    cntx->entity_manager.enableGravity(id);
    return id;
}
void rbs::addCircleCollider(SystemContext* cntx, unsigned int id, float radius)
{
    if (!cntx->entity_manager.verifyID(id))
        return;
    cntx->entity_manager.useCircleCollider(id);
    cntx->entity_manager.bodies.collider[id].circle.radius = radius;
}
void rbs::addConvexCollider(SystemContext* cntx, unsigned int id, std::vector<glm::vec2> verticies)
{
    // To Calclate the bounding radius:
    // 1. Get center:
    glm::vec2 sum;
    for (auto& verticy : verticies)
        sum += verticy;
    sum /= verticies.size();

    // store verticies relative to center
    for (auto& verticy : verticies)
        verticy -= sum;

    // find the bounding radius
    float radius = 0;
    for (auto& verticy : verticies)
        radius = std::max(radius, glm::length(verticy));

    if (!cntx->entity_manager.verifyID(id))
        return;
    cntx->entity_manager.useConvexCollider(id);
    cntx->entity_manager.bodies.collider[id].convex.begin = cntx->vertex_pool.size();
    cntx->vertex_pool.insert(cntx->vertex_pool.end(), verticies.begin(), verticies.end());
    cntx->entity_manager.bodies.collider[id].convex.end = cntx->vertex_pool.size();
    cntx->entity_manager.bodies.collider[id].convex.bounding_radius = radius;
}
void rbs::killEntity(SystemContext* cntx, uint32_t id)
{
    if (!cntx->entity_manager.verifyID(id))
        return;
    cntx->entity_manager.removeEntity(id);
}
void rbs::setPosition(SystemContext* cntx, uint32_t id, glm::vec2 pos)
{
    if (!cntx->entity_manager.verifyID(id))
        return;
    cntx->entity_manager.bodies.position[id] = pos;
}
void rbs::setVelocity(SystemContext* cntx, uint32_t id, glm::vec2 vel)
{
    if (!cntx->entity_manager.verifyID(id))
        return;
    cntx->entity_manager.bodies.velocity[id] = vel;
}
void rbs::applyForce(SystemContext* cntx, uint32_t id, glm::vec2 force)
{
    if (!cntx->entity_manager.verifyID(id))
        return;
    cntx->entity_manager.bodies.force[id] = force;
}

[[nodiscard]] glm::vec2 rbs::getPosition(SystemContext* cntx, uint32_t id)
{
    if (!cntx->entity_manager.verifyID(id))
        return {};
    return cntx->entity_manager.bodies.position[id];
}
[[nodiscard]] float rbs::getRotation(SystemContext* cntx, uint32_t id)
{
    if (!cntx->entity_manager.verifyID(id))
        return -1;
    return cntx->entity_manager.bodies.rotation[id];
}
[[nodiscard]] float rbs::getCircleRadius(SystemContext* cntx, uint32_t id)
{
    if (!cntx->entity_manager.verifyID(id))
        return -1;
    return cntx->entity_manager.bodies.collider[id].circle.radius;
}
[[nodiscard]] const std::vector<glm::vec2> rbs::getConvexVerticies(SystemContext* cntx, uint32_t id)
{
    if (!cntx->entity_manager.verifyID(id))
        return {};
    return std::vector<glm::vec2>(
        cntx->vertex_pool.begin() + cntx->entity_manager.bodies.collider[id].convex.begin,
        cntx->vertex_pool.begin() + cntx->entity_manager.bodies.collider[id].convex.end);
}
