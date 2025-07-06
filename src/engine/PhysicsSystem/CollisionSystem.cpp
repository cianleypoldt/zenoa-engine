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

void CollisionSystem::circleCircleCollision(SystemContext* cntx, uint32_t id_a,
                                            uint32_t id_b) {
    auto& bodies = cntx->entity_manager.bodies;

    float radius_a = bodies.collider[id_a].circle.radius;
    float radius_b = bodies.collider[id_b].circle.radius;

    glm::vec2 diff = bodies.position[id_b] - bodies.position[id_a];
    if (glm::length(diff) > radius_a + radius_b)
        return;
    glm::vec2 collision_normal_ab =
        glm::normalize(bodies.position[id_b] - bodies.position[id_a]);

    glm::vec2 relative_velocity = bodies.velocity[id_b] - bodies.velocity[id_a];
    float rv_along_normal = glm::dot(relative_velocity, collision_normal_ab);

    if (rv_along_normal > 0)
        return;

    float j = -(1 + std::min(bodies.elasticity[id_a], bodies.elasticity[id_b])) *
              rv_along_normal / (bodies.invMass[id_a] + bodies.invMass[id_b]);
    glm::vec2 impulse = j * collision_normal_ab;
    bodies.velocity[id_a] -= impulse * bodies.invMass[id_a];
    bodies.velocity[id_b] += impulse * bodies.invMass[id_b];

    glm::vec2 collision_tangent =
        glm::vec2(-collision_normal_ab.y, collision_normal_ab.x);
    float rv_along_surface = glm::dot(relative_velocity, collision_tangent) +
                             radius_a * bodies.angular_velocity[id_a] -
                             radius_b * bodies.angular_velocity[id_b];

    float mu = std::min(bodies.friction[id_a], bodies.friction[id_b]);

    float j_friction = -mu * rv_along_surface /
                       (bodies.invMass[id_a] + bodies.invMass[id_b] +
                        bodies.invInertia[id_a] * radius_a * radius_a +
                        bodies.invInertia[id_b] * radius_b * radius_b);

    j_friction = glm::clamp(j_friction, -mu * j, mu * j);

    glm::vec2 friction_impulse = j_friction * collision_tangent;
    bodies.velocity[id_a] -= friction_impulse * bodies.invMass[id_a];
    bodies.angular_velocity[id_a] -= j_friction * bodies.invInertia[id_a];
    bodies.velocity[id_b] += friction_impulse * bodies.invMass[id_b];
    bodies.angular_velocity[id_b] += j_friction * bodies.invInertia[id_b];
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
