#include "Simulation.h"
#include "EntityComponents.h"

bool Simulation::running = false;
EntityManager<ENTITY_CAP>* Simulation::m_entity_manager = nullptr;
std::vector<glm::vec2> Simulation::m_convex_verticy_pool;

void Simulation::init()
{
    if (!m_entity_manager)
        m_entity_manager = new EntityManager<ENTITY_CAP>();
    for (unsigned int id = 0; id < ENTITY_CAP; id++)
        m_entity_manager->kill(id);
}

void Simulation::free()
{
    delete m_entity_manager;
}

unsigned int Simulation::addRigidConvex(const std::vector<glm::vec2>& points, glm::vec2 pos, float mass)
{
    for (unsigned int id = 0; id < ENTITY_CAP; ++id)
    {
        if (m_entity_manager->isAlive(id))
            continue;

        m_entity_manager->setAlive(id);

        m_entity_manager->pos[id].value = pos;
        m_entity_manager->prev_pos[id].value = pos;
        m_entity_manager->rot[id].value = 0.0f;
        m_entity_manager->prev_rot[id].value = 0.0f;
        m_entity_manager->mass[id].value = mass;

        ConvexCollider collider;
        collider.begin = static_cast<unsigned int>(m_convex_verticy_pool.size());
        collider.count = static_cast<unsigned int>(points.size());

        // Compute bounding radius
        float max_sq_dist = 0.0f;
        for (const auto& point : points)
        {
            float dist_sq = glm::dot(point, point);
            if (dist_sq > max_sq_dist)
                max_sq_dist = dist_sq;
        }

        m_convex_verticy_pool.insert(
            m_convex_verticy_pool.end(),
            points.begin(),
            points.end());

        m_entity_manager->setConvexCollider(id, collider);

        return id;
    }
    assert(false && "Entity capacity exceeded");
    return UINT_MAX;
}

unsigned int Simulation::addRigidCircle(float radius, glm::vec2 pos, float mass)
{
    for (unsigned int id = 0; id < ENTITY_CAP; ++id)
    {
        if (m_entity_manager->isAlive(id))
            continue;

        m_entity_manager->setAlive(id);

        m_entity_manager->pos[id].value = pos;
        m_entity_manager->prev_pos[id].value = pos;
        m_entity_manager->rot[id].value = 0.0f;
        m_entity_manager->prev_rot[id].value = 0.0f;
        m_entity_manager->mass[id].value = mass;

        m_entity_manager->setCircleCollider(id, {radius});

        return id;
    }
    assert(false && "Entity capacity exceeded");
    return UINT_MAX;
}

void Simulation::killEntity(unsigned int id)
{
    m_entity_manager->kill(id);
}

void Simulation::step()
{
}
