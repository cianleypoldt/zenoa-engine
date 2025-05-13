#include "Simulation.h"
#include <cstdlib>

Simulation::context* Simulation::make_context()
{
    char* arena_mem = reinterpret_cast<char*>(malloc(ARENA_BUFFER_SIZE));
    context* cntx = new context{new EntityManager<ENTITY_CAP>, arena{arena_mem, ARENA_BUFFER_SIZE, 0}};
    cntx->convex_verticy_pool.reserve(INITIAL_VERTEX_POOL_SIZE);
    return cntx;
}

void Simulation::drop(context* cntx)
{
    delete cntx->entity_manager;
    delete cntx->frame_data.memory;
    delete cntx;
}

unsigned int Simulation::addEntity(context* cntx, glm::vec2 pos, float mass)
{
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
    assert(false && "Entity capacity exceeded");
    return UINT_MAX;
}

void Simulation::addConvexCollider(context* cntx, unsigned int id, const std::vector<glm::vec2>& points)
{
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
}

void Simulation::addCircleCollider(context* cntx, unsigned int id, float radius)
{
    CircleCollider collider{radius};
    cntx->entity_manager->setCircleCollider(id, collider);
}

void Simulation::killEntity(context* cntx, unsigned int id)
{
    cntx->entity_manager->kill(id);
}

void Simulation::step(context* cntx)
{
}
