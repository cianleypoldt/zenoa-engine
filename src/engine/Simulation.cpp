#include "Simulation.h"
#include "EntityComponents.h"
#include <cstdlib>

Simulation::context* Simulation::make_context()
{
    malloc(200);
    context* cntx = new context{new EntityManager<ENTITY_CAP>};

    return cntx;
}

void Simulation::drop(context* cntx)
{
    delete cntx->entity_manager;
}

unsigned int Simulation::addEntity(context* cntx, const std::vector<glm::vec2>& points, glm::vec2 pos, float mass)
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

        ConvexCollider collider;
        collider.begin = static_cast<unsigned int>(cntx->convex_verticy_pool.size());
        collider.count = static_cast<unsigned int>(points.size());

        // Compute bounding radius
        float max_sq_dist = 0.0f;
        for (const auto& point : points)
        {
            float dist_sq = glm::dot(point, point);
            if (dist_sq > max_sq_dist)
                max_sq_dist = dist_sq;
        }

        cntx->convex_verticy_pool.insert(
            cntx->convex_verticy_pool.end(),
            points.begin(),
            points.end());

        cntx->entity_manager->setConvexCollider(id, collider);

        return id;
    }
    assert(false && "Entity capacity exceeded");
    return UINT_MAX;
}

unsigned int Simulation::addEntity(context* cntx, float radius, glm::vec2 pos, float mass)
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

        cntx->entity_manager->setCircleCollider(id, {radius});

        return id;
    }
    assert(false && "Entity capacity exceeded");
    return UINT_MAX;
}

void Simulation::killEntity(context* cntx, unsigned int id)
{
    cntx->entity_manager->kill(id);
}

void Simulation::step(context* cntx)
{
}
