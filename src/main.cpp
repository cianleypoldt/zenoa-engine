#include "engine/interface.h"
#include "utility/Utility.h"
#include <SFML/Graphics.hpp>

sf::Vector2f translate(glm::vec2 pos)
{
    return sf::Vector2f(pos.x + 500, -pos.y + 250);
}
float random(float min, float max)
{
    return min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (max - min);
}

constexpr int count = 200;
int main()
{
    utl::initLogger(utl::LogLevel::Debug, "physics_engine.log");
    UTL_INFO("2D Physics Engine Starting...");
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 16;
    sf::RenderWindow window(sf::VideoMode({1000, 500}), "Debug View", sf::Style::None, sf::State::Windowed, settings);
    window.setPosition({0, 0});
    window.display();

    auto simulation = rbs::make_context();
    rbs::setBounds(simulation, {-500, -250}, {500, 250});
    rbs::setGravity(simulation, -9.81);
    simulation->fixed_timestep = 0.00001;
    simulation->bounded = true;
    unsigned int id[count + 1];

    // populate Physicsworld
    for (int i = 0; i < count; i++)
    {
        id[i] = rbs::addEntity(simulation, {random(-450, 450), random(-230, 230)});

        rbs::addConvexCollider(simulation, id[i], {{1, 1}, {2, 2}});
        rbs::addCircleCollider(simulation, id[i], random(10, 10));
        rbs::applyForce(simulation, id[i], {rbs::getPosition(simulation, id[i]).y * 1000, 0});
        simulation->entity_manager.bodies.mass[id[i]] = simulation->entity_manager.bodies.collider[id[i]].circle.radius;
    }

    sf::CircleShape circle_a;
    circle_a.setOutlineColor(sf::Color::White);
    circle_a.setOutlineThickness(1);
    circle_a.setFillColor({20, 20, 20});

    sf::Clock clock;
    const sf::Time frameTime = sf::seconds(1.0f / 60.0f); // 60 FPS

    // Number of simulation steps per frame
    const int STEPS_PER_FRAME = 30; // Adjust as needed for desired simulation speed
    while (window.isOpen())
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            window.close();

        for (int step = 0; step < STEPS_PER_FRAME; step++)
        {
            rbs::step(simulation);
        }
        window.clear();
        for (int i = 0; i < count; i++)
        {
            float radius_a = rbs::getCircleRadius(simulation, id[i]);
            circle_a.setRadius(radius_a);
            circle_a.setOrigin({radius_a, radius_a});
            circle_a.setPosition(translate(rbs::getPosition(simulation, id[i])));
            window.draw(circle_a);
        }

        window.display();
        sf::Time elapsed = clock.getElapsedTime();
        if (elapsed < frameTime)
        {
            sf::sleep(frameTime - elapsed);
        }
        clock.restart();
    }
    /**/
    rbs::drop(simulation);
    return 0;
}
