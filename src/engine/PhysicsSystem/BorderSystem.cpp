#include "BorderSystem.h"
#include "../../graphics/Renderer.h"
#include "../SystemContext.h"
#include <SFML/Graphics.hpp>

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
    sf::CircleShape circle(5, 6);
    circle.setOrigin({3, 3});
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::White);
    circle.setOutlineThickness(1);

    Bodies& bodies = cntx->entity_manager.bodies;
    float radius = bodies.collider[id].convex.bounding_radius;
    if (bodies.position[id].x - radius <= bottom_left_corner.x)
    {
        float max = 0;
        uint32_t index = -1;

        for (int i = bodies.collider[i].convex.begin; i < bodies.collider[i].convex.end; i++)
        {
            if (cntx->vertex_pool[i].x <= max)
            {
                index = i;
                max = cntx->vertex_pool[i].x;
            }
        }
        bodies.rotation[id] += 0.001;
        circle.setPosition(Renderer::translate(cntx->vertex_pool[index] + bodies.position[id]));
    }
    else if (bodies.position[id].x + radius >= top_right_corner.x)
    {
    }

    if (bodies.position[id].y - radius <= bottom_left_corner.y)
    {
    }
    else if (bodies.position[id].y + radius >= top_right_corner.y)
    {
    }
    // circle.setPosition({500, 250});
    cntx->window.draw(circle);
}
