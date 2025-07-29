#pragma once
#include "context.h"

#include <cstdint>
#include <glm/ext/vector_float2.hpp>
#include <vector>

namespace rbs
{
system_context * make_context();
void             drop(system_context * cntx);

void     step(system_context * cntx);
uint32_t get_max_entity_count(system_context * cntx);
void     set_bounds(system_context * cntx, glm::vec2 bottom_left, glm::vec2 top_right);
void     set_gravity(system_context * cntx, float gravity);

[[nodiscard]] unsigned int add_entity(system_context * cntx, glm::vec2 pos = { 0, 0 }, float mass = 1);
void add_convex_collider(system_context * cntx, unsigned int id, std::vector<glm::vec2> points, float density);
void add_circle_collider(system_context * cntx, unsigned int id, float radius, float density);

void kill_entity(system_context * cntx, uint32_t id);

void set_position(system_context * cntx, uint32_t id, glm::vec2 pos);
void set_velocity(system_context * cntx, uint32_t id, glm::vec2 vel);
void apply_force(system_context * cntx, uint32_t id, glm::vec2 force);
void apply_torque(system_context * cntx, uint32_t id, float torque);

void set_circle_collider(system_context * cntx, uint32_t id, float radius);
// void setCircleCollider(system_context* cntx, uint32_t id, std::vector<glm::vec2>& verticies);

[[nodiscard]] glm::vec2 get_position(system_context * cntx, uint32_t id);
[[nodiscard]] float     get_rotation(system_context * cntx, uint32_t id);

[[nodiscard]] float                  get_circle_radius(system_context * cntx, uint32_t id);
[[nodiscard]] std::vector<glm::vec2> get_convex_verticies(system_context * cntx, uint32_t id);

}  // namespace rbs
