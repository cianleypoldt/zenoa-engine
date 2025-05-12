#pragma once
#include "ECS.h"
#include <glm/glm.hpp>
#include <vector>

constexpr unsigned int ENTITY_CAP = 100;

class Simulation
{
  public:
    enum class RigidBody
    {
        CONVEX,
        CIRCLE
    };

    static void init();
    static void free();

    [[nodiscard]] static unsigned int addRigidConvex(const std::vector<glm::vec2>& points, glm::vec2 pos = {}, float mass = 1);
    [[nodiscard]] static unsigned int addRigidCircle(float radius, glm::vec2 pos = {}, float mass = 1);

    static void killEntity(unsigned int id);

    static void step();

  private:
    Simulation() = delete;

    static bool running;

    static EntityManager<ENTITY_CAP>* m_entity_manager;
    static std::vector<glm::vec2> m_convex_verticy_pool;
};
