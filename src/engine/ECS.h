#pragma once
#include "EntityComponents.h"

enum EntityFlag : uint8_t
{
    ALIVE = 0b10000000,
    CONVEX = 0b01000000,
    CIRCLE = 0b00100000,
    MOVEABLE = 0b00000001
};

template <unsigned int max_entity_count>
struct EntityManager
{
    uint8_t flag[max_entity_count];
    Position pos[max_entity_count];
    PreviousPosition prev_pos[max_entity_count];
    Rotation rot[max_entity_count];
    PreviousRotation prev_rot[max_entity_count];
    Mass mass[max_entity_count];
    Collider collider[max_entity_count];

    void setAlive(unsigned int id) { flag[id] |= EntityFlag::ALIVE; }
    void kill(unsigned int id) { flag[id] = 0; }
    void moveable(unsigned int id, bool moveavle)
    {
        if (moveavle)
            flag[id] |= -EntityFlag::MOVEABLE;
        else
            flag[id] &= ~EntityFlag::MOVEABLE;
    }
    void setConvexCollider(unsigned int id, const ConvexCollider& convex_collider)
    {
        collider[id].convex_collider = convex_collider;
        flag[id] |= EntityFlag::CONVEX;
        flag[id] &= ~EntityFlag::CIRCLE;
    }
    void setCircleCollider(unsigned int id, const CircleCollider& circle_collider)
    {
        collider[id].circle_collider = circle_collider;
        flag[id] |= EntityFlag::CIRCLE;
        flag[id] &= ~EntityFlag::CONVEX;
    }
    [[nodiscard]] bool isConvex(unsigned int id) { return (flag[id] & EntityFlag::CONVEX) != 0; }
    [[nodiscard]] bool isCircle(unsigned int id) { return (flag[id] & EntityFlag::CIRCLE) != 0; }
    [[nodiscard]] bool isAlive(unsigned int id) { return (flag[id] & EntityFlag::ALIVE) != 0; }
    [[nodiscard]] bool isMoveable(unsigned int id) { return (flag[id] & EntityFlag::MOVEABLE) != 0; }
};
