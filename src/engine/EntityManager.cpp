#include "EntityManager.h"

EntityManager::EntityManager()
{
    for (int i = 0; i < MAX_ENTITY_COUNT; i++)
    {
        bodies.flag[i] = 0;
    }
}

uint32_t EntityManager::addEntity()
{
    uint32_t id = -1;
    if (free_list.size() > 0)
    {
        id = free_list[free_list.size() - 1];
        free_list.pop_back();
    }
    else
        id = ++highest_id;

    bodies.flag[id] = ALIVE;

    bodies.position[id] = {0, 0};
    bodies.velocity[id] = {0, 0};
    bodies.force[id] = {0, 0};
    bodies.rotation[id] = 0;
    bodies.angular_velocity[id] = 0;
    bodies.torque[id] = 0;
    bodies.mass[id] = 0;
    bodies.invMass[id] = 0;
    bodies.inertia[id] = 0;
    bodies.invInertia[id] = 0;
    bodies.elasticity[id] = 0.8f;

    return id;
}
void EntityManager::removeEntity(uint32_t id)
{
    gravity_entities.remove(id);
    moveable_entities.remove(id);
    circle_colliders.remove(id);
    convex_colliders.remove(id);

    bodies.flag[id] = 0;

    free_list.push_back(id);
}

void EntityManager::enableGravity(uint32_t id)
{
    gravity_entities.add(id);
    bodies.flag[id] |= HAS_GRAVITY;
}
void EntityManager::disableGravity(uint32_t id)
{
    gravity_entities.remove(id);
    bodies.flag[id] &= ~HAS_GRAVITY;
}
void EntityManager::makeMoveable(uint32_t id)
{
    moveable_entities.add(id);
    bodies.flag[id] |= MOVEABLE;
}
void EntityManager::makeImmoveable(uint32_t id)
{
    moveable_entities.remove(id);
    bodies.flag[id] &= ~MOVEABLE;
}
void EntityManager::useCircleCollider(uint32_t id)
{
    circle_colliders.add(id);
    bodies.flag[id] |= COLLIDER;
    convex_colliders.remove(id);
    bodies.flag[id] &= ~CONVEX;
    bodies.collider[id].circle.radius = 0.0f;
}
void EntityManager::useConvexCollider(uint32_t id)
{
    convex_colliders.add(id);
    bodies.flag[id] |= COLLIDER;
    circle_colliders.remove(id);
    bodies.flag[id] |= CONVEX;
    bodies.collider[id].convex.begin = 0;
    bodies.collider[id].convex.end = 0;
}
