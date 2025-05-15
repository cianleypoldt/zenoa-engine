#pragma once
#include "SystemContext.h"

namespace rbs
{
SystemContext* make_context();
void drop(SystemContext* cntx);

void step(SystemContext* cntx);
uint32_t getMaxEntityCount(SystemContext* cntx);
void setBounds(SystemContext* cntx, glm::vec2 bottom_left, glm::vec2 top_right);
void setGravity(SystemContext* cntx, float gravity);

[[nodiscard]] unsigned int addEntity(SystemContext* cntx, glm::vec2 pos = {0, 0}, float mass = 1);
void addConvexCollider(SystemContext* cntx, unsigned int id, std::vector<glm::vec2> points);
void addCircleCollider(SystemContext* cntx, unsigned int id, float radius);

void killEntity(SystemContext* cntx, uint32_t id);

void setPosition(SystemContext* cntx, uint32_t id, glm::vec2 pos);
void setVelocity(SystemContext* cntx, uint32_t id, glm::vec2 vel);
void applyForce(SystemContext* cntx, uint32_t id, glm::vec2 force);

void setCircleCollider(SystemContext* cntx, uint32_t id, float radius);
// void setCircleCollider(SystemContext* cntx, uint32_t id, std::vector<glm::vec2>& verticies);

[[nodiscard]] glm::vec2 getPosition(SystemContext* cntx, uint32_t id);
[[nodiscard]] float getRotation(SystemContext* cntx, uint32_t id);

[[nodiscard]] float getCircleRadius(SystemContext* cntx, uint32_t id);
[[nodiscard]] const std::vector<glm::vec2> getConvexVerticies(SystemContext* cntx, uint32_t id);

} // namespace rbs
