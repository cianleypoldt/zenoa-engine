#pragma once
#include "EntityComponents.h"
#include <stdio.h>

enum EntityFlag : uint8_t
{
    ALIVE = 0b10000000,
    MOVEABLE = 0b01000000,
    GRAVITY = 0b00100000,
    COLLIDER = 0b00010000,
    CONVEX = 0b00001000
};

template <unsigned int max_entity_count>
struct EntityManager
{
    uint8_t flag[max_entity_count];
    Body body[max_entity_count];
    Collider collider[max_entity_count];

    EntityManager()
    {
        for (unsigned int i = 0; i < max_entity_count; i++)
        {
            flag[i] = 0;
        }
    }

    void kill(unsigned int id) { flag[id] = 0; }

    void setFlag(unsigned int id, EntityFlag flagBit, bool value)
    {
        if (value)
            flag[id] |= flagBit;
        else
            flag[id] &= ~flagBit;
    }
    void setAlive(unsigned int id, bool alive)
    {
        if (alive)
            flag[id] |= EntityFlag::ALIVE;
        else
            flag[id] &= ~EntityFlag::ALIVE;
    }
    void setMoveable(unsigned int id, bool moveable)
    {
        if (moveable)
            flag[id] |= EntityFlag::MOVEABLE;
        else
            flag[id] &= ~EntityFlag::MOVEABLE;
    }
    void setGravity(unsigned int id, bool gravity)
    {
        if (gravity)
            flag[id] |= EntityFlag::GRAVITY;
        else
            flag[id] &= ~EntityFlag::GRAVITY;
    }
    void setCollider(unsigned int id, bool hasCollider)
    {
        if (hasCollider)
            flag[id] |= EntityFlag::COLLIDER;
        else
            flag[id] &= ~EntityFlag::COLLIDER;
    }
    void setConvex(unsigned int id, bool isConvex)
    {
        if (isConvex)
            flag[id] |= EntityFlag::CONVEX;
        else
            flag[id] &= ~EntityFlag::CONVEX;
    }
    void setConvexCollider(unsigned int id, const ConvexCollider& convex_collider)
    {
        collider[id].convex_collider = convex_collider;
        flag[id] |= EntityFlag::CONVEX;
        flag[id] |= EntityFlag::COLLIDER;
    }
    void setCircleCollider(unsigned int id, const CircleCollider& circle_collider)
    {
        collider[id].circle_collider = circle_collider;
        flag[id] &= ~EntityFlag::CONVEX;
        flag[id] |= EntityFlag::COLLIDER;
    }
    [[nodiscard]] bool getFlag(unsigned int id, EntityFlag flagBit) { return (flag[id] & flagBit) != 0; }
    [[nodiscard]] bool isAlive(unsigned int id) { return (flag[id] & EntityFlag::ALIVE) != 0; }
    [[nodiscard]] bool isMoveable(unsigned int id) { return (flag[id] & EntityFlag::MOVEABLE) != 0; }
    [[nodiscard]] bool hasGravity(unsigned int id) { return (flag[id] & EntityFlag::GRAVITY) != 0; }
    [[nodiscard]] bool hasCollider(unsigned int id) { return (flag[id] & EntityFlag::COLLIDER) != 0; }
    [[nodiscard]] bool isConvex(unsigned int id) { return (flag[id] & EntityFlag::CONVEX) != 0; }
};
