#include "engine/interface.h"
#include "graphics/Renderer.h"
#include "utility/Utility.h"
#include <SFML/Graphics.hpp>

float random(float min, float max) {
    return min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (max - min);
}

constexpr int convex_count = 5;
constexpr int circle_count = 3;
int main() {
    utl::initLogger(utl::LogLevel::Debug, "physics_engine.log");
    UTL_INFO("2D Physics Engine Starting...");
    auto simulation = rbs::make_context();
    Renderer renderer(simulation);
    rbs::setBounds(simulation, {-500, -250}, {500, 250});
    rbs::setGravity(simulation, -9.81);
    simulation->fixed_timestep = 0.00001;
    simulation->bounded = true;
    unsigned int id[convex_count + circle_count];

    std::vector<glm::vec2> points;
    points = {{0, 0}, {47.5, -34.5}, {29.4, -90.5}, {-29.4, -90.5}, {-47.5, -34.5}};

    // populate Physicsworld
    for (int i = 0; i < convex_count; i++) {
        id[i] = rbs::addEntity(simulation, {random(-450, 450), random(-230, 230)});
        rbs::addConvexCollider(simulation, id[i], points);
        renderer.addConvex(id[i]);
    }
    /**/
    for (int i = 0; i < circle_count; i++) {
        id[convex_count + i] = rbs::addEntity(simulation, {random(-450, 450), random(-230, 230)});
        rbs::addCircleCollider(simulation, id[convex_count + i], 20);
        renderer.addCircle(id[convex_count + i]);
    }
    /**/
    // Number of simulation steps per frame
    const int STEPS_PER_FRAME = 30; // Adjust as needed for desired simulation speed
    while (renderer.active()) {
        // Movement controls for entity 0
        // Define movement speeds as variables
        float player1Speed = 5.0f;
        float player2Speed = 5000.0f;
        {
            // Movement controls for entity 0
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                simulation->entity_manager.bodies.position[id[0]] =
                    simulation->entity_manager.bodies.position[id[0]] + glm::vec2{0, player1Speed};
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                simulation->entity_manager.bodies.position[id[0]] =
                    simulation->entity_manager.bodies.position[id[0]] + glm::vec2{0, -player1Speed};
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                simulation->entity_manager.bodies.position[id[0]] =
                    simulation->entity_manager.bodies.position[id[0]] + glm::vec2{-player1Speed, 0};
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                simulation->entity_manager.bodies.position[id[0]] =
                    simulation->entity_manager.bodies.position[id[0]] + glm::vec2{player1Speed, 0};
        }
        // Movement controls for entity 1 - controlling angular velocity
        float angularAcceleration = 5; // Adjust this value as needed for rotation speed
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            rbs::applyTorque(simulation, 1, 10000000);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            rbs::applyTorque(simulation, 1, -10000000);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            rbs::applyForce(simulation, 1, {-5000000, 0});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            rbs::applyForce(simulation, 1, {5000000, 0});

        for (int step = 0; step < STEPS_PER_FRAME; step++) {
            rbs::step(simulation);
        }
        renderer.refresh();
    }
    /**/
    rbs::drop(simulation);
    return 0;
}
