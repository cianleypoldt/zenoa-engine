#include "../utility/Utility.h"
#include "Simulation.h"

rbs::PhyiscsWorld* rbs::make_context()
{
    PhyiscsWorld* cntx = new PhyiscsWorld{EntityManager<ENTITY_CAP>(), utl::MemoryArena::create(ARENA_BUFFER_SIZE)};

    cntx->convex_verticy_pool.reserve(INITIAL_VERTEX_POOL_SIZE);
    return cntx;
}

void rbs::drop(PhyiscsWorld* cntx)
{
    if (cntx == nullptr)
    {
        UTL_ERROR("Attempted to drop null rbs context");
        return;
    }
    cntx->memory_arena.destroy();
    delete cntx;
    UTL_INFO("rbs context destroyed");
}

unsigned int rbs::addEntity(PhyiscsWorld* cntx, glm::vec2 pos, float mass)
{
    if (cntx == nullptr) [[unlikely]]
    {
        UTL_ERROR("Null context passed to addEntity");
        return UINT_MAX;
    }

    for (unsigned int id = 0; id < ENTITY_CAP; ++id)
    {
        if (cntx->entity_manager.isAlive(id))
            continue;

        if (id >= cntx->max_id)
            cntx->max_id++;

        cntx->entity_manager.setAlive(id, true);
        cntx->entity_manager.setMoveable(id, true);
        cntx->entity_manager.setGravity(id, true);
        cntx->entity_manager.setCollider(id, false);
        cntx->entity_manager.setConvex(id, false);

        cntx->entity_manager.body[id].position = pos;
        cntx->entity_manager.body[id].velocity = {0, 0};
        cntx->entity_manager.body[id].force = {0, 0};
        cntx->entity_manager.body[id].rotation = 0.0f;
        cntx->entity_manager.body[id].angular_velocity = 0.0f;
        cntx->entity_manager.body[id].torque = 0.0f;
        cntx->entity_manager.body[id].mass = 1;
        cntx->entity_manager.body[id].invMass = 0;
        cntx->entity_manager.body[id].invInertia = 0;
        cntx->entity_manager.body[id].inertia = 0;
        cntx->entity_manager.body[id].elasticity = DEFAULT_ELASTICITY;

        cntx->entity_manager.collider[id].circle_collider.radius = 0;

        return id;
    }
    UTL_ERROR("Entity capacity exceeded, cannot create more than %i entities", ENTITY_CAP);
    return UINT_MAX;
}

void rbs::addCircleCollider(PhyiscsWorld* cntx, unsigned int id, float radius)
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
    if (!cntx->entity_manager.isAlive(id)) [[unlikely]]
    {
        UTL_ERROR("Attempted to add convex collider to non-existent entity %u", id);
        return;
    }

    CircleCollider collider{radius};
    cntx->entity_manager.setCircleCollider(id, collider);
}

void rbs::addConvexCollider(PhyiscsWorld* cntx, unsigned int id, const std::vector<glm::vec2>& points)
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
    if (!cntx->entity_manager.isAlive(id)) [[unlikely]]
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
    collider.buffer_begin = static_cast<unsigned int>(cntx->convex_verticy_pool.size());
    collider.buffer_end = collider.buffer_begin + static_cast<unsigned int>(points.size());

    cntx->convex_verticy_pool.insert(cntx->convex_verticy_pool.end(), points.begin(), points.end());

    cntx->entity_manager.setConvexCollider(id, collider);
}

void rbs::killEntity(PhyiscsWorld* cntx, unsigned int id)
{
    if (id >= ENTITY_CAP) [[unlikely]]
    {
        UTL_ERROR("Attempted to access invalid entity id %u", id);
        return;
    }
    cntx->entity_manager.kill(id);
}

glm::vec2 rbs::getPosition(PhyiscsWorld* cntx, unsigned int id)
{
    if (id >= ENTITY_CAP) [[unlikely]]
    {
        UTL_ERROR("Attempted to access invalid entity id %u", id);
        return {};
    }
    return cntx->entity_manager.body[id].position;
}

float rbs::getRotation(PhyiscsWorld* cntx, unsigned int id)
{
    if (id >= ENTITY_CAP) [[unlikely]]
    {
        UTL_ERROR("Attempted to access invalid entity id %u", id);
        return 0.0f;
    }
    return cntx->entity_manager.body[id].rotation;
}

float rbs::getCircleRadius(PhyiscsWorld* cntx, unsigned int id)
{
    if (id >= ENTITY_CAP) [[unlikely]]
    {
        UTL_ERROR("Attempted to access invalid entity id %u", id);
        return {};
    }

    if (cntx->entity_manager.isConvex(id)) [[unlikely]]
    {
        UTL_ERROR("Object with id %u is not a circle\n", id);
        return -1.0f;
    }

    return cntx->entity_manager.collider[id].circle_collider.radius;
}

std::vector<glm::vec2> rbs::getConvexShape(PhyiscsWorld* cntx, unsigned int id)
{
    if (!cntx->entity_manager.isAlive(id)) [[unlikely]]
    {
        UTL_ERROR("Object with id %u is dead\n", id);
        return {};
    }

    if (!cntx->entity_manager.isConvex(id)) [[unlikely]]
    {
        UTL_ERROR("Object with id %u is not a convex shape\n", id);
        return {};
    }

    const auto& collider = cntx->entity_manager.collider[id].convex_collider;
    return std::vector<glm::vec2>(
        cntx->convex_verticy_pool.begin() + collider.buffer_begin,
        cntx->convex_verticy_pool.begin() + collider.buffer_end);
}
