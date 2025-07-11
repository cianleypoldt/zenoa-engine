#include "engine/interface.h"
#include "graphics/Renderer.h"
#include "utility/Utility.h"
#include <SFML/Graphics.hpp>

float random(float min, float max) {
    return min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (max - min);
}

constexpr int convex_count = 40;
constexpr int circle_count = 0;
int main() {
    utl::initLogger(utl::LogLevel::Debug, "physics_engine.log");
    UTL_INFO("2D Physics Engine Starting...");
    auto simulation = rbs::make_context();
    Renderer renderer(simulation, true, 120);
    rbs::setBounds(simulation, {-500, -800}, {1420, 250});
    rbs::setGravity(simulation, -9.81);
    simulation->fixed_timestep = 0.00005;
    float force_coeficient = 1000000;
    const int STEPS_PER_FRAME = 25; // Adjust as needed for desired simulation speed
    simulation->bounded = true;
    int player_id = 0;
    unsigned int id[convex_count + circle_count];
    std::vector<glm::vec2> points = {{0, 0}, {47.5, -34.5}, {29.4, -90.5}, {-29.4, -90.5}, {-47.5, -34.5}};
    std::vector<glm::vec2> points_ = {{0, 0}, {0, 100}, {50, 100}, {50, 0}};
    std::vector<glm::vec2> points__ = {
        {50.17, 2.12}, {51.02, -5.87}, {49.26, -21.17}, {42.52, -36.42}, {29.36, -47.83}, {10.89, -53.87}, {-8.55, -53.44}, {-24.72, -45.79}, {-37.85, -32.68}, {-46.67, -14.92}, {-50.23, 5.06}, {-48.91, 24.85}, {-40.67, 40.74}, {-25.49, 51.58}, {-8.13, 57.65}, {10.14, 57.42}, {27.01, 51.78}, {37.25, 40.06}, {46.12, 22.47}, {49.48, 4.16}};
    std::vector<glm::vec2> points___ = {
        {0.0f, 50.0f},
        {75.0f, 75.0f},
        {50.0f, 0.0f}};

    // populate Physicsworld
    for (int i = 0; i < convex_count; i++) {
        id[i] = rbs::addEntity(simulation, {random(-450, 1400), random(-230, 230)});
        if (i % 4 == 0)
            rbs::addConvexCollider(simulation, id[i], points, 0.000005);
        else if (i % 4 == 1)
            rbs::addConvexCollider(simulation, id[i], points_, 0.0000001);
        else if (i % 4 == 2)
            rbs::addConvexCollider(simulation, id[i], points__, 0.0000001);
        else
            rbs::addConvexCollider(simulation, id[i], points___, 0.0000001);
        renderer.addConvex(id[i]);
    }
    /**/
    for (int i = 0; i < circle_count; i++) {
        id[convex_count + i] = rbs::addEntity(simulation, {random(-450, 1400), random(-600, -230)});
        rbs::addCircleCollider(simulation, id[convex_count + i], 30, 0.0000001);
        simulation->entity_manager.bodies.friction[id[convex_count + i]] = 0;
        renderer.addCircle(id[convex_count + i]);
    }
    /**/
    // Number of simulation steps per frame

    while (renderer.active()) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            rbs::applyForce(simulation, player_id, {0, force_coeficient});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            rbs::applyForce(simulation, player_id, {0, -force_coeficient});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            rbs::applyForce(simulation, player_id, {-force_coeficient, 0});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            rbs::applyForce(simulation, player_id, {force_coeficient, 0});

        for (int step = 0; step < STEPS_PER_FRAME; step++) {
            rbs::step(simulation);
        }
        renderer.refresh(player_id);
    }
    /**/
    rbs::drop(simulation);
    return 0;
}
