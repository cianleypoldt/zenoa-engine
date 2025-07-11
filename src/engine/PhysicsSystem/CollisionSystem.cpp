#include "CollisionSystem.h"
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
    // if (glm::length(relative_pos) > circle_radius + convex.bounding_radius) return;

    // Find closest point on convex to circle
    glm::vec2 closest_point;
    float min_distance = std::numeric_limits<float>::max();
    bool inside = true;
    for (int i = convex.begin; i < convex.end; i++) {
        // Get current and next vertex (with wraparound)
        const glm::vec2& p1 = cntx->vertex_pool_worldspace[i];
        const glm::vec2& p2 = cntx->vertex_pool_worldspace[i < convex.end - 1 ? i + 1 : convex.begin];

        // Edge vector and circle position relative to p1
        glm::vec2 edge = p2 - p1;
        glm::vec2 circle_to_p1 = circle_pos - p1;

        // Skip if circle is on the wrong side of the edge

        if (circle_to_p1.x * edge.y - circle_to_p1.y * edge.x > 0) continue;
        inside = false;
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

    if (inside) {
        min_distance = 0;
        closest_point = bodies.position[id_convex];
    }

    // Check if there's a collision
    float overlap = circle_radius - min_distance;
    if (overlap <= 0) return; // No collision

    // Create vectors from object centers to contact point
    glm::vec2 r_circle = closest_point - bodies.position[id_circle];
    glm::vec2 r_convex = closest_point - bodies.position[id_convex];

    glm::vec2 collision_normal = glm::normalize(closest_point - circle_pos);

    glm::vec2 relative_velocity = bodies.velocity[id_convex] +
                                  glm::vec2(-r_convex.y * bodies.angular_velocity[id_convex], r_convex.x * bodies.angular_velocity[id_convex]) -
                                  bodies.velocity[id_circle];

    float rv_along_normal = glm::dot(relative_velocity, collision_normal);

    if (rv_along_normal > 0) return;

    float total_inv_mass = bodies.invMass[id_circle] + bodies.invMass[id_convex];
    if (total_inv_mass > 0) {
        bodies.position[id_circle] -= collision_normal * overlap * (bodies.invMass[id_circle] / total_inv_mass);
        bodies.position[id_convex] += collision_normal * overlap * (bodies.invMass[id_convex] / total_inv_mass);
    }
    // Calculate elasticity (coefficient of restitution)
    float elasticity = 1.0f + std::min(bodies.elasticity[id_circle], bodies.elasticity[id_convex]);

    // Calculate cross products for rotational effects
    float r_convex_cross_n = r_convex.x * collision_normal.y - r_convex.y * collision_normal.x;

    // Calculate impulse scalar
    float denominator = bodies.invMass[id_circle] + bodies.invMass[id_convex] +
                        bodies.invInertia[id_convex] * r_convex_cross_n * r_convex_cross_n;

    float impulse = -(elasticity * rv_along_normal) / denominator;

    // Apply impulse to linear and angular velocities
    bodies.velocity[id_circle] -= collision_normal * impulse * bodies.invMass[id_circle];
    bodies.velocity[id_convex] += collision_normal * impulse * bodies.invMass[id_convex];
    bodies.angular_velocity[id_convex] += impulse * r_convex_cross_n * bodies.invInertia[id_convex];

    glm::vec2 tangent{-collision_normal.y, collision_normal.x};
    relative_velocity = bodies.velocity[id_convex] +
                        glm::vec2(-r_convex.y * bodies.angular_velocity[id_convex], r_convex.x * bodies.angular_velocity[id_convex]) -
                        bodies.velocity[id_circle] -
                        glm::vec2(-r_circle.y * bodies.angular_velocity[id_circle], r_circle.x * bodies.angular_velocity[id_circle]);
    float rv_along_tangent = glm::dot(relative_velocity, tangent);

    float friction_coefficient = std::min(bodies.friction[id_circle], bodies.friction[id_convex]);

    float r_circle_cross_t = r_circle.x * tangent.y - r_circle.y * tangent.x;
    float r_convex_cross_t = r_convex.x * tangent.y - r_convex.y * tangent.x;

    float friction_denominator = bodies.invMass[id_circle] + bodies.invMass[id_convex] +
                                 bodies.invInertia[id_circle] * r_circle_cross_t * r_circle_cross_t +
                                 bodies.invInertia[id_convex] * r_convex_cross_t * r_convex_cross_t;

    float friction_impulse = -rv_along_tangent / friction_denominator;

    // Clamp friction impulse
    friction_impulse = glm::clamp(friction_impulse, -friction_coefficient * std::abs(impulse),
                                  friction_coefficient * std::abs(impulse));

    // Apply friction impulse
    bodies.velocity[id_circle] -= tangent * friction_impulse * bodies.invMass[id_circle];
    bodies.velocity[id_convex] += tangent * friction_impulse * bodies.invMass[id_convex];
    bodies.angular_velocity[id_circle] -= friction_impulse * r_circle_cross_t * bodies.invInertia[id_circle];
    bodies.angular_velocity[id_convex] += friction_impulse * r_convex_cross_t * bodies.invInertia[id_convex];
}

void CollisionSystem::convexConvexCollision(SystemContext* cntx, uint32_t id_a, uint32_t id_b) {
    auto& bodies = cntx->entity_manager.bodies;

    glm::vec2 relative_pos = bodies.position[id_b] - bodies.position[id_a];
    if (glm::length(relative_pos) > bodies.collider[id_a].convex.bounding_radius + bodies.collider[id_b].convex.bounding_radius) return;

    const auto& convex_a = bodies.collider[id_a].convex;
    const auto& convex_b = bodies.collider[id_b].convex;

    float min_overlap = std::numeric_limits<float>::max();
    glm::vec2 smallest_axis;

    for (int i = convex_a.begin; i < convex_a.end; i++) {
        const glm::vec2& p1 = cntx->vertex_pool_worldspace[i];
        const glm::vec2& p2 = cntx->vertex_pool_worldspace[i < convex_a.end - 1 ? i + 1 : convex_a.begin];

        glm::vec2 edge = p2 - p1;
        glm::vec2 normal = glm::normalize(glm::vec2(-edge.y, edge.x));

        float minA = FLT_MAX, maxA = -FLT_MAX;
        float minB = FLT_MAX, maxB = -FLT_MAX;

        for (int j = convex_a.begin; j < convex_a.end; j++) {
            float projection = glm::dot(cntx->vertex_pool_worldspace[j], normal);
            minA = std::min(minA, projection);
            maxA = std::max(maxA, projection);
        }
        for (int j = convex_b.begin; j < convex_b.end; j++) {
            float projection = glm::dot(cntx->vertex_pool_worldspace[j], normal);
            minB = std::min(minB, projection);
            maxB = std::max(maxB, projection);
        }

        if (maxA < minB || maxB < minA) return;
        float overlap = std::min(maxA, maxB) - std::max(minA, minB);
        if (overlap < min_overlap) {
            min_overlap = overlap;
            smallest_axis = normal;
        }
    }
    for (int i = convex_b.begin; i < convex_b.end; i++) {
        const glm::vec2& p1 = cntx->vertex_pool_worldspace[i];
        const glm::vec2& p2 = cntx->vertex_pool_worldspace[i < convex_b.end - 1 ? i + 1 : convex_b.begin];

        glm::vec2 edge = p2 - p1;
        glm::vec2 normal = glm::normalize(glm::vec2(-edge.y, edge.x));

        float minA = FLT_MAX, maxA = -FLT_MAX;
        float minB = FLT_MAX, maxB = -FLT_MAX;

        for (int j = convex_a.begin; j < convex_a.end; j++) {
            float projection = glm::dot(cntx->vertex_pool_worldspace[j], normal);
            minA = std::min(minA, projection);
            maxA = std::max(maxA, projection);
        }
        for (int j = convex_b.begin; j < convex_b.end; j++) {
            float projection = glm::dot(cntx->vertex_pool_worldspace[j], normal);
            minB = std::min(minB, projection);
            maxB = std::max(maxB, projection);
        }

        if (maxA < minB || maxB < minA) return;
        float overlap = std::min(maxA, maxB) - std::max(minA, minB);
        if (overlap < min_overlap) {
            min_overlap = overlap;
            smallest_axis = normal;
        }
    }

    // Ensure correct collision normal direction (from A to B)
    if (glm::dot(smallest_axis, relative_pos) < 0) {
        smallest_axis = -smallest_axis;
    }

    // Apply positional correction
    float total_inv_mass = bodies.invMass[id_a] + bodies.invMass[id_b];
    if (total_inv_mass > 0) {
        // Move objects apart based on their inverse mass ratio
        bodies.position[id_a] -= smallest_axis * min_overlap * (bodies.invMass[id_a] / total_inv_mass);
        bodies.position[id_b] += smallest_axis * min_overlap * (bodies.invMass[id_b] / total_inv_mass);
    }

    // Find contact point (approximate using the average of the deepest points)
    glm::vec2 contact_point = {0, 0};
    int contact_count = 0;

    // Find deepest points on each shape relative to collision axis
    for (int i = convex_a.begin; i < convex_a.end; i++) {
        const glm::vec2& vertex = cntx->vertex_pool_worldspace[i];
        // Check if this vertex is near the contact plane
        if (glm::dot(vertex - bodies.position[id_b], smallest_axis) <= min_overlap + 0.01f) {
            contact_point += vertex;
            contact_count++;
        }
    }

    for (int i = convex_b.begin; i < convex_b.end; i++) {
        const glm::vec2& vertex = cntx->vertex_pool_worldspace[i];
        // Check if this vertex is near the contact plane
        if (glm::dot(vertex - bodies.position[id_a], -smallest_axis) <= min_overlap + 0.01f) {
            contact_point += vertex;
            contact_count++;
        }
    }

    if (contact_count > 0) {
        contact_point /= static_cast<float>(contact_count);
    } else {
        // Fallback: use midpoint between the two objects
        contact_point = (bodies.position[id_a] + bodies.position[id_b]) * 0.5f;
    }

    // Calculate vectors from object centers to contact point
    glm::vec2 r_a = contact_point - bodies.position[id_a];
    glm::vec2 r_b = contact_point - bodies.position[id_b];

    // Calculate relative velocity at contact point
    glm::vec2 rel_velocity = bodies.velocity[id_b] +
                             glm::vec2(-r_b.y * bodies.angular_velocity[id_b],
                                       r_b.x * bodies.angular_velocity[id_b]) -
                             bodies.velocity[id_a] -
                             glm::vec2(-r_a.y * bodies.angular_velocity[id_a],
                                       r_a.x * bodies.angular_velocity[id_a]);

    // Calculate velocity along normal
    float vel_along_normal = glm::dot(rel_velocity, smallest_axis);

    // Do not resolve if velocities are separating
    if (vel_along_normal > 0) return;

    // Calculate elasticity (coefficient of restitution)
    float elasticity = 1.0f + std::min(bodies.elasticity[id_a], bodies.elasticity[id_b]);

    // Calculate cross products for rotational effects
    float r_a_cross_n = r_a.x * smallest_axis.y - r_a.y * smallest_axis.x;
    float r_b_cross_n = r_b.x * smallest_axis.y - r_b.y * smallest_axis.x;

    // Calculate impulse scalar
    float impulse_denominator = bodies.invMass[id_a] + bodies.invMass[id_b] +
                                bodies.invInertia[id_a] * r_a_cross_n * r_a_cross_n +
                                bodies.invInertia[id_b] * r_b_cross_n * r_b_cross_n;

    float impulse = -(elasticity * vel_along_normal) / impulse_denominator;

    // Apply impulse to linear and angular velocities
    bodies.velocity[id_a] -= smallest_axis * impulse * bodies.invMass[id_a];
    bodies.velocity[id_b] += smallest_axis * impulse * bodies.invMass[id_b];
    bodies.angular_velocity[id_a] -= impulse * r_a_cross_n * bodies.invInertia[id_a];
    bodies.angular_velocity[id_b] += impulse * r_b_cross_n * bodies.invInertia[id_b];

    // Calculate friction impulse
    glm::vec2 tangent = glm::vec2(-smallest_axis.y, smallest_axis.x);
    float vel_along_tangent = glm::dot(rel_velocity, tangent);

    // Calculate friction coefficient
    float friction_coefficient = std::min(bodies.friction[id_a], bodies.friction[id_b]);

    // Calculate cross products for tangential direction
    float r_a_cross_t = r_a.x * tangent.y - r_a.y * tangent.x;
    float r_b_cross_t = r_b.x * tangent.y - r_b.y * tangent.x;

    // Calculate friction impulse scalar
    float friction_denominator = bodies.invMass[id_a] + bodies.invMass[id_b] +
                                 bodies.invInertia[id_a] * r_a_cross_t * r_a_cross_t +
                                 bodies.invInertia[id_b] * r_b_cross_t * r_b_cross_t;

    float friction_impulse = -vel_along_tangent / friction_denominator;

    // Clamp friction impulse
    friction_impulse = glm::clamp(friction_impulse, -friction_coefficient * std::abs(impulse),
                                  friction_coefficient * std::abs(impulse));

    // Apply friction impulse
    bodies.velocity[id_a] -= tangent * friction_impulse * bodies.invMass[id_a];
    bodies.velocity[id_b] += tangent * friction_impulse * bodies.invMass[id_b];
    bodies.angular_velocity[id_a] -= friction_impulse * r_a_cross_t * bodies.invInertia[id_a];
    bodies.angular_velocity[id_b] += friction_impulse * r_b_cross_t * bodies.invInertia[id_b];
}
