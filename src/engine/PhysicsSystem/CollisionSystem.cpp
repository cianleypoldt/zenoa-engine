#include "CollisionSystem.h"
#include "../SystemContext.h"

void CollisionSystem::apply(SystemContext* cntx)
{
    for (uint32_t i = 0; i < cntx->entity_manager.circle_colliders.count; i++)
    {
        uint32_t circle_a = cntx->entity_manager.circle_colliders.id[i];
        for (uint32_t j = 0; j < cntx->entity_manager.circle_colliders.count; j++)
        {
            uint32_t circle_b = cntx->entity_manager.circle_colliders.id[j];
            if (!(circle_a == circle_b)) [[likely]]
                circleCircleCollision(cntx, circle_a, circle_b);
        }
    }
    for (uint32_t i = 0; i < cntx->entity_manager.circle_colliders.count; i++)
    {
        uint32_t circle = cntx->entity_manager.circle_colliders.id[i];
        for (uint32_t j = 0; j < cntx->entity_manager.convex_colliders.count; j++)
        {
            uint32_t convex = cntx->entity_manager.convex_colliders.id[j];
            circleCircleCollision(cntx, circle, convex);
        }
    }
    for (uint32_t i = 0; i < cntx->entity_manager.convex_colliders.count; i++)
    {
        uint32_t convex_a = cntx->entity_manager.convex_colliders.id[i];
        for (uint32_t j = 0; j < cntx->entity_manager.convex_colliders.count; j++)
        {
            uint32_t convex_b = cntx->entity_manager.convex_colliders.id[j];
            if (!(convex_a == convex_b)) [[likely]]
                convexConvexCollision(cntx, convex_a, convex_b);
        }
    }
}
void CollisionSystem::circleCircleCollision(SystemContext* cntx, uint32_t id_a, uint32_t id_b)
{
    auto& bodies = cntx->entity_manager.bodies;

    float radius_a = bodies.collider[id_a].circle.radius;
    float radius_b = bodies.collider[id_b].circle.radius;

    glm::vec2 diff = bodies.position[id_a] - bodies.position[id_b];
    float dist = glm::length(diff);
    float overlap = radius_a + radius_b - dist;

    if (overlap < 0)
        return;

    // Avoid division by zero
    glm::vec2 normal = (dist > 0) ? diff / dist : glm::vec2(1, 0);

    // Separate circles
    bodies.position[id_a] += normal * (overlap / 2.0f);
    bodies.position[id_b] -= normal * (overlap / 2.0f);

    // Calculate impulse for collision response
    glm::vec2 relativeVelocity = bodies.velocity[id_a] - bodies.velocity[id_b];
    float velAlongNormal = glm::dot(relativeVelocity, normal);

    // Only resolve if objects are moving toward each other
    if (velAlongNormal > 0)
        return;

    // Calculate restitution (elasticity)
    float e = std::min(bodies.elasticity[id_a], bodies.elasticity[id_b]);

    // Calculate impulse scalar
    float j = -(1.0f + e) * velAlongNormal;
    j /= (1.0f / bodies.mass[id_a]) + (1.0f / bodies.mass[id_b]);

    // Apply impulse
    glm::vec2 impulse = j * normal;
    bodies.velocity[id_a] += impulse / bodies.mass[id_a];
    bodies.velocity[id_b] -= impulse / bodies.mass[id_b];
}
void CollisionSystem::circleConvexCollision(SystemContext* cntx, uint32_t id_a, uint32_t id_b)
{
}
void CollisionSystem::convexConvexCollision(SystemContext* cntx, uint32_t id_a, uint32_t id_b)
{
}
