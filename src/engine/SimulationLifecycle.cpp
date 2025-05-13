#include "../utility/Utility.h"
#include "Simulation.h"

Simulation::context* Simulation::make_context()
{
    context* cntx = new context{new EntityManager<ENTITY_CAP>, utl::ScopedArena(ARENA_BUFFER_SIZE)};
    cntx->convex_verticy_pool.reserve(INITIAL_VERTEX_POOL_SIZE);
    return cntx;
}

void Simulation::drop(context* cntx)
{
    if (cntx == nullptr)
    {
        UTL_ERROR("Attempted to drop null simulation context");
        return;
    }
    delete cntx->entity_manager;
    delete cntx;
    UTL_INFO("Simulation context destroyed");
}

unsigned int Simulation::addEntity(context* cntx, glm::vec2 pos, float mass)
{
    if (cntx == nullptr) [[unlikely]]
    {
        UTL_ERROR("Null context passed to addEntity");
        return UINT_MAX;
    }

    for (unsigned int id = 0; id < ENTITY_CAP; ++id)
    {
        if (cntx->entity_manager->isAlive(id))
            continue;

        cntx->entity_manager->setAlive(id);

        cntx->entity_manager->pos[id].value = pos;
        cntx->entity_manager->prev_pos[id].value = pos;

        cntx->entity_manager->rot[id].value = 0.0f;
        cntx->entity_manager->prev_rot[id].value = 0.0f;

        cntx->entity_manager->mass[id].value = mass;

        return id;
    }
    UTL_ERROR("Entity capacity exceeded, cannot create new entity");
    return UINT_MAX;
}

void Simulation::addConvexCollider(context* cntx, unsigned int id, const std::vector<glm::vec2>& points)
{
    if (cntx == nullptr) [[unlikely]]
    {
        UTL_ERROR("Null context passed to addConvexCollider");
        return;
    }
    if (id >= ENTITY_CAP) [[unlikely]]
    {
        UTL_ERROR("Attempted to add convex collider to invalid entity id %u", id);
        return;
    }
    if (!cntx->entity_manager->isAlive(id)) [[unlikely]]
    {
        UTL_ERROR("Attempted to add convex collider to non-existent entity %u", id);
        return;
    }
    if (points.empty()) [[unlikely]]
    {
        UTL_ERROR("Empty points vector passed to addConvexCollider for entity %u", id);
        return;
    }

    ConvexCollider collider;
    collider.begin = static_cast<unsigned int>(cntx->convex_verticy_pool.size());
    collider.count = static_cast<unsigned int>(points.size());

    // Calculate maximum squared distance for bounding radius
    float max_sq_dist = 0.0f;
    for (const auto& point : points)
    {
        float dist_sq = glm::dot(point, point);
        if (dist_sq > max_sq_dist)
            max_sq_dist = dist_sq;
    }

    cntx->convex_verticy_pool.insert(cntx->convex_verticy_pool.end(), points.begin(), points.end());

    cntx->entity_manager->setConvexCollider(id, collider);
    UTL_INFO("Added convex collider to entity %u with %u vertices", id, collider.count);
}

void Simulation::addCircleCollider(context* cntx, unsigned int id, float radius)
{
    if (cntx == nullptr) [[unlikely]]
    {
        UTL_ERROR("Null context passed to addConvexCollider");
        return;
    }
    if (id >= ENTITY_CAP) [[unlikely]]
    {
        UTL_ERROR("Attempted to add circle collider to invalid entity id %u", id);
        return;
    }
    if (!cntx->entity_manager->isAlive(id)) [[unlikely]]
    {
        UTL_ERROR("Attempted to add convex collider to non-existent entity %u", id);
        return;
    }

    CircleCollider collider{radius};
    cntx->entity_manager->setCircleCollider(id, collider);
}

void Simulation::killEntity(context* cntx, unsigned int id)
{
    if (id >= ENTITY_CAP) [[unlikely]]
    {
        UTL_ERROR("Attempted to access invalid entity id %u", id);
        return;
    }
    cntx->entity_manager->kill(id);
}
glm::vec2 Simulation::getPosition(context* cntx, unsigned int id)
{
    if (id >= ENTITY_CAP) [[unlikely]]
    {
        UTL_ERROR("Attempted to access invalid entity id %u", id);
        return {};
    }
    return cntx->entity_manager->pos[id].value;
}

float Simulation::getRotation(context* cntx, unsigned int id)
{
    if (id >= ENTITY_CAP) [[unlikely]]
    {
        UTL_ERROR("Attempted to access invalid entity id %u", id);
        return 0.0f;
    }
    return cntx->entity_manager->rot[id].value;
}

float Simulation::getCircleRadius(context* cntx, unsigned int id)
{
    if (id >= ENTITY_CAP) [[unlikely]]
    {
        UTL_ERROR("Attempted to access invalid entity id %u", id);
        return {};
    }

    if (cntx->entity_manager->isConvex(id)) [[unlikely]]
    {
        UTL_ERROR("Object with id %u is not a circle\n", id);
        return -1.0f;
    }

    return cntx->entity_manager->collider[id].circle_collider.radius;
}

std::vector<glm::vec2> Simulation::getConvexShape(context* cntx, unsigned int id)
{
    if (!cntx->entity_manager->isAlive(id)) [[unlikely]]
    {
        UTL_ERROR("Object with id %u is dead\n", id);
        return {};
    }

    if (!cntx->entity_manager->isConvex(id)) [[unlikely]]
    {
        UTL_ERROR("Object with id %u is not a convex shape\n", id);
        return {};
    }

    const auto& collider = cntx->entity_manager->collider[id].convex_collider;
    return std::vector<glm::vec2>(
        cntx->convex_verticy_pool.begin() + collider.begin,
        cntx->convex_verticy_pool.begin() + collider.begin + collider.count);
}
