#pragma once
#include "ECS.h"
#include "MemoryArena.h"
#include <glm/glm.hpp>
#include <vector>

namespace Simulation
{
constexpr unsigned int ENTITY_CAP = 100;
constexpr unsigned int ARENA_BUFFER_SIZE = 500;
constexpr unsigned int INITIAL_VERTEX_POOL_SIZE = 30;

enum class RigidBody
{
    CONVEX,
    CIRCLE
};
struct context
{
    EntityManager<ENTITY_CAP>* entity_manager;
    arena frame_data;
    std::vector<glm::vec2> convex_verticy_pool;
};

// Lifecycle
context* make_context();
void drop(context* cntx);

[[nodiscard]] unsigned int addEntity(context* cntx, glm::vec2 pos = {}, float mass = 1);
void addConvexCollider(context* cntx, unsigned int id, const std::vector<glm::vec2>& points);
void addCircleCollider(context* cntx, unsigned int id, float radius);

void killEntity(context* cntx, unsigned int id);

// Simulation
void step(context* cntx);

} // namespace Simulation
