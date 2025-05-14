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

constexpr int count = 1000;
void ain()
{
    utl::initLogger(utl::LogLevel::Debug, "physics_engine.log");
    UTL_INFO("2D Physics Engine Starting...");

    sf::RenderWindow window(sf::VideoMode({1000, 500}), "Debug View", sf::Style::None, sf::State::Windowed);
    window.setPosition({0, 0});
    window.display();

    auto simulation = rbs::make_context();
    rbs::setBounds(simulation, {-500, -250}, {500, 250});
    rbs::setGravity(simulation, -0.1);
    simulation->fixed_timestep = 0.1;
    simulation->bounded = true;

    unsigned int id[count];

    // populate Physicsworld
    for (int i = 0; i < count; i++)
    {
        id[i] = rbs::addEntity(simulation, {random(-450, 450), random(-230, 230)});
        rbs::addCircleCollider(simulation, id[i], random(3, 15));
        rbs::applyForce(simulation, id[i], {100, 0});
        simulation->entity_manager.bodies.elasticity[id[i]] = 0.8;
    }

    sf::CircleShape circle_a;
    circle_a.setOutlineColor(sf::Color::White);
    circle_a.setOutlineThickness(1);
    circle_a.setFillColor(sf::Color::Transparent);

    while (window.isOpen())
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            window.close();

        for (int i = 0; i < count; i++)
        {
            float radius_a = rbs::getCircleRadius(simulation, id[i]);
            circle_a.setRadius(radius_a);
            circle_a.setOrigin({radius_a, radius_a});
            circle_a.setPosition(translate(rbs::getPosition(simulation, id[i])));
            window.draw(circle_a);
        }

        window.display();
        rbs::step(simulation);
        window.clear();
    }

    rbs::drop(simulation);
}
