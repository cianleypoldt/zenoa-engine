#pragma once
#include "../utility/Utility.h"
#include "ECS.h"
#include <glm/glm.hpp>
#include <vector>

namespace rbs
{
constexpr unsigned int ENTITY_CAP = 100;
constexpr unsigned int ARENA_BUFFER_SIZE = 500;
constexpr unsigned int INITIAL_VERTEX_POOL_SIZE = 30;
constexpr float DEFAULT_ELASTICITY = 0.8f;

enum class RigidBody
{
    CONVEX,
    CIRCLE
};
struct PhyiscsWorld
{
    EntityManager<ENTITY_CAP> entity_manager;
    utl::MemoryArena memory_arena;
    std::vector<glm::vec2> convex_verticy_pool;
    float simulation_timestep = 1;
    bool bounded = false;
    glm::vec2 simulation_bounds[2] = {{0, 0}, {0, 0}};
    unsigned int iterations = 0;
    unsigned int max_id = 0;
    float gravity = -9.81;

    ~PhyiscsWorld()
    {
        memory_arena.destroy();
    }
};

// Lifecycle
PhyiscsWorld* make_context();
void drop(PhyiscsWorld* cntx);

[[nodiscard]] unsigned int addEntity(PhyiscsWorld* cntx, glm::vec2 pos = {0, 0}, float mass = 1);
void addConvexCollider(PhyiscsWorld* cntx, unsigned int id, const std::vector<glm::vec2>& points);
void addCircleCollider(PhyiscsWorld* cntx, unsigned int id, float radius);

void killEntity(PhyiscsWorld* cntx, unsigned int id);

[[nodiscard]] glm::vec2 getPosition(PhyiscsWorld* cntx, unsigned int id);
[[nodiscard]] float getRotation(PhyiscsWorld* cntx, unsigned int id);
[[nodiscard]] float getCircleRadius(PhyiscsWorld* cntx, unsigned int id);
[[nodiscard]] std::vector<glm::vec2> getConvexShape(PhyiscsWorld* cntx, unsigned int id);

// Simulation
void step(PhyiscsWorld* cntx);
void applyGravity(PhyiscsWorld* cntx, unsigned int id);
void integrate(PhyiscsWorld* cntx, unsigned int id);

void constraintIteration(PhyiscsWorld* cntx);

void circleBoundaryCollision(PhyiscsWorld* cntx, unsigned int id);
void convexBoundaryCollision(PhyiscsWorld* cntx, unsigned int id);
void circleCirlceCollision(PhyiscsWorld* cntx, unsigned int id, unsigned int id_b);
void circleConvexCollision(PhyiscsWorld* cntx, unsigned int id, unsigned int id_b);
void convexConvexCollision(PhyiscsWorld* cntx, unsigned int id, unsigned int id_b);

void applyForce(PhyiscsWorld* cntx, unsigned int id);

/*

1. Broad Phase: Dynamic AABB Tree
2. Narrow Phase: GJK + EPA + SAT + Voxel BVH
3. Contact Manifolds: Persistent
4. Constraints: MLCP Formulation
5. Solver: Projected Gauss-Seidel (warm-started, block)
6. Position Fix: Nonlinear Gauss-Seidel (split solvers)
7. Integration: Symplectic Euler
8. CCD: Conservative Advancement


*/

} // namespace rbs
