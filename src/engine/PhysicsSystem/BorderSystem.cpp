#include "BorderSystem.h"
#include "../../graphics/Renderer.h"
#include "../SystemContext.h"
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

void BorderSystem::apply(SystemContext* cntx)
{
    for (uint32_t i = 0; i < cntx->entity_manager.circle_colliders.count; i++)
    {
        sphereBorderCollision(cntx, cntx->entity_manager.circle_colliders.id[i]);
    }
    for (uint32_t i = 0; i < cntx->entity_manager.convex_colliders.count; i++)
    {
        convexBorderCollision(cntx, cntx->entity_manager.convex_colliders.id[i]);
    }
}

void BorderSystem::sphereBorderCollision(SystemContext* cntx, uint32_t id)
{
    auto& bodies = cntx->entity_manager.bodies;
    float radius = bodies.collider[id].circle.radius;

    if (bodies.position[id].x - radius <= bottom_left_corner.x)
    {
        bodies.position[id].x = bottom_left_corner.x + radius;
        bodies.velocity[id].x *= -bodies.elasticity[id];
    }
    else if (bodies.position[id].x + radius >= top_right_corner.x)
    {
        bodies.position[id].x = top_right_corner.x - radius;
        bodies.velocity[id].x *= -bodies.elasticity[id];
    }

    if (bodies.position[id].y - radius <= bottom_left_corner.y)
    {
        bodies.position[id].y = bottom_left_corner.y + radius;
        bodies.velocity[id].y *= -bodies.elasticity[id];
        // rbs::killEntity(cntx, id);
        // uint32_t id = rbs::addEntity(cntx);
        // rbs::addCircleCollider(cntx, id, 3);
    }
    else if (bodies.position[id].y + radius >= top_right_corner.y)
    {
        bodies.position[id].y = top_right_corner.y - radius;
        bodies.velocity[id].y *= -bodies.elasticity[id];
    }
}

void BorderSystem::convexBorderCollision(SystemContext* cntx, uint32_t id)
{
    Bodies& bodies = cntx->entity_manager.bodies;
    float radius = bodies.collider[id].convex.bounding_radius;

    // Bounding radius out of bounds check
    if (bodies.position[id].x - radius <= bottom_left_corner.x)
    {
        for (int index = bodies.collider[id].convex.begin;
             index < bodies.collider[id].convex.end; index++)
        {
            // for every object vertex that is out of bounds
            if (cntx->vertex_pool_worldspace[index].x < bottom_left_corner.x)
            {
                float overlap =
                    bottom_left_corner.x - cntx->vertex_pool_worldspace[index].x;

                bodies.position[id].x += overlap;
            }
        }
    }

    else if (bodies.position[id].x + radius >= top_right_corner.x)
    {
        for (int index = bodies.collider[id].convex.begin;
             index < bodies.collider[id].convex.end; index++)
        {
            // for every object vertex that is out of bounds
            if (cntx->vertex_pool_worldspace[index].x > top_right_corner.x)
            {

                float overlap =
                    cntx->vertex_pool_worldspace[index].x - top_right_corner.x;
                glm::vec2 point_rel_to_convex =
                    cntx->vertex_pool_worldspace[index] - bodies.position[id];
                // points velocity = convex_vel + convex_angular_vel * vec(convex->point)
                glm::vec2 point_vel =
                    bodies.angular_velocity[id] *
                        glm::vec2(-point_rel_to_convex.y, point_rel_to_convex.x) +
                    bodies.velocity[id];

                bodies.position[id].x -= overlap;

                // Apply collision response
                bodies.velocity[id].x *= -bodies.elasticity[id];

                // Apply angular impulse based on contact point
                float impulse = -point_vel.x * (1.0f + bodies.elasticity[id]);
                bodies.angular_velocity[id] +=
                    impulse * (point_rel_to_convex.y / bodies.inertia[id]);
            }
        }
    }

    if (bodies.position[id].y - radius <= bottom_left_corner.y)
    {
        float overlap = 0;
        for (int index = bodies.collider[id].convex.begin;
             index < bodies.collider[id].convex.end; index++)
        {
            // for every object vertex that is out of bounds
            if (cntx->vertex_pool_worldspace[index].y < bottom_left_corner.y)
            {
                float overlap =
                    std::max(overlap, bottom_left_corner.y -
                                          cntx->vertex_pool_worldspace[index].y);
            }
        }
        bodies.position[id].y += overlap;
    }
    else if (bodies.position[id].y + radius >= top_right_corner.y)
    {
        for (int index = bodies.collider[id].convex.begin;
             index < bodies.collider[id].convex.end; index++)
        {
            // for every object vertex that is out of bounds
            if (cntx->vertex_pool_worldspace[index].y > top_right_corner.y)
            {
                float overlap =
                    cntx->vertex_pool_worldspace[index].y - top_right_corner.y;
                bodies.position[id].y -= overlap;
            }
        }
    }
}
