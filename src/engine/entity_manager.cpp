#include "entity_manager.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>

EntityManager::EntityManager() {
    bodies.resize(40);
}

uint32_t EntityManager::add_entity() {
    uint32_t id = -1;
    if (!free_list.empty()) {
        id = free_list[free_list.size() - 1];
        free_list.pop_back();
    } else {
        if (bodies.size() <= occupied_slot_count) {
            bodies.resize(std::max<size_t>(static_cast<size_t>(occupied_slot_count * 1.5), occupied_slot_count + 40));
        }
        id = occupied_slot_count++;
    }

    bodies.flag[id] = ALIVE;

    bodies.position[id]         = { 0, 0 };
    bodies.velocity[id]         = { 0, 0 };
    bodies.force[id]            = { 0, 0 };
    bodies.rotation[id]         = 0;
    bodies.angular_velocity[id] = 0;
    bodies.torque[id]           = 0;
    bodies.mass[id]             = 0.1;
    bodies.inv_mass[id]         = 1 / bodies.mass[id];
    bodies.inertia[id]          = 1;
    bodies.inv_inertia[id]      = 1 / bodies.inertia[id];
    bodies.elasticity[id]       = 0.5;
    bodies.friction[id]         = 1;

    return id;
}

void EntityManager::remove_entity(uint32_t id) {
    gravity_entities.remove(id);
    moveable_entities.remove(id);
    circle_colliders.remove(id);
    convex_colliders.remove(id);

    bodies.flag[id] = 0;

    free_list.push_back(id);
}

bool EntityManager::verify_id(uint32_t id) {
    return ((id < occupied_slot_count) && (id >= 0) && is_alive(id));
}

void EntityManager::enable_gravity(uint32_t id) {
    gravity_entities.add(id);
    bodies.flag[id] |= HAS_GRAVITY;
}

void EntityManager::disable_gravity(uint32_t id) {
    gravity_entities.remove(id);
    bodies.flag[id] &= ~HAS_GRAVITY;
}

void EntityManager::make_moveable(uint32_t id) {
    moveable_entities.add(id);
    bodies.flag[id] |= MOVEABLE;
}

void EntityManager::make_immoveable(uint32_t id) {
    moveable_entities.remove(id);
    bodies.flag[id] &= ~MOVEABLE;
}

void EntityManager::use_circle_collider(uint32_t id) {
    circle_colliders.add(id);
    bodies.flag[id] |= COLLIDER;
    convex_colliders.remove(id);
    bodies.flag[id] &= ~CONVEX;
    bodies.collider[id].circle.radius = 0.0f;
}

void EntityManager::use_convex_collider(uint32_t id) {
    convex_colliders.add(id);
    bodies.flag[id] |= COLLIDER;
    circle_colliders.remove(id);
    bodies.flag[id] |= CONVEX;
    bodies.collider[id].convex.begin = 0;
    bodies.collider[id].convex.end   = 0;
}

bool EntityManager::is_alive(uint32_t id) {
    return (bodies.flag[id] & ALIVE);
}
