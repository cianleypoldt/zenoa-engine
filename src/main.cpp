#include "engine/interface.h"
#include "graphics/Renderer.h"
#include "utility/Utility.h"
#include <SFML/Graphics.hpp>

float random(float min, float max) {
    return min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (max - min);
}

constexpr int convex_count = 1;
constexpr int circle_count = 300;
int main() {
    utl::initLogger(utl::LogLevel::Debug, "physics_engine.log");
    UTL_INFO("2D Physics Engine Starting...");
    auto simulation = rbs::make_context();
    Renderer renderer(simulation, true, 120);
    rbs::setBounds(simulation, {-500, -800}, {1420, 250});
    rbs::setGravity(simulation, -9.81);
    simulation->fixed_timestep = 0.00005;
    simulation->bounded = true;
    unsigned int id[convex_count + circle_count];
    std::vector<glm::vec2> points;
    points = {{0, 0}, {47.5, -34.5}, {29.4, -90.5}, {-29.4, -90.5}, {-47.5, -34.5}};
    // points = {{0, 0}, {0, 200}, {100, 200}, {100, 0}};

    // populate Physicsworld
    for (int i = 0; i < convex_count; i++) {
        id[i] = rbs::addEntity(simulation, {random(-450, 1400), random(-230, 230)});
        rbs::addConvexCollider(simulation, id[i], points, 0.000005);
        // simulation->entity_manager.disableGravity(id[i]);
        renderer.addConvex(id[i]);
    }
    /**/
    for (int i = 0; i < circle_count; i++) {
        id[convex_count + i] = rbs::addEntity(simulation, {random(-450, 1400), random(-230, 230)});
        rbs::addCircleCollider(simulation, id[convex_count + i], 20, 0.0000005);
        // simulation->entity_manager.disableGravity(id[i]);
        renderer.addCircle(id[convex_count + i]);
    }
    /**/
    // Number of simulation steps per frame
    const int STEPS_PER_FRAME = 20; // Adjust as needed for desired simulation speed
    while (renderer.active()) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            rbs::applyForce(simulation, 0, {0, 600000});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            rbs::applyForce(simulation, 0, {0, -200000});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            rbs::applyForce(simulation, 0, {-200000, 0});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            rbs::applyForce(simulation, 0, {200000, 0});

        for (int step = 0; step < STEPS_PER_FRAME; step++) {
            rbs::step(simulation);
        }
        renderer.refresh();
    }
    /**/
    rbs::drop(simulation);
    return 0;
}
