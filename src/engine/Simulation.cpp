#include "Simulation.h"

void rbs::step(PhyiscsWorld* cntx)
{
    constraintIteration(cntx);

    EntityManager<ENTITY_CAP>& mgr = cntx->entity_manager;
    for (int id = 0; id < cntx->max_id; id++)
    {
        if (!mgr.isAlive(id))
            continue;

        if (!mgr.isMoveable(id))
            continue;

        if (!mgr.isConvex(id))
            circleBoundaryCollision(cntx, id);

        if (mgr.hasGravity(id))
            applyGravity(cntx, id);

        integrate(cntx, id);
    }
}
void rbs::applyGravity(PhyiscsWorld* cntx, unsigned int id)
{
    cntx->entity_manager.body[id].velocity.y += cntx->gravity;
}

void rbs::integrate(PhyiscsWorld* cntx, unsigned int id)
{
    Body& body = cntx->entity_manager.body[id];
    body.velocity += (body.force / body.mass) * cntx->simulation_timestep;
    body.position += body.velocity * cntx->simulation_timestep;
    body.force = {0, 0};
}

void rbs::constraintIteration(PhyiscsWorld* cntx)
{
    for (int i = 0; i < cntx->max_id; i++)
    {
        if (!cntx->entity_manager.hasCollider(i) || !cntx->entity_manager.isAlive(i))
            continue;
        if (cntx->entity_manager.isConvex(i))
        {
            continue;
        }

        for (int j = 0; j < cntx->max_id; j++)
        {
            if (!cntx->entity_manager.hasCollider(i) || !cntx->entity_manager.isAlive(i))
                continue;
            if (j == i)
                continue;

            circleCirlceCollision(cntx, i, j);
        }
    }
}

void rbs::circleBoundaryCollision(PhyiscsWorld* cntx, unsigned int id)
{
    Body& body = cntx->entity_manager.body[id];
    float radius = cntx->entity_manager.collider[id].circle_collider.radius;

    if (body.position.x - radius <= cntx->simulation_bounds[0].x)
    {
        body.position.x = cntx->simulation_bounds[0].x + radius;
        body.velocity.x *= -body.elasticity;
    }
    else if (body.position.x + radius >= cntx->simulation_bounds[1].x)
    {
        body.position.x = cntx->simulation_bounds[1].x - radius;
        body.velocity.x *= -body.elasticity;
    }

    if (body.position.y - radius <= cntx->simulation_bounds[0].y)
    {
        body.position.y = cntx->simulation_bounds[0].y + radius;
        body.velocity.y *= -body.elasticity;
    }
    else if (body.position.y + radius >= cntx->simulation_bounds[1].y)
    {
        body.position.y = cntx->simulation_bounds[1].y - radius;
        body.velocity.y *= -body.elasticity;
    }
}

void rbs::convexBoundaryCollision(PhyiscsWorld* cntx, unsigned int id)
{
}
void rbs::circleCirlceCollision(PhyiscsWorld* cntx, unsigned int id_a, unsigned int id_b)
{
    Body& body_a = cntx->entity_manager.body[id_a];
    Body& body_b = cntx->entity_manager.body[id_b];

    float radius_a = cntx->entity_manager.collider[id_a].circle_collider.radius;
    float radius_b = cntx->entity_manager.collider[id_b].circle_collider.radius;

    glm::vec2 diff = body_a.position - body_b.position;
    float dist = glm::length(diff);
    float overlap = radius_a + radius_b - dist;

    if (overlap < 0)
        return;

    // Avoid division by zero
    glm::vec2 normal = (dist > 0) ? diff / dist : glm::vec2(1, 0);

    // Separate circles
    body_a.position += normal * (overlap / 2.0f);
    body_b.position -= normal * (overlap / 2.0f);

    // Calculate impulse for collision response
    glm::vec2 relativeVelocity = body_a.velocity - body_b.velocity;
    float velAlongNormal = glm::dot(relativeVelocity, normal);

    // Only resolve if objects are moving toward each other
    if (velAlongNormal > 0)
        return;

    // Calculate restitution (elasticity)
    float e = std::min(body_a.elasticity, body_b.elasticity);

    // Calculate impulse scalar
    float j = -(1.0f + e) * velAlongNormal;
    j /= (1.0f / body_a.mass) + (1.0f / body_b.mass);

    // Apply impulse
    glm::vec2 impulse = j * normal;
    body_a.velocity += impulse / body_a.mass;
    body_b.velocity -= impulse / body_b.mass;
}

void rbs::circleConvexCollision(PhyiscsWorld* cntx, unsigned int id_a, unsigned int id_b)
{
}
void rbs::convexConvexCollision(PhyiscsWorld* cntx, unsigned int id_a, unsigned int id_b)
{
}

void rbs::applyForce(PhyiscsWorld* cntx, unsigned int id)
{
}
