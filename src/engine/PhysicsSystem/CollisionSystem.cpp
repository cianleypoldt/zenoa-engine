#include "CollisionSystem.h"
#include "../../graphics/Renderer.h"
#include "../../utility/Utility.h"
#include "../SystemContext.h"

void CollisionSystem::apply(SystemContext* cntx) {
    for (uint32_t i = 0; i < cntx->entity_manager.circle_colliders.count; i++) {
        uint32_t circle_a = cntx->entity_manager.circle_colliders.id[i];
        for (uint32_t j = 0; j < cntx->entity_manager.circle_colliders.count; j++) {
            uint32_t circle_b = cntx->entity_manager.circle_colliders.id[j];
            if (circle_a != circle_b) [[likely]]
                circleCircleCollision(cntx, circle_a, circle_b);
        }
    }
    for (uint32_t i = 0; i < cntx->entity_manager.circle_colliders.count; i++) {
        uint32_t circle = cntx->entity_manager.circle_colliders.id[i];
        for (uint32_t j = 0; j < cntx->entity_manager.convex_colliders.count; j++) {
            uint32_t convex = cntx->entity_manager.convex_colliders.id[j];
            circleConvexCollision(cntx, circle, convex);
        }
    }
    for (uint32_t i = 0; i < cntx->entity_manager.convex_colliders.count; i++) {
        uint32_t convex_a = cntx->entity_manager.convex_colliders.id[i];
        for (uint32_t j = 0; j < cntx->entity_manager.convex_colliders.count; j++) {
            uint32_t convex_b = cntx->entity_manager.convex_colliders.id[j];
            if (convex_a != convex_b) [[likely]]
                convexConvexCollision(cntx, convex_a, convex_b);
        }
    }
}

void CollisionSystem::circleCircleCollision(SystemContext* cntx, uint32_t id_a, uint32_t id_b) {
    auto& bodies = cntx->entity_manager.bodies;

    float radius_a = bodies.collider[id_a].circle.radius;
    float radius_b = bodies.collider[id_b].circle.radius;

    glm::vec2 diff = bodies.position[id_b] - bodies.position[id_a];
    float distance = glm::length(diff);
    float overlap = (radius_a + radius_b) - distance;
    if (overlap < 0) return;

    glm::vec2 collision_normal_ab = diff / distance;

    float total_inv_mass = bodies.invMass[id_a] + bodies.invMass[id_b];
    if (total_inv_mass > 0) {
        bodies.position[id_a] -= collision_normal_ab * overlap * (bodies.invMass[id_a] / total_inv_mass);
        bodies.position[id_b] += collision_normal_ab * overlap * (bodies.invMass[id_b] / total_inv_mass);
    }

    glm::vec2 relative_velocity = bodies.velocity[id_b] - bodies.velocity[id_a];
    float rv_along_normal = glm::dot(relative_velocity, collision_normal_ab);
    if (rv_along_normal > 0) return;

    float elasticity_coefficient = (1 + std::min(bodies.elasticity[id_a], bodies.elasticity[id_b]));
    float numerator = -elasticity_coefficient * rv_along_normal;
    float denominator = bodies.invMass[id_a] + bodies.invMass[id_b];
    float impulse = numerator / denominator;

    bodies.velocity[id_a] -= collision_normal_ab * impulse * bodies.invMass[id_a];
    bodies.velocity[id_b] += collision_normal_ab * impulse * bodies.invMass[id_b];

    relative_velocity = bodies.velocity[id_b] - bodies.velocity[id_a];

    radius_a *= -1;
    glm::vec2 collision_tangent = glm::vec2(-collision_normal_ab.y, collision_normal_ab.x);

    glm::vec2 contact_vector_a = collision_normal_ab * radius_a;
    glm::vec2 contact_vector_b = collision_normal_ab * -radius_b;

    float rot_a = bodies.angular_velocity[id_a] * radius_a;
    float rot_b = bodies.angular_velocity[id_b] * radius_b;

    float rv_along_surface = glm::dot(relative_velocity, collision_tangent) + rot_a - rot_b;

    float friction_coefficient = std::min(bodies.friction[id_a], bodies.friction[id_b]);

    float friction_denominator = bodies.invMass[id_a] + bodies.invMass[id_b] +
                                 bodies.invInertia[id_a] * radius_a * radius_a +
                                 bodies.invInertia[id_b] * radius_b * radius_b;

    float friction_impulse = -rv_along_surface / friction_denominator;

    friction_impulse = glm::clamp(friction_impulse, -friction_coefficient * std::abs(impulse), friction_coefficient * std::abs(impulse));

    glm::vec2 friction_force = collision_tangent * friction_impulse;

    bodies.velocity[id_a] -= friction_force * bodies.invMass[id_a];
    bodies.velocity[id_b] += friction_force * bodies.invMass[id_b];

    float torque_a = contact_vector_a.x * friction_force.y - contact_vector_a.y * friction_force.x;
    float torque_b = contact_vector_b.x * friction_force.y - contact_vector_b.y * friction_force.x;

    bodies.angular_velocity[id_a] += torque_a * bodies.invInertia[id_a];
    bodies.angular_velocity[id_b] += torque_b * bodies.invInertia[id_b];
}

void CollisionSystem::circleConvexCollision(SystemContext* cntx, uint32_t id_circle, uint32_t id_convex) {
    auto& bodies = cntx->entity_manager.bodies;
    const auto& circle_pos = bodies.position[id_circle];
    const auto& convex_pos = bodies.position[id_convex];
    const float circle_radius = bodies.collider[id_circle].circle.radius;
    const auto& convex = bodies.collider[id_convex].convex;

    // Quick bounding radius check
    glm::vec2 relative_pos = convex_pos - circle_pos;
    if (glm::length(relative_pos) > circle_radius + convex.bounding_radius) return;

    // Find closest point on convex to circle
    glm::vec2 closest_point;
    float min_distance = std::numeric_limits<float>::max();

    for (int i = convex.begin; i < convex.end; i++) {
        // Get current and next vertex (with wraparound)
        const glm::vec2& p1 = cntx->vertex_pool_worldspace[i];
        const glm::vec2& p2 = cntx->vertex_pool_worldspace[i < convex.end - 1 ? i + 1 : convex.begin];

        // Edge vector and circle position relative to p1
        glm::vec2 edge = p2 - p1;
        glm::vec2 circle_to_p1 = circle_pos - p1;

        // Skip if circle is on the wrong side of the edge
        if (circle_to_p1.x * edge.y - circle_to_p1.y * edge.x > 0) continue;

        // Project circle center onto edge
        float edge_length = glm::length(edge);
        glm::vec2 edge_dir = edge / edge_length;
        float projection = glm::dot(edge_dir, circle_to_p1);

        // Find closest point on edge
        glm::vec2 candidate_point;
        if (projection <= 0)
            candidate_point = p1;
        else if (projection >= edge_length)
            candidate_point = p2;
        else
            candidate_point = p1 + edge_dir * projection;

        // Update if this is the closest point so far
        float distance = glm::length(candidate_point - circle_pos);
        if (distance < min_distance) {
            closest_point = candidate_point;
            min_distance = distance;
        }
    }
}

void CollisionSystem::convexConvexCollision(SystemContext* cntx, uint32_t id_a,
                                            uint32_t id_b) {}
