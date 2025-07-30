#pragma once
#include "bodies.h"
#include "entity_list.h"

#include <cstdint>
#include <vector>

class EntityManager
{
  public:
    static constexpr uint32_t max_entity_count = 5000;

    enum FlagBytes : uint8_t {
        ALIVE       = 1 << 0,
        MOVEABLE    = 1 << 1,
        HAS_GRAVITY = 1 << 2,
        COLLIDER    = 1 << 3,
        CONVEX      = 1 << 4
    };

    bodies_container bodies;

    uint32_t              occupied_slot_count = 0;
    std::vector<uint32_t> free_list;

    entity_id_list<max_entity_count> gravity_entities;
    entity_id_list<max_entity_count> moveable_entities;
    entity_id_list<max_entity_count> circle_colliders;
    entity_id_list<max_entity_count> convex_colliders;

    EntityManager();

    uint32_t add_entity();
    void     remove_entity(uint32_t id);

    bool verify_id(uint32_t id);

    void enable_gravity(uint32_t id);
    void disable_gravity(uint32_t id);
    void make_moveable(uint32_t id);
    void make_immoveable(uint32_t id);
    void use_circle_collider(uint32_t id);
    void use_convex_collider(uint32_t id);

    bool is_alive(uint32_t id);

  private:
};
