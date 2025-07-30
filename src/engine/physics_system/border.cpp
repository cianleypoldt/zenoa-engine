#include "border.h"

#include "../context.h"

#include <sys/types.h>

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <glm/common.hpp>
#define GlmEnableExperimental

void BorderSystem::apply(system_context* cntx) const {
    for (uint32_t i = 0; i < cntx->entity_manager.circle_colliders.count; i++) {
        sphere_border_collision(cntx, cntx->entity_manager.circle_colliders.id[i]);
    }
    for (uint32_t i = 0; i < cntx->entity_manager.convex_colliders.count; i++) {
        convex_border_collision(cntx, cntx->entity_manager.convex_colliders.id[i]);
    }
}

void BorderSystem::sphere_border_collision(system_context* cntx, uint32_t id) const {
    auto& bodies = cntx->entity_manager.bodies;
    float radius = bodies.collider[id].circle.radius;
    if (bodies.position[id].x - radius <= bottom_left_corner.x) {
        if (bodies.velocity[id].x < 0) {
            bodies.position[id].x        = bottom_left_corner.x + radius;
            float elasticity_coefficient = (1 + std::min(bodies.elasticity[id], border_elasticity));
            float vel_change             = bodies.velocity[id].x * -elasticity_coefficient;
            float impulse                = vel_change * bodies.mass[id];
            bodies.velocity[id].x += vel_change;

            float point_vel_y = bodies.velocity[id].y + bodies.angular_velocity[id] * -radius;

            float friction_coefficient = std::min(border_friction, bodies.friction[id]);
            float friction_numerator   = point_vel_y * -friction_coefficient;
            float friction_denominator = bodies.inv_mass[id] + bodies.inv_inertia[id] * radius * radius;
            float friction_impulse     = friction_numerator / friction_denominator;

            // Clamp based on normal impulse (computed earlier)
            float max_friction = std::abs(friction_coefficient) * std::abs(impulse);
            friction_impulse   = glm::clamp(friction_impulse, -max_friction, max_friction);

            // Apply friction impulse
            bodies.velocity[id].y += friction_impulse * bodies.inv_mass[id];
            bodies.angular_velocity[id] += (-radius * friction_impulse) * bodies.inv_inertia[id];
        }
    }

    else if (bodies.position[id].x + radius >= top_right_corner.x) {
        if (bodies.velocity[id].x > 0) {
            bodies.position[id].x        = top_right_corner.x - radius;
            float elasticity_coefficient = 1 + std::min(bodies.elasticity[id], border_elasticity);
            float vel_change             = bodies.velocity[id].x * -elasticity_coefficient;
            float impulse                = vel_change * bodies.mass[id];
            bodies.velocity[id].x += vel_change;

            float point_vel_y = bodies.velocity[id].y + bodies.angular_velocity[id] * radius;

            float friction_coefficient = std::min(border_friction, bodies.friction[id]);
            float friction_numerator   = point_vel_y * -friction_coefficient;
            float friction_denominator = bodies.inv_mass[id] + bodies.inv_inertia[id] * radius * radius;
            float friction_impulse     = friction_numerator / friction_denominator;

            // Clamp based on normal impulse
            float max_friction = std::abs(friction_coefficient) * std::abs(impulse);
            friction_impulse   = glm::clamp(friction_impulse, -max_friction, max_friction);

            // Apply friction impulse
            bodies.velocity[id].y += friction_impulse * bodies.inv_mass[id];
            bodies.angular_velocity[id] += (radius * friction_impulse) * bodies.inv_inertia[id];
        }
    }

    if (bodies.position[id].y - radius <= bottom_left_corner.y) {
        if (bodies.velocity[id].y < 0) {
            bodies.position[id].y        = bottom_left_corner.y + radius;
            float elasticity_coefficient = 1 + std::min(bodies.elasticity[id], border_elasticity);
            float vel_change             = bodies.velocity[id].y * -elasticity_coefficient;
            float impulse                = vel_change * bodies.mass[id];
            bodies.velocity[id].y += vel_change;

            float point_vel_x = bodies.velocity[id].x + bodies.angular_velocity[id] * radius;

            float friction_coefficient = std::min(border_friction, bodies.friction[id]);
            float friction_numerator   = point_vel_x * -friction_coefficient;
            float friction_denominator = bodies.inv_mass[id] + bodies.inv_inertia[id] * radius * radius;
            float friction_impulse     = friction_numerator / friction_denominator;

            // Clamp based on normal impulse
            float max_friction = std::abs(friction_coefficient) * std::abs(impulse);
            friction_impulse   = glm::clamp(friction_impulse, -max_friction, max_friction);

            // Apply friction impulse
            bodies.velocity[id].x += friction_impulse * bodies.inv_mass[id];
            bodies.angular_velocity[id] += (radius * friction_impulse) * bodies.inv_inertia[id];
        }
    }

    else if (bodies.position[id].y + radius >= top_right_corner.y) {
        if (bodies.velocity[id].y > 0) {
            bodies.position[id].y        = top_right_corner.y - radius;
            float elasticity_coefficient = 1 + std::min(bodies.elasticity[id], border_elasticity);
            float vel_change             = bodies.velocity[id].y * -elasticity_coefficient;
            float impulse                = vel_change * bodies.mass[id];
            bodies.velocity[id].y += vel_change;

            float point_vel_x = bodies.velocity[id].x + bodies.angular_velocity[id] * -radius;

            float friction_coefficient = std::min(border_friction, bodies.friction[id]);
            float friction_numerator   = point_vel_x * -friction_coefficient;
            float friction_denominator = bodies.inv_mass[id] + bodies.inv_inertia[id] * radius * radius;
            float friction_impulse     = friction_numerator / friction_denominator;

            // Clamp based on normal impulse
            float max_friction = std::abs(friction_coefficient) * std::abs(impulse);
            friction_impulse   = glm::clamp(friction_impulse, -max_friction, max_friction);

            // Apply friction impulse
            bodies.velocity[id].x += friction_impulse * bodies.inv_mass[id];
            bodies.angular_velocity[id] += (-radius * friction_impulse) * bodies.inv_inertia[id];
        }
    }
}

void BorderSystem::convex_border_collision(system_context* cntx, uint32_t id) const {
    bodies_container& bodies = cntx->entity_manager.bodies;
    float             radius = bodies.collider[id].convex.bounding_radius;

    // Bounding radius out of bounds check
    if (bodies.position[id].x - radius <= bottom_left_corner.x) {
        float max_penetration       = 0.0f;
        int   max_penetration_index = -1;

        // Find the vertex with maximum penetration
        for (uint index = bodies.collider[id].convex.begin; index < bodies.collider[id].convex.end; index++) {
            if (cntx->vertex_pool_worldspace[index].x < bottom_left_corner.x) {
                float penetration = bottom_left_corner.x - cntx->vertex_pool_worldspace[index].x;
                if (penetration > max_penetration) {
                    max_penetration       = penetration;
                    max_penetration_index = index;
                }
            }
        }

        // Apply correction and impulse only for the deepest penetrating vertex
        if (max_penetration_index != -1) {
            // Position correction
            bodies.position[id].x += max_penetration;

            glm::vec2 r           = cntx->vertex_pool_worldspace[max_penetration_index] - bodies.position[id];
            float     point_vel_x = bodies.velocity[id].x + bodies.angular_velocity[id] * -r.y;
            if (point_vel_x <= 0) {
                float elasticity_coefficient = -(1 + std::min(bodies.elasticity[id], border_elasticity));
                float numerator              = elasticity_coefficient * point_vel_x;
                float denominator            = bodies.inv_mass[id] + bodies.inv_inertia[id] * r.y * r.y;
                float impulse                = numerator / denominator;

                // Apply linear impulse
                bodies.velocity[id].x += impulse * bodies.inv_mass[id];

                // Apply angular impulse
                bodies.angular_velocity[id] += (-r.y * impulse) * bodies.inv_inertia[id];

                float point_vel_y = bodies.velocity[id].y + bodies.angular_velocity[id] * r.x;

                float friction_coefficient = -std::min(border_friction, bodies.friction[id]);
                float friction_numerator   = point_vel_y * friction_coefficient;
                float friction_denominator = bodies.inv_mass[id] + bodies.inv_inertia[id] * r.x * r.x;
                float friction_impulse     = friction_numerator / friction_denominator;

                // Clamp based on normal impulse (computed earlier)
                float max_friction = std::abs(friction_coefficient) * std::abs(impulse);
                friction_impulse   = glm::clamp(friction_impulse, -max_friction, max_friction);

                // Apply friction impulse
                bodies.velocity[id].y += friction_impulse * bodies.inv_mass[id];
                bodies.angular_velocity[id] += (r.x * friction_impulse) * bodies.inv_inertia[id];
            }
        }
    }

    else if (bodies.position[id].x + radius >= top_right_corner.x) {
        float max_penetration       = 0.0f;
        int   max_penetration_index = -1;

        // Find the vertex with maximum penetration
        for (uint index = bodies.collider[id].convex.begin; index < bodies.collider[id].convex.end; index++) {
            if (cntx->vertex_pool_worldspace[index].x > top_right_corner.x) {
                float penetration = cntx->vertex_pool_worldspace[index].x - top_right_corner.x;
                if (penetration > max_penetration) {
                    max_penetration       = penetration;
                    max_penetration_index = index;
                }
            }
        }

        // Apply correction and impulse only for the deepest penetrating vertex
        if (max_penetration_index != -1) {
            // Position correction
            bodies.position[id].x -= max_penetration;

            glm::vec2 r           = cntx->vertex_pool_worldspace[max_penetration_index] - bodies.position[id];
            float     point_vel_x = bodies.velocity[id].x + bodies.angular_velocity[id] * -r.y;
            if (point_vel_x >= 0) {
                float elasticity_coefficient = -(1 + std::min(bodies.elasticity[id], border_elasticity));
                float numerator              = elasticity_coefficient * point_vel_x;
                float denominator            = bodies.inv_mass[id] + bodies.inv_inertia[id] * r.y * r.y;
                float impulse                = numerator / denominator;

                // Apply linear impulse
                bodies.velocity[id] += glm::vec2(impulse * bodies.inv_mass[id], 0);

                // Apply angular impulse
                bodies.angular_velocity[id] += (-r.y * impulse) * bodies.inv_inertia[id];

                // Apply friction
                float point_vel_y = bodies.velocity[id].y + bodies.angular_velocity[id] * r.x;

                float friction_coefficient = -std::min(border_friction, bodies.friction[id]);
                float friction_numerator   = point_vel_y * friction_coefficient;
                float friction_denominator = bodies.inv_mass[id] + bodies.inv_inertia[id] * r.x * r.x;
                float friction_impulse     = friction_numerator / friction_denominator;

                // Clamp based on normal impulse
                float max_friction = std::abs(friction_coefficient) * std::abs(impulse);
                friction_impulse   = glm::clamp(friction_impulse, -max_friction, max_friction);

                // Apply friction impulse
                bodies.velocity[id].y += friction_impulse * bodies.inv_mass[id];
                bodies.angular_velocity[id] += (r.x * friction_impulse) * bodies.inv_inertia[id];
            }
        }
    }

    if (bodies.position[id].y - radius <= bottom_left_corner.y) {
        float max_penetration       = 0.0f;
        int   max_penetration_index = -1;

        // Find the vertex with maximum penetration
        for (uint index = bodies.collider[id].convex.begin; index < bodies.collider[id].convex.end; index++) {
            if (cntx->vertex_pool_worldspace[index].y < bottom_left_corner.y) {
                float penetration = bottom_left_corner.y - cntx->vertex_pool_worldspace[index].y;
                if (penetration > max_penetration) {
                    max_penetration       = penetration;
                    max_penetration_index = index;
                }
            }
        }

        // Apply correction and impulse only for the deepest penetrating vertex
        if (max_penetration_index != -1) {
            // Position correction
            bodies.position[id].y += max_penetration;

            glm::vec2 r           = cntx->vertex_pool_worldspace[max_penetration_index] - bodies.position[id];
            float     point_vel_y = bodies.velocity[id].y + bodies.angular_velocity[id] * r.x;
            if (point_vel_y <= 0) {
                float elasticity_coefficient = -(1 + std::min(bodies.elasticity[id], border_elasticity));
                float numerator              = elasticity_coefficient * point_vel_y;
                float denominator            = bodies.inv_mass[id] + bodies.inv_inertia[id] * r.x * r.x;
                float impulse                = numerator / denominator;

                // Apply linear impulse
                bodies.velocity[id] += glm::vec2(0, impulse * bodies.inv_mass[id]);

                // Apply angular impulse
                bodies.angular_velocity[id] += (r.x * impulse) * bodies.inv_inertia[id];

                // Apply friction
                float point_vel_x = bodies.velocity[id].x + bodies.angular_velocity[id] * -r.y;

                float friction_coefficient = -std::min(border_friction, bodies.friction[id]);
                float friction_numerator   = point_vel_x * friction_coefficient;
                float friction_denominator = bodies.inv_mass[id] + bodies.inv_inertia[id] * r.y * r.y;
                float friction_impulse     = friction_numerator / friction_denominator;

                // Clamp based on normal impulse
                float max_friction = std::abs(friction_coefficient) * std::abs(impulse);
                friction_impulse   = glm::clamp(friction_impulse, -max_friction, max_friction);

                // Apply friction impulse
                bodies.velocity[id].x += friction_impulse * bodies.inv_mass[id];
                bodies.angular_velocity[id] += (-r.y * friction_impulse) * bodies.inv_inertia[id];
            }
        }
    }

    else if (bodies.position[id].y + radius >= top_right_corner.y) {
        float max_penetration       = 0.0f;
        int   max_penetration_index = -1;

        // Find the vertex with maximum penetration
        for (uint index = bodies.collider[id].convex.begin; index < bodies.collider[id].convex.end; index++) {
            if (cntx->vertex_pool_worldspace[index].y > top_right_corner.y) {
                float penetration = cntx->vertex_pool_worldspace[index].y - top_right_corner.y;
                if (penetration > max_penetration) {
                    max_penetration       = penetration;
                    max_penetration_index = index;
                }
            }
        }

        // Apply correction and impulse only for the deepest penetrating vertex
        if (max_penetration_index != -1) {
            // Position correction
            bodies.position[id].y -= max_penetration;

            glm::vec2 r           = cntx->vertex_pool_worldspace[max_penetration_index] - bodies.position[id];
            float     point_vel_y = bodies.velocity[id].y + bodies.angular_velocity[id] * r.x;
            if (point_vel_y >= 0) {
                float elasticity_coefficient = -(1 + std::min(bodies.elasticity[id], border_elasticity));
                float numerator              = elasticity_coefficient * point_vel_y;
                float denominator            = bodies.inv_mass[id] + bodies.inv_inertia[id] * r.x * r.x;
                float impulse                = numerator / denominator;

                // Apply linear impulse
                bodies.velocity[id] += glm::vec2(0, impulse * bodies.inv_mass[id]);

                // Apply angular impulse
                bodies.angular_velocity[id] += (r.x * impulse) * bodies.inv_inertia[id];

                // Apply friction
                float point_vel_x = bodies.velocity[id].x + bodies.angular_velocity[id] * -r.y;

                float friction_coefficient = -std::min(border_friction, bodies.friction[id]);
                float friction_numerator   = point_vel_x * friction_coefficient;
                float friction_denominator = bodies.inv_mass[id] + bodies.inv_inertia[id] * r.y * r.y;
                float friction_impulse     = friction_numerator / friction_denominator;

                // Clamp based on normal impulse
                float max_friction = std::abs(friction_coefficient) * std::abs(impulse);
                friction_impulse   = glm::clamp(friction_impulse, -max_friction, max_friction);

                // Apply friction impulse
                bodies.velocity[id].x += friction_impulse * bodies.inv_mass[id];
                bodies.angular_velocity[id] += (-r.y * friction_impulse) * bodies.inv_inertia[id];
            }
        }
    }
}
