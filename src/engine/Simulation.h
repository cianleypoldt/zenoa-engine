#pragma once
#include "ECS.h"
#include "memory.h"
#include <glm/glm.hpp>
#include <vector>

namespace Simulation
{

constexpr unsigned int ENTITY_CAP = 100;

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

context* make_context();
void drop(context* cntx);

[[nodiscard]] unsigned int addEntity(context* cntx, const std::vector<glm::vec2>& points, glm::vec2 pos = {}, float mass = 1);
[[nodiscard]] unsigned int addEntity(context* cntx, float radius, glm::vec2 pos = {}, float mass = 1);

void killEntity(context* cntx, unsigned int id);

void step(context* cntx);

} // namespace Simulation
