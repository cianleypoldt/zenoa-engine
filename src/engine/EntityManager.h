#pragma once
#include "Bodies.h"
#include "EntityList.h"
#include <glm/glm.hpp>
#include <stdio.h>
#include <vector>

struct EntityManager
{
    static constexpr uint32_t MAX_ENTITY_COUNT = 5000;

    enum FlagBytes : uint8_t
    {
        ALIVE = 1 << 0,
        MOVEABLE = 1 << 1,
        HAS_GRAVITY = 1 << 2,
        COLLIDER = 1 << 3,
        CONVEX = 1 << 4
    };

    Bodies bodies;

    uint32_t occupied_slot_count = 0;
    std::vector<uint32_t> free_list;

    EntityIDList<MAX_ENTITY_COUNT> gravity_entities;
    EntityIDList<MAX_ENTITY_COUNT> moveable_entities;
    EntityIDList<MAX_ENTITY_COUNT> circle_colliders;
    EntityIDList<MAX_ENTITY_COUNT> convex_colliders;

    EntityManager();

    uint32_t addEntity();
    void removeEntity(uint32_t id);

    bool verifyID(uint32_t id);

    void enableGravity(uint32_t id);
    void disableGravity(uint32_t id);
    void makeMoveable(uint32_t id);
    void makeImmoveable(uint32_t id);
    void useCircleCollider(uint32_t id);
    void useConvexCollider(uint32_t id);

    void addConvexCollider();

    bool isAlive(uint32_t id);

  private:
};
